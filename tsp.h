/*
tsp.h
Traveling Salesman Solver
3-30-17
Neil McGlohon
*/


#ifndef _tsp_h
#define _tsp_h

#include "ross.h"
#include "globals.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdint.h>




//STRUCTS ------------------------------


//TODO order the declarations to optimize memory usage
typedef struct
{
     uint64_t min_tour[MAX_INTS_NEEDED];
     int L;
     int* incomingWeights; //TODO for generalization, make into double
     tw_lpid* neighborIDs;
     int self_place;
     int self_city;
     int rng_count;
     int min_tour_weight;
     int num_neighbors;
     int msgs_sent;
     int msgs_rcvd;
     unsigned int complete_tour_msgs_rcvd;
} tsp_actor_state;


typedef struct
{
     uint64_t tour_history[MAX_INTS_NEEDED];
     int tour_weight;
     tw_lpid sender;
     int saved_rng_count;
     int saved_complete_tours;
     int saved_msgs_rcvd;
} tsp_mess;


//MAPPING -----------------------------
extern tw_peid tsp_map(tw_lpid gid);
extern tw_lpid get_lp_gid(int city, int place);
extern int get_city_from_gid(tw_lpid gid);
extern int get_place_from_gid(tw_lpid gid);

//TOUR STUFF ------------------------------
extern uint8_t get_bit(uint64_t bits, uint8_t pos);
extern uint8_t get_bit_rev(uint64_t bits, uint8_t pos);
extern uint64_t set_bit(uint64_t bits, uint8_t pos, uint8_t value);
extern uint8_t isInTour(int city, int endPlace, compact_tour_part_t *tour);
extern void addToTour(int city, int place, compact_tour_part_t *tour);
extern void decodeTour(compact_tour_part_t *encodedTour, int *decodedTour);
extern tw_lpid get_first_gid_in_tour(compact_tour_part_t *tour);


//DRIVER STUFF -----------------------------
extern void tsp_init (tsp_actor_state *s, tw_lp *lp);
extern void tsp_prerun(tsp_actor_state *s, tw_lp *lp);
extern void tsp_event_handler(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_msg, tw_lp *lp);
extern void tsp_RC_event_handler(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_msg, tw_lp *lp);
extern void tsp_commit(tsp_actor_state*s, tw_lp *lp);
extern void tsp_final(tsp_actor_state *s, tw_lp *lp);


//IO STUFF --------------------------------


//MAIN STUFF-----------------------------

extern int rand_range(int low, int high);


extern tw_lptype model_lps[];








#endif
