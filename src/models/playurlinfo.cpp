#include "playurlinfo.h"
#include <QDebug>

PlayUrlInfo::PlayUrlInfo(QObject *parent)
    : QObject{parent}
{}

PlayUrlInfo::PlayUrlInfo(const PlayUrlInfo &other)
    : QObject{nullptr}
    , m_quality{other.quality()}
    , m_format{other.format()}
    , m_timelength{other.timelength()}
    , m_acceptFormat{other.acceptFormat()}
    , m_acceptQuality{other.acceptQuality()}
{
    qDebug() << this->quality();
}

PlayUrlInfo& PlayUrlInfo::operator=(const PlayUrlInfo &other)
{
    if (this != &other) {
        m_quality = other.m_quality;
        m_format = other.m_format;
        m_timelength = other.m_timelength;
        m_acceptFormat = other.m_acceptFormat;
        m_acceptQuality = other.m_acceptQuality;
    }
    return *this;
}

bool PlayUrlInfo::operator==(const PlayUrlInfo &other) const
{
    return m_quality == other.m_quality &&
           m_format == other.m_format &&
           m_timelength == other.m_timelength &&
           m_acceptFormat == other.m_acceptFormat &&
           m_acceptQuality == other.m_acceptQuality;
}

int PlayUrlInfo::quality() const
{
    return m_quality;
}

void PlayUrlInfo::setQuality(int newQuality)
{
    if (m_quality == newQuality)
        return;
    m_quality = newQuality;
    emit qualityChanged();
}

QString PlayUrlInfo::format() const
{
    return m_format;
}

void PlayUrlInfo::setFormat(const QString &newFormat)
{
    if (m_format == newFormat)
        return;
    m_format = newFormat;
    emit formatChanged();
}

int PlayUrlInfo::timelength() const
{
    return m_timelength;
}

void PlayUrlInfo::setTimelength(int newTimelength)
{
    if (m_timelength == newTimelength)
        return;
    m_timelength = newTimelength;
    emit timelengthChanged();
}

QString PlayUrlInfo::acceptFormat() const
{
    return m_acceptFormat;
}

void PlayUrlInfo::setAcceptFormat(const QString &newAcceptFormat)
{
    if (m_acceptFormat == newAcceptFormat)
        return;
    m_acceptFormat = newAcceptFormat;
    emit acceptFormatChanged();
}

QString PlayUrlInfo::acceptQuality() const
{
    return m_acceptQuality;
}

void PlayUrlInfo::setAcceptQuality(const QString &newAcceptQuality)
{
    if (m_acceptQuality == newAcceptQuality)
        return;
    m_acceptQuality = newAcceptQuality;
    emit acceptQualityChanged();
}

int PlayUrlInfo::audioCode() const
{
    return m_audioCode;
}

void PlayUrlInfo::setAudioCode(int newAudioCode)
{
    if (m_audioCode == newAudioCode)
        return;
    m_audioCode = newAudioCode;
    emit audioCodeChanged();
}

QString PlayUrlInfo::audioUrl() const
{
    return m_audioUrl;
}

void PlayUrlInfo::setAudioUrl(const QString &newAudioUrl)
{
    if (m_audioUrl == newAudioUrl)
        return;
    m_audioUrl = newAudioUrl;
    emit audioUrlChanged();
}
