#ifndef globals_h
#define globals_h

#define FALSE 0
#define TRUE 1

#define MAX_TOUR_LENGTH 100
#define MIN_CITY_SEPARATION 4
#define MAX_CITY_SEPARATION 11

#define BITS_TO_ENCODE_TOUR ((MAX_TOUR_LENGTH*MAX_TOUR_LENGTH) + MAX_TOUR_LENGTH) //equivalent to number of max unique gids
#define MAX_INTS_NEEDED ((BITS_TO_ENCODE_TOUR / 64)+1)


#define MSG_TIME_DELAY 1000

typedef uint64_t compact_tour_part_t;


tw_stime lookahead;
unsigned int nlp_per_pe;
unsigned int custom_LPs_per_pe;
int total_actors;
int total_cities;
int L;

//GLOBALS

int** weight_matrix;


#endif
