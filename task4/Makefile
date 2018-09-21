.PHONY:clean
test.out:test.c
	gcc -g -o test.out test.c -I /home/tom/ffmpeg/host/include -L /home/tom/ffmpeg/host/lib -lavutil -lavformat -lavcodec -lswresample -lm -lrt -lpthread -lz

clean:
	-@rm test.out
