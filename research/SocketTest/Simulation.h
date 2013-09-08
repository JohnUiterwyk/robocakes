//
//  simulation.h
//  SocketTest
//
//  Created by John Uiterwyk on 8/19/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#ifndef SocketTest_simulation_h
#define SocketTest_simulation_h

typedef enum{
    CIRCLE
}shapes;

typedef struct
{
    float x;
    float y;
    float z;
}vector2d;

typedef struct
{
    vector2d position;
    vector2d velocity;
    int alive;
    
}game_object;


typedef struct
{
    game_object ** objects;
    int size;
}simulation;

simulation * new_simulation();

#endif
