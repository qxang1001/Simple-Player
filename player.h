#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QSocketNotifier>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QUrl>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

Q_DECLARE_METATYPE(GstMessage*)

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QWidget *videoWidget,QObject *parent = nullptr);
    ~Player();
    qint64 get_duration();

    void play();
    void pause();
    void setUri(const QString &uri);

signals:
    void stateChanged(const QString &state);
    void error(const QString &message);

    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);

public slots:
    void onBusMessage(GstMessage *message);

    void onSliderPressed();
    void onSliderReleased(int value);
    void setPlaybackRate(double rate);

    void setVolume(int level);

private slots:
    void updateProgress();

private:
    QWidget *m_videoWidget;
    QTimer *m_timer;
    gint64 m_duration;
    GstState current_state;
    double m_rate;

    GstElement *m_pipeline;
    GstElement *m_source;
    GstElement *m_decodebin;
    GstElement *m_videoconvert;
    GstElement *m_audioconvert;
    GstElement *m_videosink;
    GstElement *m_audiosink;
    GstElement *m_volume;
};



#endif // PLAYER_H
