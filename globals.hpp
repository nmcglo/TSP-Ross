#ifndef globals_h
#define globals_h

#include "ross.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdint.h>


#define FALSE 0
#define TRUE 1

#define MAX_TOUR_LENGTH 21
#define MIN_CITY_SEPARATION 1
#define MAX_CITY_SEPARATION 100

#define BITS_TO_ENCODE_TOUR ((MAX_TOUR_LENGTH*MAX_TOUR_LENGTH) + MAX_TOUR_LENGTH) //equivalent to number of max unique gids
#define MAX_INTS_NEEDED ((BITS_TO_ENCODE_TOUR / 64)+1)


typedef uint64_t compact_tour_part_t;

extern double jitter;
extern double weight_multiplier;

extern tw_stime lookahead;
extern unsigned int nlp_per_pe;
extern unsigned int custom_LPs_per_pe;
extern int total_actors;
extern int total_cities;

//GLOBALS

extern int** weight_matrix;


#endif
