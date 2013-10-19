/**
 *  @file sim_lib.h
 *
 */

/*  Created by John Uiterwyk on 9/22/13.
 *  Copyright (c) 2013 John Uiterwyk. All rights reserved.
 */

#ifndef _SIM_LIB_H
#define _SIM_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX_BUF_SIZE 255

#define SIM_OBJECT_STRING_SIZE 128

/**
 * State storage for each ball on the game map.
 */
typedef struct
{

  /** Unique identifier for the ball */
  int uid;
  /** Current position on the x axis */
  float x;
  /** Current position on the y axis */
  float y;
  /** Distance travelled per tick on the x axis */
  float dx;
  /** Distance travelled per tick on the y axis */
  float dy;
  /** Radius of the ball */
  int size;
}sim_object_t;


/**
 * State storage for the game world simulation.
 */
typedef struct
{
  /** Number of balls referenced in objects */
  int size;
  /** Width of the entire game map */
  int width;
  /** Height of the game map */
  int height;
  /** Tick counter */
  int tick;
  /** Number of clients connected to server.
   *
   * Used by clients to calculate how the map should be subdivided. */
  int clients;
  /** The objects (balls) that currently exist in the game world. */
  sim_object_t ** objects;
}sim_data_t;

/**
 * Inits a blank new simulation.
 *
 * sim_data_t members are set to defaults.
 */
sim_data_t * sim_new();

/**
 * Configures simulation once clients have connected and the game has
 * started.
 */
void sim_init(sim_data_t * sim_data,int clients,int num_of_objects, int width, int height);

/**
 * Creates a new object to simulate.
 *
 * Struct members are set to default values, so it is the responsibility
 * of the calling function to describe the object required.
 */
sim_object_t * sim_object_new();

/**
 * Update the map at each tick.
 *
 * Recalculates the positions of every object on the game map.
 */
void sim_tick(sim_data_t * sim_data);

/**
 * Frees the sim_data struct and all objects it contains.
 */
void sim_dispose(sim_data_t * sim_data);

/**
 * Collision detection for game objects.
 *
 * Currently checks to see if the object has reached the bounds of the
 * map, and adjusts the dx or dy values as appropriate to bounce the
 * object back.
 */
void sim_check_walls(sim_object_t * object, int width, int height);

/**
 * Random number generated between lower and upper bounds.
 */
float sim_random_float(float lower, float upper);

/**
 * Debug method to print object details to stdout.
 */
void sim_print_object(sim_object_t * object);

/**
 * Serializes the current game map state to a string, ready for
 * transmission over the network.
 */
void sim_serialize_state(sim_data_t * sim_data, char * string, int max_length);

/**
 * Updates the game state from a string (probably received by the
 * network lib). The string is de-serialized and the sim_data struct is
 * updated with the new information.
 */
void sim_deserialize_state(sim_data_t * sim_data, char * message);
#endif
