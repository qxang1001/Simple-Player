#include "player.h"

// C-style callback for GStreamer bus signals (e.g., "message")
static void on_bus_message(GstBus *bus, GstMessage *message, gpointer user_data) {
    Q_UNUSED(bus)

    gst_message_ref(message);
    Player *player = static_cast<Player*>(user_data);
    QMetaObject::invokeMethod(player, "onBusMessage", Qt::QueuedConnection,
                                     Q_ARG(GstMessage*, message));

}

Player::Player(QWidget *videoWidget,QObject *parent)
    : QObject(parent), m_videoWidget(videoWidget), m_pipeline(nullptr)
{
    gst_init(nullptr, nullptr);

    m_pipeline = gst_pipeline_new("player-pipeline");
    m_source = gst_element_factory_make("uridecodebin", "source");
    m_videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    m_audioconvert = gst_element_factory_make("audioconvert", "audioconvert");
    m_videosink = gst_element_factory_make("glimagesink", "videosink");
    m_audiosink = gst_element_factory_make("autoaudiosink", "audiosink");
    m_volume = gst_element_factory_make("volume", "volume");

    if (!m_pipeline || !m_source || !m_videoconvert || !m_videosink || !m_audioconvert || !m_audiosink || !m_volume) {
        qCritical() << "Failed to create one of the GStreamer elements.";
        emit error("Failed to create GStreamer elements.");
        return;
    }

    GstVideoOverlay *overlay = GST_VIDEO_OVERLAY(m_videosink);
    if (overlay) {
        gst_video_overlay_set_window_handle(overlay, m_videoWidget->winId());
        qDebug() << "GStreamer window handle set to:" << m_videoWidget;
    }

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Player::updateProgress);

    m_duration = 0;
    //current_state = GST_STATE_READY;
    m_rate = 1.0;

    gst_bin_add_many(GST_BIN(m_pipeline), m_source, m_videoconvert, m_audioconvert, m_audiosink, m_volume,m_videosink,  NULL);

    if (!gst_element_link(m_videoconvert, m_videosink) || !gst_element_link(m_audioconvert, m_volume)||!gst_element_link(m_volume, m_audiosink)) {
        qCritical() << "Failed to link static elements.";
        emit error("Failed to link GStreamer elements.");
        return;
    }

    g_signal_connect(m_source, "pad-added", G_CALLBACK(+[](GstElement *element, GstPad *pad, gpointer user_data) {
        Player *player = static_cast<Player*>(user_data);
        GstCaps *caps = gst_pad_get_current_caps(pad);
        GstStructure *str = gst_caps_get_structure(caps, 0);
        const gchar *name = gst_structure_get_name(str);

        GstElement *sink = nullptr;
        if (g_str_has_prefix(name, "video/x-raw")) {
            sink = player->m_videoconvert;
        } else if (g_str_has_prefix(name, "audio/x-raw")) {
            sink = player->m_audioconvert;
        }

        if (sink) {
            GstPad *sinkpad = gst_element_get_static_pad(sink, "sink");
            if (gst_pad_is_linked(sinkpad)) {
                gst_object_unref(sinkpad);
                return;
            }
            GstPadLinkReturn ret = gst_pad_link(pad, sinkpad);
            if (ret != GST_PAD_LINK_OK) {
                qWarning() << "Pad link failed:" << ret;
            } else {
                qDebug() << "Successfully linked" << name << "pad.";
            }
            gst_object_unref(sinkpad);
        }
        gst_caps_unref(caps);
        }), this);

    GstBus *bus = gst_element_get_bus(m_pipeline);

    gst_bus_add_signal_watch(bus);
    g_signal_connect(bus, "message", G_CALLBACK(on_bus_message), this);
    gst_object_unref(bus);
}

Player::~Player()
{
    if (m_pipeline) {
        gst_element_set_state(m_pipeline, GST_STATE_NULL);
        gst_object_unref(m_pipeline);
        m_pipeline = nullptr;
    }
}

qint64 Player::get_duration()
{
    return static_cast<qint64>(m_duration) / GST_MSECOND;
}

void Player::updateProgress()
{
    if (!m_pipeline) return;
    gint64 position = 0;
    if (gst_element_query_position(m_pipeline, GST_FORMAT_TIME, &position)) {
        emit positionChanged(static_cast<qint64>(position) / GST_MSECOND); // 转换为毫秒
    }
    if (m_duration == 0) {
        if (gst_element_query_duration(m_pipeline, GST_FORMAT_TIME, &m_duration)) {
            emit durationChanged(static_cast<qint64>(m_duration) / GST_MSECOND); // 转换为毫秒
        }
    }
}

void Player::setUri(const QString &filePath)
{
    if (!m_source) {
        qWarning() << "Source element not available.";
        return;
    }

    QString uri;
    QUrl url(filePath);

    if (url.isValid() && !url.scheme().isEmpty()) {
        uri = url.toString();
    } else {
        uri = QUrl::fromLocalFile(filePath).toString();
    }

    g_object_set(m_source, "uri", uri.toUtf8().constData(), NULL);
}

void Player::play()
{
    if (m_pipeline) {
        gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
        m_timer->start(200);
        //current_state = GST_STATE_PLAYING;
    }
}

void Player::pause()
{
    if (m_pipeline) {
        gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
        m_timer->stop();
        //current_state = GST_STATE_PAUSED;
    }
}

void Player::onSliderPressed()
{
    //gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
    m_timer->stop();
}

void Player::setVolume(int level)
{
    if (!m_volume) {
        qWarning() << "Volume element is not available.";
        return;
    }
    // 将滑块的 0-100 范围转换为 GStreamer volume 元素的 0.0-1.0 范围
    double volume_level = static_cast<double>(level) / 100.0;
    g_object_set(m_volume, "volume", volume_level, NULL);
    //qDebug() << "Volume set to:" << volume_level;
}

void Player::setPlaybackRate(double rate)
{
    if (!m_pipeline) {
        qWarning() << "Pipeline is not available, cannot set rate.";
        return;
    }

    gint64 current_pos;
    if (!gst_element_query_position(m_pipeline, GST_FORMAT_TIME, &current_pos)) {
        qWarning() << "Failed to query current position for rate change.";
        return;
    }

    GstSeekFlags flags = static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE);

    if (!gst_element_seek(m_pipeline,
                          rate,
                          GST_FORMAT_TIME,
                          flags,
                          GST_SEEK_TYPE_SET, current_pos,
                          GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE)) {
        qWarning() << "Failed to set playback rate to" << rate;
    }
    else {
        m_rate = rate;
        qDebug() << "Playback rate set to" << rate;
    }
}

void Player::onSliderReleased(int value)
{
    if (!m_pipeline) return;
    double x = (double)value/100.0;
    gint64 target_ns = static_cast<gint64>(x*this->m_duration);
    //qDebug() << "value is:" << value;
    //qDebug() << "target_ns is:" << target_ns;

    //GstSeekFlags flags = static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT);
    GstSeekFlags flags = static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE);
    if (!gst_element_seek(m_pipeline, m_rate,GST_FORMAT_TIME,
                                 flags,
                                 GST_SEEK_TYPE_SET,target_ns,
                          GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE)) {
        qWarning() << "Seek failed!";
    }
    //gst_element_set_state(m_pipeline, current_state);

    m_timer->start(200);
}

void Player::onBusMessage(GstMessage *message)
{
    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_STATE_CHANGED: {
        if (GST_MESSAGE_SRC(message) == GST_OBJECT(m_pipeline)) {
            GstState old_state, new_state, pending_state;
            gst_message_parse_state_changed(message, &old_state, &new_state, &pending_state);
            QString state_str = gst_element_state_get_name(new_state);
            qDebug() << "State changed to:" << state_str;
            emit stateChanged(state_str);
        }
        break;
    }
    case GST_MESSAGE_ERROR: {
        GError *err = nullptr;
        gchar *debug_info = nullptr;
        gst_message_parse_error(message, &err, &debug_info);
        QString error_message = QString("GStreamer Error: %1").arg(err->message);
        qCritical() << error_message;
        qCritical() << "Debug info:" << debug_info;
        emit error(error_message);
        g_clear_error(&err);
        g_free(debug_info);
        break;
    }
    case GST_MESSAGE_EOS:{
        qDebug() << "End-Of-Stream reached.";
        break;
    }
    default:
        break;
    }
    gst_message_unref(message);
}

