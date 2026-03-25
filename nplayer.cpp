#include "nplayer.h"

NPlayer::NPlayer(QObject *parent)
    : QObject{parent}
{
    const char* args[] = {
        "--verbose=2",
        "--no-video",
    };
    m_vlcInst = libvlc_new(sizeof(args) / sizeof(args[0]), args);
    m_vlcMediaPlayer = libvlc_media_player_new(m_vlcInst);
    m_vlcEventManager = libvlc_media_player_event_manager(m_vlcMediaPlayer);

    libvlc_event_attach(m_vlcEventManager, libvlc_MediaParsedChanged, [](const struct libvlc_event_t *p_event, void *p_data) {
        NPlayer *p = static_cast<NPlayer *>(p_data);
        p->onVlcMediaParsedChanged(p_event);
    }, this);
    libvlc_event_attach(m_vlcEventManager, libvlc_MediaPlayerPlaying, [](const struct libvlc_event_t *p_event, void *p_data) {
        NPlayer *p = static_cast<NPlayer *>(p_data);
        p->onVlcMediaPlayerPlaying(p_event);
    }, this);
    libvlc_event_attach(m_vlcEventManager, libvlc_MediaPlayerEndReached, [](const struct libvlc_event_t *p_event, void *p_data) {
        NPlayer *p = static_cast<NPlayer *>(p_data);
        p->onVlcMediaPlayerEndReached(p_event);
    }, this);
}

NPlayer::~NPlayer() {
    if (m_vlcMedia) {
        libvlc_media_release(m_vlcMedia);
        m_vlcMedia = NULL;
    }

    if (m_vlcMediaPlayer) {
        libvlc_media_player_stop(m_vlcMediaPlayer);
        libvlc_media_player_release(m_vlcMediaPlayer);
        m_vlcMediaPlayer = NULL;
    }

    if (m_vlcInst) {
        libvlc_release(m_vlcInst);
        m_vlcInst = NULL;
    }
}

void NPlayer::createVlcPlayer() {
    m_vlcMedia = libvlc_media_new_location(m_vlcInst, source().toStdString().c_str());
    libvlc_media_player_set_media(m_vlcMediaPlayer, m_vlcMedia);
}

void NPlayer::releaseVlcPlayer() {
    if (m_vlcMediaPlayer) {
        libvlc_media_player_stop(m_vlcMediaPlayer);
    }
    if (m_vlcMedia) {
        libvlc_media_release(m_vlcMedia);
        m_vlcMedia = NULL;
    }
}

void NPlayer::onVlcMediaParsedChanged(const struct libvlc_event_t *p_event) {
    qint64 mediaLength = libvlc_media_player_get_length(m_vlcMediaPlayer);
    setRealDurationMs(static_cast<int>(mediaLength));
}


void NPlayer::onVlcMediaPlayerPlaying(const struct libvlc_event_t *p_event) {
    // TODO: implement
}
void NPlayer::onVlcMediaPlayerEndReached(const struct libvlc_event_t *p_event) {
    // TODO: implement
}

QString NPlayer::source() const
{
    return m_source;
}

void NPlayer::setSource(const QString &newSource)
{
    if (m_source == newSource)
        return;

    if (newSource.isEmpty()) {
        return;
    }

    releaseVlcPlayer();

    m_source = newSource;
    emit sourceChanged();

    setPosition(0);
    setRealDurationMs(0);
    createVlcPlayer();
}

int NPlayer::position() const
{
    return m_position;
}

void NPlayer::setPosition(int newPosition)
{
    if (m_position == newPosition)
        return;
    m_position = newPosition;
    emit positionChanged();
}

int NPlayer::durationMs() const
{
    return m_durationMs;
}

void NPlayer::setDurationMs(int newDurationMs)
{
    if (m_durationMs == newDurationMs)
        return;
    m_durationMs = newDurationMs;
    emit durationMsChanged();
}

int NPlayer::realDurationMs() const
{
    return m_realDurationMs;
}

void NPlayer::setRealDurationMs(int newRealDurationMs)
{
    if (m_realDurationMs == newRealDurationMs)
        return;
    m_realDurationMs = newRealDurationMs;
    emit realDurationMsChanged();
}
