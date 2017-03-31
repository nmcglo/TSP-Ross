/*
tsp_driver.c
Traveling Salesman Problem Solver
3-30-2017
Neil McGlohon
*/


//Includes
#include "tsp.h"


//--------------LIF Neuron stuff-------------

void lif_init (lif_neuron_state *s, tw_lp *lp)
{
     int self = lp -> gid;


     s->rng_count = 0;
}

void lif_prerun(lif_neuron_state *s, tw_lp *lp)
{
     int self = lp -> gid;

}

void lif_event_handler(lif_neuron_state *s, tw_bf *bf, neuron_mess *in_msg, tw_lp *lp)
{
     in_msg -> encodedState = *s;
     if(tw_now(lp) < simulation_length)
     {

     }
}

void lif_RC_event_handler(lif_neuron_state *s, tw_bf *bf, neuron_mess *in_msg, tw_lp *lp)
{
     int cur_rng_count = s->rng_count;
     int prev_rng_count = (in_msg->encodedState).rng_count;
     int total_rollbacks_needed = cur_rng_count - prev_rng_count;

     for(int i = 0; i<total_rollbacks_needed; i++)
     {
          tw_rand_reverse_unif(lp->rng);
     }
     s = &(in_msg->encodedState);
}

void lif_final(lif_neuron_state *s, tw_lp *lp)
{
     int self = lp->gid;

}
