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
     s->min_tour_weight = 99999;
     s->complete_tour_msgs_rcvd = 0;

     s->msgs_sent = 0;
     s->incomingWeights = calloc(total_cities,sizeof(int));
     s->neighborIDs = calloc(total_cities,sizeof(tw_lpid));
     s->min_tour = calloc(total_cities+1,sizeof(int));

     for(int i = 0; i<total_cities+1;i++)
     {
          s->min_tour[i] = -1;
     }

     s->num_neighbors = 0;

     int me = s->self_city;

     for(int j = 0; j < total_cities; j++)
     {
          if(weight_matrix[me][j] > 0)
          {
               s->incomingWeights[s->num_neighbors] = weight_matrix[me][j];
               s->neighborIDs[s->num_neighbors] = j;
               s->num_neighbors +=1;
          }
     }

     // printf("%d,%d: %d\n",s->self_city,s->self_place,s->num_neighbors);

     if(self == 0)
     {
          for(int i = 0; i < total_cities; i++)
          {
               for(int j = 0; j < total_cities; j++)
               {
                    printf("%d ",weight_matrix[i][j]);
               }
               printf("\n");
          }
     }
}

void tsp_prerun(tsp_actor_state *s, tw_lp *lp)
{
     int self = lp->gid;

     if(self < total_cities) //only start on the first layer
     {
          double init_time = tw_rand_unif(lp->rng);

          tw_event *e = tw_event_new(self,init_time,lp);
          tsp_mess *mess = tw_event_data(e);
          mess->sender = self;
          mess->recipient = self;
          for(int i = 0; i < total_cities+1; i++)
          {
               mess->tour_history[i] = -1;
          }
          mess->tour_weight = 0;
          tw_event_send(e);
     }
}

int is_in_tour(int* tour, int len, int input)
{
     for(int i = 0; i<len; i++)
     {
          if(tour[i] == input)
          {
               return 1;
               break;
          }
     }

     return 0;
}


void tsp_event_handler(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_msg, tw_lp *lp)
{
     in_msg->encodedState = *s;

     // printf("%d,%d: Received Event\n",s->self_city,s->self_place);

     int incoming_city = get_city_from_gid(in_msg->sender);
     // printf("%d,%d:Incoming City: %d\n",s->self_city,s->self_place,incoming_city);

     int* working_tour = calloc(total_cities+1, sizeof(int));
     working_tour = in_msg->tour_history;
     working_tour[s->self_place] = s->self_city;

     // if(s->self_city == 0)
     // {
     //      printf("%d: ",s->self_city);
     //      for(int i = 0; i < total_cities+1; i++)
     //      {
     //           printf("%d ",working_tour[i]);
     //      }
     //      printf("\n");
     // }



     //get which index the weight is located
     int neighborIndex = 0;
     for(int i = 0; i < s->num_neighbors; i++)
     {
          if(s->neighborIDs[i] == incoming_city)
          {
               neighborIndex = i;
               break;
          }
     }

     int new_tour_weight;
     if(s->self_place > 0)
          new_tour_weight = in_msg->tour_weight + s->incomingWeights[neighborIndex];
     else
          new_tour_weight = 0;

     // printf("%d\n",new_tour_weight);

     if(s->self_place == total_cities)
     {
          if(new_tour_weight < (s->min_tour_weight))
          {
               s->min_tour_weight = new_tour_weight;
               s->min_tour = working_tour;
          }
          s->complete_tour_msgs_rcvd++;
     }

     //forward to the neighbors not currently in the tour
     if(s->self_place < total_cities)
     {
          for(int i = 0; i < s->num_neighbors; i++)
          {
               int neighbor_city_id = s->neighborIDs[i];

               int found = is_in_tour(working_tour,total_cities,neighbor_city_id);

               if(found == 0)
               {
                    s->msgs_sent++;
                    // for(int i = 0; i < total_cities+1; i++)
                    // {
                    //      printf("%d ",working_tour[i]);
                    // }
                    // printf(" sending to %d\n",neighbor_city_id);
                    // // printf("%d: sending to %d\n",s->self_city,neighbor_city_id);

                    tw_lpid recipient = get_lp_gid(neighbor_city_id,s->self_place+1);

                    tw_event *e = tw_event_new(recipient,tw_rand_unif(lp->rng)*MSG_TIME_DELAY,lp);
                    tsp_mess *mess = tw_event_data(e);
                    mess->sender = lp->gid;
                    mess->recipient = recipient;
                    mess->tour_weight = new_tour_weight;

                    for(int j = 0; j < total_cities+1;j++)
                    {
                         mess->tour_history[j] = working_tour[j];
                    }
                    tw_event_send(e);
               }
          }

          if(s->self_place == total_cities-1) //then you need to send to the original city
          {
               tw_lpid recipient = get_lp_gid(working_tour[0],s->self_place+1);
               s->msgs_sent++;


               // printf("%d: ",s->self_city);
               // for(int i = 0; i < total_cities+1; i++)
               // {
               //      printf("%d ",working_tour[i]);
               // }
               // printf(" sending to original %d\n",working_tour[0]);

               tw_event *e = tw_event_new(recipient,tw_rand_unif(lp->rng)*MSG_TIME_DELAY,lp);
               tsp_mess *mess = tw_event_data(e);
               mess->sender = lp->gid;
               mess->recipient = recipient;
               mess->tour_weight = new_tour_weight;

               for(int j = 0; j < total_cities+1;j++)
               {
                    mess->tour_history[j] = working_tour[j];
               }
               tw_event_send(e);
          }
     }

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

void tsp_commit(tsp_actor_state *s, tw_lp *lp)
{
     // printf("%d: Sent Messages: %i\n",s->self_city, s->msgs_sent);
}

void tsp_final(tsp_actor_state *s, tw_lp *lp)
{
     int self = lp->gid;
     if(s->self_place == total_cities)
     {
          printf("%d: Min Tour Weight: %d\n",s->self_city,s->min_tour_weight);

          for(int i = 0; i < total_cities+1; i++)
          {
               printf("%d ",s->min_tour[i]);
          }
          printf("\n");

          printf("%d: Complete Tours: %d\n",s->self_city,s->complete_tour_msgs_rcvd);

     }


}
