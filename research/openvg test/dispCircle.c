#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"

void display(VGfloat x, VGfloat y, VGfloat r)
{
    //printf("%d\n", (int)time);

	Fill(44,77,232,1);
	Circle(x, y, r);
}

int main(void)
{
    int i = 1, width, height;
    time_t newTime, oldTime = time(NULL);
    double timeDiff = 0;
	VGfloat x = 0, y = 0, r = 0;

	init(&width, &height);	//Start the ball
	x = (VGfloat)width/2;
	r =  (VGfloat)width/4;
    while(i)
    {
        newTime = time(NULL);
        timeDiff = difftime(newTime, oldTime);
        
        if(timeDiff >= .01)
        {
			Start(width, height);
			//printf("timeDiff: %f\n", timeDiff);
            Fill(44,77,232,1);
			Circle(X)
			display(x, y, r);
			End();
			x+=10;
			y+=10;
        }
        oldTime = newTime;
    }
	finish();
    return 0;
}
