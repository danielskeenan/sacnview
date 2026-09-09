#include "clssnapshot.h"

#include "jsonfile.h"
#include "preferences.h"

#include <QHBoxLayout>
#include <QJsonArray>
#include <QPainter>

static const auto JSON_KEY_UNIVERSE = QStringLiteral("universe");
static const auto JSON_KEY_NAME = QStringLiteral("name");
static const auto JSON_KEY_PRIORITY = QStringLiteral("priority");
static const auto JSON_KEY_VALUES = QStringLiteral("values");

clsSnapshot::clsSnapshot(quint16 universe, CID cid, QString name, QWidget * parent)
    : QWidget(parent)
    , m_universe(universe)
    , m_priority(DEFAULT_SACN_PRIORITY)
    , m_cid(cid)
    , m_sbUniverse(new QSpinBox(this))
    , m_sbPriority(new QSpinBox(this))
    , m_btnPlayback(new QToolButton(this))
    , m_lblStatus(new QLabel(this))
    , m_controlWidget(new QWidget(this))
    , m_sender(Q_NULLPTR)
    , m_listener(Q_NULLPTR)
    , m_camera(new QSoundEffect(this))
    , m_backgroundMatches(false)
{
    m_sbUniverse->setMinimum(MIN_SACN_UNIVERSE);
    m_sbUniverse->setMaximum(MAX_SACN_UNIVERSE);
    m_sbUniverse->setValue(m_universe);
    connect(m_sbUniverse, (void (QSpinBox::*)(int))&QSpinBox::valueChanged, [this](int value) { setUniverse(value); });

    m_sbPriority->setMinimum(MIN_SACN_PRIORITY);
    m_sbPriority->setMaximum(Preferences::GetTxMaxUiPriority());
    m_sbPriority->setValue(m_priority);
    connect(m_sbPriority, (void (QSpinBox::*)(int))&QSpinBox::valueChanged, [this](int value) { setPriority(value); });

    connect(m_btnPlayback, &QAbstractButton::clicked, this, &clsSnapshot::btnEnableClicked);
    updateIcons();

    m_sender = sACNManager::Instance().getSender(m_universe, m_cid);
    m_sender->setName(name);
    connect(m_sender.data(), &sACNSentUniverse::sendingTimeout, [this]() { emit senderTimedOut(); });
    setUniverse(m_universe);

    m_btnPlayback->setAutoRaise(true);
    m_controlWidget->setAutoFillBackground(true);
    QHBoxLayout * layout = new QHBoxLayout(m_controlWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch();
    layout->addWidget(m_btnPlayback);
    layout->addWidget(m_lblStatus);
    layout->addStretch();

    m_camera->setSource(QUrl("qrc:/sound/camera.wav"));
}

clsSnapshot::~clsSnapshot() {}

void clsSnapshot::setUniverse(quint16 universe)
{
    Q_ASSERT(universe >= MIN_SACN_UNIVERSE);
    Q_ASSERT(universe <= MAX_SACN_UNIVERSE);

    m_universe = universe;

    m_sbUniverse->setValue(m_universe);

    // Bug #197
    //if (m_listener)
    //    m_listener->deleteLater();
    m_listener = sACNManager::Instance().getListener(m_universe);
    connect(m_listener.data(), &sACNListener::levelsChanged, this, &clsSnapshot::levelsChanged);
    connect(m_listener.data(), &sACNListener::sourceFound, this, &clsSnapshot::levelsChanged);
    connect(m_listener.data(), &sACNListener::sourceLost, this, &clsSnapshot::levelsChanged);
    connect(m_listener.data(), &sACNListener::sourceResumed, this, &clsSnapshot::levelsChanged);

    m_sender->setUniverse(m_universe);
}

void clsSnapshot::setPriority(quint8 priority)
{
    Q_ASSERT(priority >= MIN_SACN_PRIORITY);
    Q_ASSERT(priority <= MAX_SACN_PRIORITY);

    m_priority = priority;

    m_sbPriority->setValue(m_priority);

    m_sender->setPerSourcePriority(m_priority);
}

void clsSnapshot::updateIcons()
{
    if (!hasData())
        m_btnPlayback->setIcon(icons[ICON_SNAPSHOT]);
    else if (m_sender->isSending())
    {
        m_btnPlayback->setIcon(icons[ICON_PAUSE]);
    }
    else
        m_btnPlayback->setIcon(icons[ICON_PLAY]);

    if (!hasData())
    {
        m_lblStatus->setPixmap(statusIcons[STATUSICON_NONE]);
        m_lblStatus->setToolTip(statusIconTooltips[STATUSICON_NONE]);
    }
    else if (!m_sender->isSending())
    {
        m_lblStatus->setPixmap(statusIcons[STATUSICON_NONE]);
        m_lblStatus->setToolTip(statusIconTooltips[STATUSICON_NONE]);
    }
    else if (m_backgroundMatches)
    {
        m_lblStatus->setPixmap(statusIcons[STATUSICON_MATCHING]);
        m_lblStatus->setToolTip(statusIconTooltips[STATUSICON_MATCHING]);
    }
    else
    {
        m_lblStatus->setPixmap(statusIcons[STATUSICON_NOTMATCHING]);
        m_lblStatus->setToolTip(statusIconTooltips[STATUSICON_NOTMATCHING]);
    }

    // Enabled status
    m_sbUniverse->setEnabled(!isPlaying());
    m_sbPriority->setEnabled(!isPlaying());
}

void clsSnapshot::takeSnapshot()
{
    m_levelData.clear();

    // Copy current merged universe
    for (int addr = 0; addr < MAX_DMX_ADDRESS; addr++)
    {
        if (m_listener->mergedLevels().at(addr).level == -1)
            m_levelData.append(static_cast<char>(0));
        else
            m_levelData.append(m_listener->mergedLevels().at(addr).level);
    }

    updateIcons();

    emit snapshotTaken();
}

void clsSnapshot::playSnapshot()
{
    if (!hasData())
    {
        updateIcons();
        return;
    }

    m_sender->startSending();
    m_sender->setLevel(
        reinterpret_cast<const quint8 *>(m_levelData.constData()),
        std::min(static_cast<quint16>(m_levelData.size()), static_cast<quint16>(MAX_DMX_ADDRESS)));
    updateIcons();
    emit senderStarted();
}

void clsSnapshot::stopSnapshot()
{
    if (!hasData())
    {
        updateIcons();
        return;
    }

    m_sender->stopSending();
    updateIcons();
    emit senderStopped();
}

QJsonObject clsSnapshot::toJson() const
{
    QJsonObject o{
        {JSON_KEY_UNIVERSE, m_universe},
        {JSON_KEY_NAME, m_sender->name()},
        {JSON_KEY_PRIORITY, m_priority},
        {JSON_KEY_VALUES,
         [this]()
         {
             QJsonArray arr;
             for (const auto & v : m_levelData)
             {
                 arr.append(static_cast<uint8_t>(v));
             }
             return arr;
         }()}};
    return o;
}

clsSnapshot * clsSnapshot::fromJson(const QJsonObject & o, const CID & cid, QWidget * parent)
{
    // Keep this validation logic in sync with the JSON schema in res/sacnsnap.schema.json

    // Universe
    if (!isKeyPresentAndCorrect(o, JSON_KEY_UNIVERSE, QJsonValue::Double))
    {
        return nullptr;
    }
    const auto universe = o[JSON_KEY_UNIVERSE].toInt();
    if (universe < MIN_SACN_UNIVERSE || universe > MAX_SACN_UNIVERSE)
    {
        qWarning() << "Universe out of range:" << universe;
        return nullptr;
    }

    // Name
    if (!isKeyPresentAndCorrect(o, JSON_KEY_NAME, QJsonValue::String))
    {
        return nullptr;
    }
    const auto name = o[JSON_KEY_NAME].toString();
    if (name.size() > MAX_SOURCE_NAME_LEN)
    {
        qWarning() << "Source name too long";
        return nullptr;
    }

    // Priority
    if (!isKeyPresentAndCorrect(o, JSON_KEY_PRIORITY, QJsonValue::Double))
    {
        return nullptr;
    }
    const auto priority = o[JSON_KEY_PRIORITY].toInt();
    if (priority < MIN_SACN_PRIORITY || priority > MAX_SACN_PRIORITY)
    {
        qWarning() << "Priority out of range:" << priority;
        return nullptr;
    }

    // Values
    if (!isKeyPresentAndCorrect(o, JSON_KEY_VALUES, QJsonValue::Array))
    {
        return nullptr;
    }
    const auto & jsonValues = o[JSON_KEY_VALUES].toArray();
    // An empty array is allowed; it means this universe should be in the list but has not yet been snapshotted.
    if (jsonValues.size() > MAX_DMX_ADDRESS)
    {
        qWarning() << "Value array too big:" << jsonValues.size();
        return nullptr;
    }
    QByteArray values;
    for (const auto & jsonValue : jsonValues)
    {
        if (!jsonValue.isDouble())
        {
            qWarning() << "Value array member is wrong type:" << jsonValue.type();
            return nullptr;
        }
        const auto value = jsonValue.toInt();
        if (value < MIN_SACN_LEVEL || value > MAX_SACN_LEVEL)
        {
            qWarning() << "Value array member is out of range:" << value;
            return nullptr;
        }
        values.append(static_cast<char>(value));
    }

    // At this point all data has been extracted and validated. Create the snapshot.
    auto snapshot = new clsSnapshot(universe, cid, name, parent);
    snapshot->setPriority(priority);
    snapshot->m_levelData = values;
    snapshot->updateIcons();

    return snapshot;
}

void clsSnapshot::btnEnableClicked(bool value)
{
    Q_UNUSED(value);
    if (!hasData())
    {
        m_camera->play();
        takeSnapshot();
        return;
    }
    else if (isPlaying())
        stopSnapshot();
    else
        playSnapshot();
}

void clsSnapshot::levelsChanged()
{
    QByteArray background;

    int addr = 0;
    for (auto merged : m_listener->mergedLevels())
    {
        int level = 0;
        if ((merged.winningSource) && (merged.winningSource->src_cid == m_sender->cid()))
        {
            // I'm winning....
            if (merged.otherSources.isEmpty())
            {
                //...and the only source
                level = merged.level;
            }
            else
            {
                // ...find highest background priority
                int priority = 0;
                for (auto source : merged.otherSources)
                {
                    if (source->doing_per_channel)
                    {
                        if (source->priority_array[addr] > priority)
                        {
                            priority = source->priority;
                            level = source->level_array[addr];
                        }
                    }
                    else
                    {
                        if (source->priority > priority)
                        {
                            priority = source->priority;
                            level = source->level_array[addr];
                        }
                    }
                }
            }
        }
        else
        {
            // I'm not winning...
            level = merged.level;
        }

        background.append(level);

        if (addr < MAX_DMX_ADDRESS) addr++;
    }

    if ((background == m_levelData) != m_backgroundMatches)
    {
        m_backgroundMatches = (background == m_levelData);

        if (background == m_levelData)
            emit snapshotMatches();
        else
            emit snapshotDiffers();
    }

    updateIcons();
}
