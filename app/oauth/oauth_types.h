#ifndef OAUTH_TYPES_H
#define OAUTH_TYPES_H

#include <QString>
#include <QMap>
#include <QVariantMap>

struct Part{
    QByteArray contentDisposition;
    QByteArray contentType;
    QByteArray data;
};

typedef QVariantMap Params;
typedef QMap<QString, Part> Parts;

#endif // OAUTH_TYPES_H
