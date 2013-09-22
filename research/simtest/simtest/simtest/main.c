//
//  main.c
//  simtest
//
//  Created by John Uiterwyk on 9/22/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#include <stdio.h>
#include "SimLib.h"

int main(int argc, const char * argv[])
{
    int numOfObjects,i;
    Sim_Data * simData;
    
    numOfObjects = 10;
    
    simData = Sim_New(numOfObjects, 1000, 1000);
    
    for(i = 0; i< 1000;i++)
    {
        Sim_Tick(simData);
        Sim_PrintObject(simData->objects[1]);
        
    }
    
    
    
    
    return 0;
}

