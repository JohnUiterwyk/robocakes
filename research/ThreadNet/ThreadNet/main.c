//
//  main.c
//  ThreadNet
//
//  Created by John Uiterwyk on 9/27/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "TimeLoopLib.h"
typedef struct test
{
    char message[20];
    int count;
}Test_Data;

void TestFunction(void * data)
{
    Test_Data * testData = (Test_Data *)data;
    testData->count++;
    printf("%s %d\n",testData->message,testData->count);
}

int main(int argc, const char * argv[])
{

    Test_Data testData;
    TimeLoop_Data * timeLoopData;
    
    strcpy(testData.message,"Hello World!");
    testData.count= 0;
    
    timeLoopData = TimeLoop_Init();
    timeLoopData->interval = 1;
    
    TimeLoop_Start(timeLoopData, &TestFunction, &testData);
    
    
    return 0;
}

