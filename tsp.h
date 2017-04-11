/*
tsp.h
Traveling Salesman Solver
3-30-17
Neil McGlohon
*/


#ifndef _tsp_h
#define _tsp_h

#include "ross.h"
#include <stdbool.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

#define MAX_TOUR_LENGTH 20
#define MIN_CITY_SEPARATION 4
#define MAX_CITY_SEPARATION 11

#define MSG_TIME_DELAY 100



//STRUCTS ------------------------------


//TODO order the declarations to optimize memory usage
typedef struct
{
     int self_place;
     int self_city;
     int rng_count;
     int min_tour_weight;
     int num_neighbors;
     int* incomingWeights; //TODO for generalization, make into double
     tw_lpid* neighborIDs;
     int* min_tour;
} tsp_actor_state;


typedef struct
{
     tw_lpid sender;
     tw_lpid recipient;
     int tour_weight;
     tsp_actor_state encodedState;
     int tour_history[MAX_TOUR_LENGTH]; //TODO consider hash set implementation for optimization
} tsp_mess;


//MAPPING -----------------------------
extern tw_peid tsp_map(tw_lpid gid);
extern tw_lpid get_lp_gid(int city, int place);
extern int get_city_from_gid(tw_lpid gid);


//DRIVER STUFF -----------------------------
extern void tsp_init (tsp_actor_state *s, tw_lp *lp);
extern void tsp_prerun(tsp_actor_state *s, tw_lp *lp);
extern void tsp_event_handler(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_msg, tw_lp *lp);
extern void tsp_RC_event_handler(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_msg, tw_lp *lp);
extern void tsp_final(tsp_actor_state *s, tw_lp *lp);


//IO STUFF --------------------------------


//MAIN STUFF-----------------------------

extern int rand_range(int low, int high);


extern tw_lptype model_lps[];

tw_stime lookahead;
unsigned int nlp_per_pe;
unsigned int custom_LPs_per_pe;
int total_actors;
int total_cities;


//GLOBALS

int** weight_matrix;






#endif
