/*
tsp_driver.c
Traveling Salesman Problem Solver
3-30-2017
Neil McGlohon
*/


//Includes
#include "tsp.h"
#include "tsp_tour.h"




void copy_uint64_array(uint64_t* src, uint64_t* dest, int len)
{
     for(int i = 0; i < len; i++)
     {
          dest[i] = src[i];
     }
}

void print_tour(compact_tour_part_t * tour)
{
     int actualTour[total_cities+1];
     for(int i = 0; i < total_cities+1; i++)
     {
          actualTour[i] = -1;
     }

     decodeTour(tour,actualTour);

     for(int i = 0; i < total_cities+1; i++)
     {
          printf("%d ",actualTour[i]);
     }
}

//--------------LIF Neuron stuff-------------

void tsp_init (tsp_actor_state *s, tw_lp *lp)
{
     int self = lp->gid;
     s->self_city = (lp->gid)%total_cities;
     s->self_place = (lp->gid)/total_cities;
     s->rng_count = 0;
     s->min_tour_weight = 99999;
     s->min_complete_tour_weight = 99999;
     s->complete_tour_msgs_rcvd = 0;

     s->msgs_sent = 0;
     s->msgs_rcvd = 0;
     s->incomingWeights = calloc(total_cities,sizeof(int));
     s->incomingNeighborIDs = calloc(total_cities,sizeof(tw_lpid));

     s->outgoingWeights = calloc(total_cities,sizeof(int));
     s->outgoingNeighborIDs = calloc(total_cities,sizeof(tw_lpid));

     for(int i = 0; i<MAX_INTS_NEEDED;i++)
     {
          s->min_tour[i] = 0;
          s->min_complete_tour[i] = 0;
     }


     s->num_incoming_neighbors = 0;

     int me = s->self_city;

     for(int j = 0; j < total_cities; j++)
     {
          if(weight_matrix[me][j] > 0)
          {
               s->incomingWeights[s->num_incoming_neighbors] = weight_matrix[me][j];
               s->incomingNeighborIDs[s->num_incoming_neighbors] = j;
               s->num_incoming_neighbors +=1;
          }
     }

     for(int j = 0; j < total_cities; j++)
     {
          if(weight_matrix[j][me] > 0)
          {
               s->outgoingWeights[s->num_outgoing_neighbors] = weight_matrix[j][me];
               s->outgoingNeighborIDs[s->num_outgoing_neighbors] = j;
               s->num_outgoing_neighbors += 1;
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
          // mess->recipient = self;
          for(int i = 0; i<MAX_INTS_NEEDED;i++)
          {
               mess->tour_history[i] = 0;
          }

          mess->tour_weight = 0;
          mess->messType = TOUR;
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


void tsp_propogate_message(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_mst, tw_lp *lp, compact_tour_part_t working_tour[MAX_INTS_NEEDED], int new_tour_weight)
{
     // tw_stime now = tw_now(lp);

     int self_place = s->self_place;

     int self_city = s->self_city;

     // tw_stime endTime = g_tw_ts_end - (.1*g_tw_ts_end);

     // tw_stime myDelayWindowStart = ((self_place+1.0)/(total_cities+1.0))*endTime;
     // tw_stime myUpperBoundDelay = ((self_city+1.0)/(total_cities)) * (1.0/(total_cities+1.0)) * endTime;

     // printf("%i,%i: Delay = %f * %f\n",self_city,self_place,myUpperBoundDelay, tw_rand_unif(lp->rng));



     if(s->self_place < total_cities)
     {
          if(s->self_place == total_cities-1) //then you need to send to the original city
          {
               tw_lpid first_gid = get_first_gid_in_tour(working_tour);
               int city = get_city_from_gid(first_gid);

               int weight_to_original = weight_matrix[city][s->self_city];


               tw_lpid recipient = get_lp_gid( city ,s->self_place+1);
               s->msgs_sent++;
               s->rng_count++;

               // printf("%d: ",s->self_city);
               // for(int i = 0; i < total_cities+1; i++)
               // {
               //      printf("%d ",working_tour[i]);
               // }
               // printf(" sending to original %d\n",city);

               tw_stime delay = weight_to_original*100 + tw_rand_unif(lp->rng)*.00001;


               tw_event *e = tw_event_new(recipient,delay,lp);
               tsp_mess *mess = tw_event_data(e);
               mess->sender = lp->gid;
               // mess->recipient = recipient;
               mess->tour_weight = new_tour_weight;
               mess->messType = TOUR;

               copy_uint64_array(working_tour,mess->tour_history,MAX_INTS_NEEDED);

               tw_event_send(e);

          }
          else
          {
               for(int i = 0; i < s->num_incoming_neighbors; i++)
               {
                    int neighbor_city_id = s->incomingNeighborIDs[i];


                    int found = isInTour(neighbor_city_id,s->self_place,working_tour);

                    if(found == 0)
                    {
                         s->msgs_sent++;
                         s->rng_count++;

                         // for(int i = 0; i < total_cities+1; i++)
                         // {
                         //      printf("%d ",working_tour[i]);
                         // }
                         // printf("%i: sending to %d\n",s->self_city,neighbor_city_id);
                         // // printf("%d: sending to %d\n",s->self_city,neighbor_city_id);

                         tw_lpid recipient = get_lp_gid(neighbor_city_id,s->self_place+1);

                         // tw_stime timeToWindow = myDelayWindowStart - now;

                         // tw_stime delay = (tw_rand_unif(lp->rng)*myUpperBoundDelay) + timeToWindow;

                         tw_stime delay = weight_matrix[neighbor_city_id][s->self_city] + tw_rand_unif(lp->rng)*.00001;


                         // printf("%i,%i: send message to arrive at: %f\n",self_city,self_place,now+delay);

                         tw_event *e = tw_event_new(recipient,delay,lp);
                         tsp_mess *mess = tw_event_data(e);
                         mess->sender = lp->gid;
                         mess->messType = TOUR;
                         // mess->recipient = recipient;
                         mess->tour_weight = new_tour_weight;

                         copy_uint64_array(working_tour,mess->tour_history,MAX_INTS_NEEDED);

                         // for(int j = 0; j < total_cities+1;j++)
                         // {
                         //      mess->tour_history[j] = working_tour[j];
                         // }
                         tw_event_send(e);
                    }
               }
          }


     }
}

void tsp_broadcast_complete(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_msg, tw_lp *lp)
{
     for(int i = 0; i < total_cities; i++)
     {
          for(int j = 0; j < total_cities+1; j++)
          {
               tw_lpid recipient = get_lp_gid(i ,j);
               tw_event *e = tw_event_new(recipient,tw_rand_unif(lp->rng)*.00001,lp);
               tsp_mess *mess = tw_event_data(e);
               mess->sender = lp->gid;
               mess->messType = COMPLETE;
               // mess->recipient = recipient;
               copy_uint64_array(s->min_complete_tour,in_msg->tour_history,MAX_INTS_NEEDED);
               mess->tour_weight = s->min_complete_tour_weight;
               tw_event_send(e);
          }
     }
}

void tsp_event_handler(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_msg, tw_lp *lp)
{
     in_msg->saved_rng_count = s->rng_count;
     in_msg->saved_complete_tours = s->complete_tour_msgs_rcvd;
     in_msg->saved_msgs_rcvd = s->msgs_rcvd;


     switch(in_msg->messType)
     {
          case TOUR: //add and propogate the tour message
          {
               s->msgs_rcvd++;

               // printf("%d,%d: Received TOUR mess\n",s->self_city,s->self_place);

               int incoming_city = get_city_from_gid(in_msg->sender);
               // printf("%d,%d:Incoming City: %d\n",s->self_city,s->self_place,incoming_city);

               // compact_tour_part_t  * working_tour = calloc(MAX_INTS_NEEDED,sizeof(compact_tour_part_t));
               compact_tour_part_t working_tour[MAX_INTS_NEEDED];

               copy_uint64_array(in_msg->tour_history,working_tour,MAX_INTS_NEEDED);

               addToTour(s->self_city,s->self_place,working_tour);

               int neighborIndex = 0;
               for(int i = 0; i < s->num_incoming_neighbors; i++)
               {
                    if(s->incomingNeighborIDs[i] == incoming_city)
                    {
                         neighborIndex = i;
                         break;
                    }
               }

               int new_tour_weight;
               if(s->self_place > 0) //not the first city in tour
                    new_tour_weight = in_msg->tour_weight + s->incomingWeights[neighborIndex];
               else
                    new_tour_weight = 0; //the first city in the tour doesn't have an incoming weight

               if((new_tour_weight < s->min_complete_tour_weight))
               {
                         if(s->self_place == total_cities) //you're the last city in the tour
                         {
                              if(new_tour_weight < (s->min_tour_weight))
                              {
                                   s->min_tour_weight = new_tour_weight;
                                   copy_uint64_array(working_tour,s->min_complete_tour,MAX_INTS_NEEDED);
                                   tsp_broadcast_complete(s,bf,in_msg,lp);
                              }
                              s->complete_tour_msgs_rcvd++;

                         }

                         //forward to the neighbors not currently in the tour
                         tsp_propogate_message(s, bf, in_msg, lp, working_tour, new_tour_weight);
               }

          }
          case COMPLETE: //you're receiving a complete tour message, stop propogating weaker tours
          {
               // printf("%d,%d: Received COMPLETE mess\n",s->self_city,s->self_place);



               if(in_msg->tour_weight <= s->min_complete_tour_weight)
               {
                    s->min_complete_tour_weight = in_msg->tour_weight;

                    compact_tour_part_t working_tour[MAX_INTS_NEEDED];

                    copy_uint64_array(in_msg->tour_history,s->min_complete_tour,MAX_INTS_NEEDED);
               }
          }
     }





     // free(working_tour);
}

void tsp_RC_event_handler(tsp_actor_state *s, tw_bf *bf, tsp_mess *in_msg, tw_lp *lp)
{
     int cur_rng_count = s->rng_count;
     int prev_rng_count = in_msg->saved_rng_count;
     int total_rollbacks_needed = cur_rng_count - prev_rng_count;

     for(int i = 0; i<total_rollbacks_needed; i++)
     {
          tw_rand_reverse_unif(lp->rng);
     }
     s->rng_count = in_msg->saved_rng_count;
     s->complete_tour_msgs_rcvd = in_msg->saved_complete_tours;
     s->msgs_rcvd = in_msg->saved_msgs_rcvd;
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
          if(s->complete_tour_msgs_rcvd > 0)
          {
               int actualTour[total_cities+1];
               decodeTour(s->min_complete_tour,actualTour);

               printf("%d: Min Tour Weight: %d\n",s->self_city,s->min_complete_tour_weight);

               for(int i = 0; i < total_cities+1; i++)
               {
                    printf("%d ",actualTour[i]);
               }
               printf("\n");

               printf("%d: Complete Tours: %d\n",s->self_city,s->complete_tour_msgs_rcvd);
          }


     }

     // printf("%i,%i: Messages Received: %i\n",s->self_city,s->self_place,s->msgs_rcvd);


}
