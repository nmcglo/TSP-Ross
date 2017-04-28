#ifndef globals_h
#define globals_h

#define FALSE 0
#define TRUE 1

#define MAX_TOUR_LENGTH 21
#define MIN_CITY_SEPARATION 1
#define MAX_CITY_SEPARATION 100

#define BITS_TO_ENCODE_TOUR ((MAX_TOUR_LENGTH*MAX_TOUR_LENGTH) + MAX_TOUR_LENGTH) //equivalent to number of max unique gids
#define MAX_INTS_NEEDED ((BITS_TO_ENCODE_TOUR / 64)+1)




typedef uint64_t compact_tour_part_t;

double jitter;
double weight_multiplier;

tw_stime lookahead;
unsigned int nlp_per_pe;
unsigned int custom_LPs_per_pe;
int total_actors;
int total_cities;

//GLOBALS

int** weight_matrix;


#endif
