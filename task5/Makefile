.PHONY:clean

get_H264_File.out:get_H264_File.c
	gcc -g -o get_H264_File.out get_H264_File.c -I /home/ffmpeg/host/includ -L /home/ffmpeg/host/lib -lavformat -lavcodec -lavutil -lswscale

clean:
	rm *.out 
