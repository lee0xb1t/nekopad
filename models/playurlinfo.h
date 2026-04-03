#ifndef PLAYURLINFO_H
#define PLAYURLINFO_H

#include <QObject>
#include <QQmlEngine>

class PlayUrlInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int quality READ quality WRITE setQuality NOTIFY qualityChanged FINAL)
    Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged FINAL)
    Q_PROPERTY(int  timelength READ timelength WRITE setTimelength NOTIFY timelengthChanged FINAL)
    Q_PROPERTY(QString acceptFormat READ acceptFormat WRITE setAcceptFormat NOTIFY acceptFormatChanged FINAL)
    Q_PROPERTY(QString acceptQuality READ acceptQuality WRITE setAcceptQuality NOTIFY acceptQualityChanged FINAL)
    Q_PROPERTY(int audioCode READ audioCode WRITE setAudioCode NOTIFY audioCodeChanged FINAL)
    Q_PROPERTY(QString audioUrl READ audioUrl WRITE setAudioUrl NOTIFY audioUrlChanged FINAL)

public:
    explicit PlayUrlInfo(QObject *parent = nullptr);

    PlayUrlInfo(const PlayUrlInfo &other);

    PlayUrlInfo& operator=(const PlayUrlInfo &other);
    bool operator==(const PlayUrlInfo &other) const;

    int quality() const;
    void setQuality(int newQuality);

    QString format() const;
    void setFormat(const QString &newFormat);

    int timelength() const;
    void setTimelength(int newTimelength);

    QString acceptFormat() const;
    void setAcceptFormat(const QString &newAcceptFormat);

    QString acceptQuality() const;
    void setAcceptQuality(const QString &newAcceptQuality);

    int audioCode() const;
    void setAudioCode(int newAudioCode);

    QString audioUrl() const;
    void setAudioUrl(const QString &newAudioUrl);

signals:
    void qualityChanged();
    void formatChanged();

    void timelengthChanged();

    void acceptFormatChanged();

    void acceptQualityChanged();

    void audioCodeChanged();

    void audioUrlChanged();

private:
    int m_quality;
    QString m_format;
    int m_timelength;
    QString m_acceptFormat;
    QString m_acceptQuality;
    int m_audioCode;
    QString m_audioUrl;
};

#endif // PLAYURLINFO_H
