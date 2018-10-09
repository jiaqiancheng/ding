#include "vplayer.h"

vplayer::vplayer(QThread *parent):QThread(parent){
    this->streamindex=-1;
    this->filepath="shipin1.mp4";
    this->out_buffer_rgb=NULL;
    this->pFrame=NULL;
    this->pFrameRGB=NULL;
    this->packet=NULL;
    this->pFormatCtx=NULL;
    this->pCodecCtx=NULL;
    this->pCodec=NULL;
    this->img_convert_ctx=NULL;


}


vplayer::~vplayer(){

}


void vplayer::initFfmpeg(){
    av_register_all();
    avformat_network_init();
    pFormatCtx=avformat_alloc_context();
    if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
        QMessageBox::warning(NULL,"warning","Couldn't open input stream!",
                             QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        return ;
    }
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        QMessageBox::warning(NULL,"warning","Could not find stream information!",
                             QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        return ;
    }

    streamindex=-1;
    for(int i=0;i<pFormatCtx->nb_streams;i++){
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            streamindex=i;
            break;
        }
    }
    if(streamindex==-1){
        QMessageBox::warning(NULL,"warning","Didn't find a video stream\n",
                             QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        return ;
    }
    pCodecCtx=pFormatCtx->streams[streamindex]->codec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);

    if(pCodec==NULL){
        QMessageBox::warning(NULL,"warning","Codec not found!",
                             QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        return ;
    }
    if(avcodec_open2(pCodecCtx,pCodec,NULL)<0){
        QMessageBox::warning(NULL,"warning","Could not open codec!",
                             QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        return ;
    }

    pFrame=av_frame_alloc();
    pFrameRGB=av_frame_alloc();

    switch(pCodecCtx->pix_fmt){
    case AV_PIX_FMT_YUVJ420P:
        pixFormat=AV_PIX_FMT_YUV420P;
        break;
    case AV_PIX_FMT_YUVJ422P:
        pixFormat=AV_PIX_FMT_YUV422P;
        break;
    case AV_PIX_FMT_YUVJ444P:
        pixFormat=AV_PIX_FMT_YUV444P;
        break;
    case AV_PIX_FMT_YUVJ440P:
        pixFormat=AV_PIX_FMT_YUV440P;
        break;
    default:
        pixFormat=pCodecCtx->pix_fmt;
        break;
    }

    img_convert_ctx=sws_getContext(pCodecCtx->width,pCodecCtx->height,
                                   pixFormat,pCodecCtx->width,pCodecCtx->height,
                                   AV_PIX_FMT_RGB24,SWS_BICUBIC,NULL,NULL,NULL);
    out_buffer_rgb=(uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_RGB24,
                                                           pCodecCtx->width,pCodecCtx->height));
    avpicture_fill((AVPicture *)pFrameRGB,out_buffer_rgb,AV_PIX_FMT_RGB24,
                   pCodecCtx->width,pCodecCtx->height);
    packet=(AVPacket *)av_malloc(sizeof(AVPacket));

}


void vplayer::run(){
    int gotPicture=0;

    while(av_read_frame(pFormatCtx,packet)>=0){
        m_mutex.lock();
        if(packet->stream_index==streamindex){
            int ret=avcodec_decode_video2(pCodecCtx,pFrame,&gotPicture,packet);
            if(ret<0){
                QMessageBox::warning(NULL,"warning","decode error!",QMessageBox::Yes|QMessageBox::No,
                                     QMessageBox::Yes);
                break;
            }
        }
        if(gotPicture){
            sws_scale(img_convert_ctx,(const uint8_t * const *)pFrame->data,pFrame->linesize,
                      0,pCodecCtx->height,pFrameRGB->data,pFrameRGB->linesize);
            QImage tmpImg((uint8_t *)out_buffer_rgb,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB888);
            finalImage=tmpImg.convertToFormat(QImage::Format_RGB888,Qt::NoAlpha);
            emit sigGetOneFrame(finalImage);
            m_mutex.unlock();
            this->msleep(2);
        }
    }

    emit sigEndOfFrame();
     //videoStop();
}


void vplayer::videoPause(){
    this->m_mutex.lock();
}


void vplayer::videoResume(){
    this->m_mutex.unlock();
}


void vplayer::videoStop(){


   this->terminate();
   this->wait();
   uninit();


}


void vplayer::videoStart(){
    this->initFfmpeg();
    this->start();
}


void vplayer::uninit(){
    av_free_packet(packet);
    av_free(out_buffer_rgb);
    av_free(pFrame);
    av_free(pFrameRGB);
    sws_freeContext(img_convert_ctx);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
