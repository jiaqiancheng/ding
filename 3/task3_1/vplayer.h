#ifndef VPLAYER_H
#define VPLAYER_H

#include <QThread>
#include <QString>
#include <QImage>
#include <stdio.h>
#include <QLabel>
#include <QMessageBox>
#include <QMutex>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}

class vplayer : public QThread
{
    Q_OBJECT
public:
    explicit vplayer(QThread *parent = 0);
    ~vplayer();

    char* filepath;
    unsigned int streamindex;
    uint8_t* out_buffer_rgb;

    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pFrame,*pFrameRGB;
    AVPacket *packet;
    QImage finalImage;
    AVPixelFormat pixFormat;
    struct SwsContext *img_convert_ctx;

    void initFfmpeg();
    void run();
    void uninit();
    void startPlay();
    QMutex m_mutex;

public slots:
    void videoStart();
    void videoPause();
    void videoStop();
    void videoResume();
   // void videoPR();

signals:
    void sigGetOneFrame(QImage);
    void sigEndOfFrame();


};

#endif // VPLAYER_H
