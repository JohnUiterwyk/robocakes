#include <time.h>
#include <stdio.h>

void display(time_t time)
{
    printf("%d\n", (int)time); 
}

int main(void)
{
    int i = 1;
    time_t newTime, oldTime = time(NULL);
    double timeDiff = 0;

    while(i)
    {
        newTime = time(NULL);
        timeDiff = difftime(newTime, oldTime);
        
        if(timeDiff >= 1)
        {
            //printf("timeDiff: %f\n", timeDiff);
            display(newTime);
        }
        oldTime = newTime;
    }

    return 0;
}
