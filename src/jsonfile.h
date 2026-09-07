#ifndef SRC_JSONFILE_H
#define SRC_JSONFILE_H

#include <QJsonObject>

/**
 * Checks if the key @p k is present in the QJsonObject @p o and is the type @p t.
 */
bool isKeyPresentAndCorrect(const QJsonObject & obj, QStringView key, QJsonValue::Type type);

#endif //SRC_JSONFILE_H
