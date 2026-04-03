#ifndef NPLAYER_H
#define NPLAYER_H

#include <QObject>
#include <QQmlEngine>
#include <QTimer>

#include <vlc/vlc.h>

class NPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString bvid READ bvid WRITE setBvid NOTIFY bvidChanged FINAL)
    Q_PROPERTY(QString cookies READ cookies WRITE setCookies NOTIFY cookiesChanged FINAL)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int durationMs READ durationMs WRITE setDurationMs NOTIFY durationMsChanged)
    Q_PROPERTY(int realDurationMs READ realDurationMs WRITE setRealDurationMs NOTIFY realDurationMsChanged)
    Q_PROPERTY(float position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(int time READ time WRITE setTime NOTIFY timeChanged FINAL)
    Q_PROPERTY(int volumn READ volumn WRITE setVolumn NOTIFY volumnChanged FINAL)

    QML_ELEMENT
public:
    explicit NPlayer(QObject *parent = nullptr);
    ~NPlayer();

    Q_INVOKABLE void prepare();
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE bool isPlaying();
    Q_INVOKABLE void seek(int time);

    QString source() const;
    void setSource(const QString &newSource);

    int durationMs() const;
    void setDurationMs(int newDurationMs);

    int realDurationMs() const;
    void setRealDurationMs(int newRealDurationMs);

    QString cookies() const;
    void setCookies(const QString &newCookies);

    QString bvid() const;
    void setBvid(const QString &newBvid);

    float position() const;
    void setPosition(float newPosition);

    int time() const;
    void setTime(int newTime);

    int volumn() const;
    void setVolumn(int newVolumn);

signals:
    void mediaParsed();
    void videoStart();
    void videoEnd();
    void videoPause();
    void videoStopped();
    void videoUpdate(int time);

    //

    void sourceChanged();

    void durationMsChanged();

    void realDurationMsChanged();

    void cookiesChanged();

    void bvidChanged();

    void positionChanged();

    void timeChanged();

    void volumnChanged();

private:
    void createVlcPlayer();
    void releaseVlcPlayer();

    void onVlcMediaParsedChanged(const struct libvlc_event_t *p_event);
    void onVlcMediaPlayerEndReached(const struct libvlc_event_t *p_event);
    void onVlcMediaPlayerPlaying(const struct libvlc_event_t *p_event);
    void onVlcMediaPlayerPaused(const struct libvlc_event_t *p_event);
    void onVlcMediaPlayerStopped(const struct libvlc_event_t *p_event);
    void onVlcMediaTimeChanged(const struct libvlc_event_t *p_event);
    void onVlcMediaPositionChanged(const struct libvlc_event_t *p_event);

    //
    libvlc_instance_t *m_vlcInst = nullptr;
    libvlc_media_t *m_vlcMedia = nullptr;
    libvlc_media_player_t *m_vlcMediaPlayer = nullptr;
    libvlc_event_manager_t *m_vlcEventManager = nullptr;
    // QTimer m_progressTimer;
    //

    QString m_source;
    int m_durationMs;
    int m_realDurationMs;
    QString m_cookies;
    QString m_bvid;
    float m_position;
    int m_time;
    int m_volumn;
};

#endif // NPLAYER_H
