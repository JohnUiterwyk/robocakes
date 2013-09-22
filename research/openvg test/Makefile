all: dispCircle dispTime

dispCircle: dispCircle.c
	gcc -Wall -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/home/pi/openvg -o dispCircle dispCircle.c /home/pi/openvg/libshapes.o /home/pi/openvg/oglinit.o -L/opt/vc/lib -lGLESv2 -ljpeg

dispTime:
	gcc -Wall dispTime.c

# /home/pi/openvg/libshapes.o /home/pi/openvg/oglinit.o
