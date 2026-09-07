#include "jsonfile.h"

bool isKeyPresentAndCorrect(const QJsonObject & obj, const QStringView key, const QJsonValue::Type type)
{
    const auto v = obj.find(key);
    if (v == obj.end())
    {
        qWarning() << "Missing JSON key" << key;
        return false;
    }
    if (v->type() != type)
    {
        qWarning() << "JSON value is wrong type: Expected" << type << "got" << v->type();
        return false;
    }
    return true;
}
