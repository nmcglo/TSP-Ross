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



//STRUCTS ------------------------------


typedef struct
{


} tsp_actor_state;


typedef struct
{
     tw_lpid sender;
     tw_lpid recipient;
     tsp_actor_state encodedState;
} tsp_mess;


//MAPPING -----------------------------



//DRIVER STUFF -----------------------------



//IO STUFF --------------------------------


//MAIN STUFF-----------------------------

extern tw_lptype model_lps[];

tw_stime lookahead;
unsigned int nlp_per_pe;
unsigned int custom_LPs_per_pe;






#endif
