/*
tsp_driver.c
Traveling Salesman Problem Solver
3-30-2017
Neil McGlohon
*/


//Includes
#include "tsp.h"


//--------------LIF Neuron stuff-------------

void tsp_init (tsp_actor_state *s, tw_lp *lp)
{
     int self = lp->gid;
     s->self_city = (lp->gid)%total_cities;
     s->self_place = (lp->gid)/total_cities;
     s->rng_count = 0;
     s->min_tour_weight = 0;

     s->incomingWeights = calloc(total_cities,sizeof(int));
     s->neighborIDs = calloc(total_cities,sizeof(tw_lpid));
     s->min_tour = calloc(total_cities+1,sizeof(tw_lpid));

     s->num_incoming_neighbors = 0;

     int me = s->self_city;
     if(lp->gid >= total_cities)
     {
          for(int j = 0; j < 4; j++)
          {
               int num_neighbors = s->num_incoming_neighbors;
               if(weight_matrix[me][j] > 0)
               {
                    s->incomingWeights[num_neighbors] = weight_matrix[me][j];
                    s->neighborIDs[num_neighbors] = j;
                    s->num_incoming_neighbors +=1;
               }
          }
     }


     printf("%d,%d: %d\n",s->self_city,s->self_place,s->num_incoming_neighbors);

     // if(self == 0)
     // {
     //      for(int i = 0; i < 4; i++)
     //      {
     //           for(int j = 0; j < 4; j++)
     //           {
     //                printf("%d ",weight_matrix[i][j]);
     //           }
     //           printf("\n");
     //      }
     // }
}

void tsp_prerun(tsp_actor_state *s, tw_lp *lp)
{
     int self = lp -> gid;

     if(self < total_cities) //only start on the first layer
     {
          double init_time = tw_rand_unif(lp->rng);

          tw_event *e = tw_event_new(self,init_time,lp);
          tsp_mess *mess = tw_event_data(e);
          mess->sender = self;
          mess->recipient = self;
          mess->tour_history = calloc(total_cities+1,sizeof(tw_lpid));
          tw_event_send(e);
     }
}

void tsp_event_handler(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_msg, tw_lp *lp)
{
     in_msg -> encodedState = *s;

     // printf("%d,%d: Received Event\n",s->self_city,s->self_place);

     int current_place = s->self_place;


}

void tsp_RC_event_handler(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_msg, tw_lp *lp)
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

void tsp_final(tsp_actor_state *s, tw_lp *lp)
{
     int self = lp->gid;

}
