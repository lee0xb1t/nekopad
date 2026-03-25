#ifndef NPLAYER_H
#define NPLAYER_H

#include <QObject>
#include <QQmlEngine>
#include <QTimer>

#include <vlc/vlc.h>

class NPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int durationMs READ durationMs WRITE setDurationMs NOTIFY durationMsChanged)
    Q_PROPERTY(int realDurationMs READ realDurationMs WRITE setRealDurationMs NOTIFY realDurationMsChanged)
    Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)

    QML_ELEMENT
public:
    explicit NPlayer(QObject *parent = nullptr);
    ~NPlayer();

    // Q_INVOKABLE void play();
    // Q_INVOKABLE void pause();
    // Q_INVOKABLE bool isPlaying();

    QString source() const;
    void setSource(const QString &newSource);

    int position() const;
    void setPosition(int newPosition);

    int durationMs() const;
    void setDurationMs(int newDurationMs);

    int realDurationMs() const;
    void setRealDurationMs(int newRealDurationMs);

signals:
    void sourceChanged();
    void positionChanged();

    void durationMsChanged();

    void realDurationMsChanged();

private:
    void createVlcPlayer();
    void releaseVlcPlayer();

    void onVlcMediaParsedChanged(const struct libvlc_event_t *p_event);
    void onVlcMediaPlayerPlaying(const struct libvlc_event_t *p_event);
    void onVlcMediaPlayerEndReached(const struct libvlc_event_t *p_event);

    //
    libvlc_instance_t *m_vlcInst;
    libvlc_media_t *m_vlcMedia;
    libvlc_media_player_t *m_vlcMediaPlayer;
    libvlc_event_manager_t *m_vlcEventManager;
    //
    QTimer m_progressTimer;

    QString m_source;
    int m_position;
    int m_durationMs;
    int m_realDurationMs;
};

#endif // NPLAYER_H
