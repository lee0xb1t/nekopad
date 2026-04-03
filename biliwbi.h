#ifndef BILIBILIWBI_H
#define BILIBILIWBI_H

#include <QString>
#include <QMap>
#include <QDebug>
#include <QUrl>
#include <QRegularExpression>
#include <QDateTime>
#include <QCryptographicHash>

class BiliWbi
{
public:
    BiliWbi(const QString &imgKey, const QString &subKey);

    QString signParams(const QMap<QString, QVariant> &params);
    QString getMixinKey() const { return m_mixinKey; }

private:
    QString generateMixinKey();
    QString strictUrlEncode(const QString &input);

    static const int MIXIN_KEY_ENC_TAB[64];

    QString m_imgKey;
    QString m_subKey;
    QString m_mixinKey;
};

#endif // BILIBILIWBI_H
