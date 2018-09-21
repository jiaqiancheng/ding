#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};
#else
//Linux
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
};
#endif
#endif


int main(int argc, char *argv[]) {
	AVFormatContext *pFormatCtx;
	int                      i, streamindex;
	AVCodecContext  *pCodecCtx;
	AVCodec                  *pCodec;
	int ret, got_picture;
	AVPacket *packet;
	int y_size;
	uint8_t *dummy = NULL;
	int dummy_len;
	char nal_start[] = { 0,0,0,1 };

	char filepath[] = "shipin1.mp4";

	FILE *fp_264 = fopen("output.h264", "wb+");
	if (!fp_264) {
		printf("Could not open output file\n");
		return -1;
	}

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) {
		printf("Couldn't open input stream!\n");
		return -1;
	}
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		printf("Could not find stream information!\n");
		return -1;
	}

	streamindex = -1;

	for (i = 0; i < pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			streamindex = i;
			break;
		}
	}
	if (streamindex == -1) {
		printf("Didn't find a video stream\n");
		return -1;
	}

	pCodecCtx = pFormatCtx->streams[streamindex]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		printf("Codec not found.\n");
		return -1;
	}
	if (avcodec_open2(pCodecCtx,pCodec, NULL) < 0) {
		printf("Could not open codec.\n");
		return -1;
	}

	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	
	printf("-----------------------File Information-----------------\n");
	av_dump_format(pFormatCtx, 0, filepath, 0);
	printf("--------------------------------------------------------\n");


	AVBitStreamFilterContext *bsfc = av_bitstream_filter_init("h264_mp4toannexb");//注册filter
	//转换bitstream
	av_bitstream_filter_filter(bsfc, pCodecCtx, NULL, &dummy, &dummy_len,NULL,0,0);
	fwrite(pCodecCtx->extradata, pCodecCtx->extradata_size, 1, fp_264);
	av_bitstream_filter_close(bsfc);
	free(dummy);


	while (av_read_frame(pFormatCtx, packet) >= 0) {
		if (packet->stream_index == streamindex) {
			fwrite(nal_start, 4, 1, fp_264);
			fwrite(packet->data+4, 1, packet->size-4, fp_264);
		}
		av_free_packet(packet);
	}

	fclose(fp_264);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
	return 0;
}
