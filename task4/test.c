#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "stdio.h"

int main(int argc,char* argv[])
{
        printf("going to av_register_all\n");
        av_register_all();
        return 0;
}
 
