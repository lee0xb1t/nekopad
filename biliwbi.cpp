#include "biliwbi.h"

const int BiliWbi::MIXIN_KEY_ENC_TAB[64] = {
    46, 47, 18, 2, 53, 8, 23, 32, 15, 50, 10, 31, 58, 3, 45, 35,
    27, 43, 5, 49, 33, 9, 42, 19, 29, 28, 14, 39, 12, 38, 41, 13,
    37, 48, 7, 16, 24, 55, 40, 61, 26, 17, 0, 1, 60, 51, 30, 4,
    22, 25, 54, 21, 56, 59, 6, 63, 57, 62, 11, 36, 20, 34, 44, 52
};

BiliWbi::BiliWbi(const QString &imgKey, const QString &subKey)
    : m_imgKey{imgKey}
    , m_subKey{subKey}
{
}

QString BiliWbi::generateMixinKey() {
    QString rawKey = m_imgKey + m_subKey;
    if (rawKey.length() < 64) {
        qWarning() << "WBI Key length error!";
        return "";
    }

    QString mixinKey;
    mixinKey.reserve(32);

    for (int i = 0; i < 64; ++i) {
        int index = MIXIN_KEY_ENC_TAB[i];
        if (index < rawKey.length()) {
            mixinKey.append(rawKey.at(index));
        }
    }
    return mixinKey.left(32);
}

QString BiliWbi::strictUrlEncode(const QString &input) {
    QByteArray encoded = QUrl::toPercentEncoding(input, "");
    QString result = QString::fromLatin1(encoded);

    QRegularExpression hexRe("%[0-9a-f]{2}");
    QRegularExpressionMatchIterator it = hexRe.globalMatch(result);
    QString finalResult;
    finalResult.reserve(result.length());

    for (int i = 0; i < result.length(); ++i) {
        QChar ch = result[i];
        if (ch == '%' && i + 2 < result.length()) {
            QString hexPart = result.mid(i + 1, 2);
            finalResult += '%';
            finalResult += hexPart.toUpper();
            i += 2;
        } else {
            finalResult += ch;
        }
    }

    return finalResult;
}

QString BiliWbi::signParams(const QMap<QString, QVariant> &params) {
    this->m_mixinKey = generateMixinKey();

    if (m_mixinKey.isEmpty()) {
        qCritical() << "WBI not initialized! Call fetchWbiKeys first.";
        return "";
    }

    QMap<QString, QString> sortedParams;
    long long wts = QDateTime::currentSecsSinceEpoch();

    for (auto it = params.begin(); it != params.end(); ++it) {
        sortedParams.insert(it.key(), it.value().toString());
    }
    sortedParams.insert("wts", QString::number(wts));

    QStringList paramList;
    for (auto it = sortedParams.begin(); it != sortedParams.end(); ++it) {
        QString encodedKey = strictUrlEncode(it.key());
        QString encodedValue = strictUrlEncode(it.value());
        paramList.append(QString("%1=%2").arg(encodedKey, encodedValue));
    }

    QString queryString = paramList.join("&");

    QString signStr = queryString + m_mixinKey;

    QByteArray md5Bytes = QCryptographicHash::hash(signStr.toUtf8(), QCryptographicHash::Md5);
    QString wRid = md5Bytes.toHex();

    QString finalQuery = queryString + "&w_rid=" + wRid + "&wts=" + QString::number(wts);

    return finalQuery;
}
