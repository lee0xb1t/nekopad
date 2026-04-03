#include "nplayer.h"
#include <QDebug>

NPlayer::NPlayer(QObject *parent)
    : QObject{parent}
{
    const char* args[] = {
        "--verbose=2",
        "--no-video",
        "--aout=directsound",
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

    libvlc_event_attach(m_vlcEventManager, libvlc_MediaPlayerPaused, [](const struct libvlc_event_t *p_event, void *p_data) {
        NPlayer *p = static_cast<NPlayer *>(p_data);
        p->onVlcMediaPlayerPaused(p_event);
    }, this);

    libvlc_event_attach(m_vlcEventManager, libvlc_MediaPlayerPositionChanged, [](const struct libvlc_event_t *p_event, void *p_data) {
        NPlayer *p = static_cast<NPlayer *>(p_data);
        p->onVlcMediaPositionChanged(p_event);
    }, this);

    libvlc_event_attach(m_vlcEventManager, libvlc_MediaPlayerTimeChanged, [](const struct libvlc_event_t *p_event, void *p_data) {
        NPlayer *p = static_cast<NPlayer *>(p_data);
        p->onVlcMediaTimeChanged(p_event);
    }, this);

    libvlc_event_attach(m_vlcEventManager, libvlc_MediaPlayerStopped, [](const struct libvlc_event_t *p_event, void *p_data) {
        NPlayer *p = static_cast<NPlayer *>(p_data);
        p->onVlcMediaPlayerStopped(p_event);
    }, this);


    // TODO: implement timer
    // m_progressTimer.setInterval(400);
    // connect(&m_progressTimer, &QTimer::timeout, this, &NPlayer::onTimerUpdate);
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
    QString urlStr = source();
    qDebug() << "NPlayer: Trying to play URL:" << urlStr;

    if (urlStr.isEmpty() || !urlStr.startsWith("http")) {
        qWarning() << "NPlayer: Invalid URL! Aborting playback.";
        return;
    }

    m_vlcMedia = libvlc_media_new_location(m_vlcInst, source().toStdString().c_str());

    qDebug() << "--------------------------------------------";
    QStringList options;
    options << QString(":http-referrer=https://www.bilibili.com/video/%1").arg(bvid());
    options << ":http-user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36";
    options << ":network-caching=3000";
    options << ":avcodec-hw=any";

    // QString cookieStr = cookies();
    // if (!cookieStr.isEmpty()) {
    //     options << QString(":http-cookies=%1").arg(cookieStr);
    // }
    for (QString &opt : options) {
        std::string optstr = opt.toStdString();
        libvlc_media_add_option(m_vlcMedia, optstr.c_str());
        qDebug() << "VLC Option Added:" << optstr;
    }
    qDebug() << "--------------------------------------------";

    libvlc_audio_set_volume(m_vlcMediaPlayer, volumn());

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
    emit mediaParsed();
}


void NPlayer::onVlcMediaPlayerPlaying(const struct libvlc_event_t *p_event) {
    qint64 mediaLength = libvlc_media_player_get_length(m_vlcMediaPlayer);
    setRealDurationMs(static_cast<int>(mediaLength));

    emit videoStart();
}
void NPlayer::onVlcMediaPlayerEndReached(const struct libvlc_event_t *p_event) {
    emit videoEnd();
}


void NPlayer::onVlcMediaPlayerPaused(const struct libvlc_event_t *p_event) {
    emit videoPause();
}


void NPlayer::onVlcMediaPlayerStopped(const struct libvlc_event_t *p_event) {
    emit videoStopped();
}

void NPlayer::onVlcMediaTimeChanged(const struct libvlc_event_t *p_event) {
    int time = p_event->u.media_player_time_changed.new_time;
    setTime(time);

    emit videoUpdate(time);
}


void NPlayer::onVlcMediaPositionChanged(const struct libvlc_event_t *p_event) {
    float position = p_event->u.media_player_position_changed.new_position;
    setPosition(position);

    if (!isPlaying()) {
        play();
    }
}

QString NPlayer::source() const
{
    return m_source;
}

void NPlayer::setSource(const QString &newSource)
{
    if (m_source == newSource)
        return;
    m_source = newSource;
    emit sourceChanged();
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

QString NPlayer::cookies() const
{
    return m_cookies;
}

void NPlayer::setCookies(const QString &newCookies)
{
    if (m_cookies == newCookies)
        return;
    m_cookies = newCookies;
    emit cookiesChanged();
}

void NPlayer::prepare() {
    releaseVlcPlayer();

    setPosition(0);
    setRealDurationMs(0);
    createVlcPlayer();

    qDebug() << this->source();
}

void NPlayer::play() {
    libvlc_media_player_play(m_vlcMediaPlayer);
}

void NPlayer::pause() {
    libvlc_media_player_pause(m_vlcMediaPlayer);
}

bool NPlayer::isPlaying() {
    return static_cast<bool>(libvlc_media_player_is_playing(m_vlcMediaPlayer));
}

void NPlayer::seek(int time) {
    libvlc_media_player_set_time(m_vlcMediaPlayer, time);
}

QString NPlayer::bvid() const
{
    return m_bvid;
}

void NPlayer::setBvid(const QString &newBvid)
{
    if (m_bvid == newBvid)
        return;
    m_bvid = newBvid;
    emit bvidChanged();
}

float NPlayer::position() const
{
    return m_position;
}

void NPlayer::setPosition(float newPosition)
{
    if (qFuzzyCompare(m_position, newPosition))
        return;
    m_position = newPosition;
    emit positionChanged();
}

int NPlayer::time() const
{
    return m_time;
}

void NPlayer::setTime(int newTime)
{
    if (m_time == newTime)
        return;
    m_time = newTime;
    emit timeChanged();
}

int NPlayer::volumn() const
{
    return m_volumn;
}

void NPlayer::setVolumn(int newVolumn)
{
    if (m_volumn == newVolumn)
        return;
    m_volumn = newVolumn;
    if (m_vlcMediaPlayer) {
        libvlc_audio_set_volume(m_vlcMediaPlayer, volumn());
    }
    emit volumnChanged();
}
