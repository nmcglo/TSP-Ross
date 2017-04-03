/*
tsp_main.c
3-30-2017
Neil McGlohon
*/

//includes
#include "tsp.h"


tw_lptype model_lps[] =
{
     {
          (init_f) tsp_init,
          (pre_run_f) tsp_prerun,
          (event_f) tsp_event_handler,
          (revent_f) tsp_RC_event_handler,
          (commit_f) NULL,
          (final_f) tsp_final,
          (map_f) tsp_map,
          sizeof(tsp_actor_state)
     },
     { 0 },
};


// // COMMAND OPS EXAMPLE
//
//Define command line arguments default values
int num_cities= 4;
int min_fanout= 1;
int max_fanout= 4;

//Command line opts
const tw_optdef model_opts[] = {
     TWOPT_GROUP("Mail Model"),
     TWOPT_UINT("cities", num_cities, "Total cities in simulation"),
     TWOPT_UINT("minfanout", min_fanout, "Min Fanout of graph nodes (Not implemented yet)"),
     TWOPT_UINT("maxfanout", max_fanout, "Max Fanout of graph nodes (Not Implemented yet)"),
     TWOPT_END()
};


//Displays simple settings of the simulation
void displayModelSettings()
{
     if(g_tw_mynode ==0)
     {
          for (int i = 0; i < 30; i++)
          {
               printf("*");
          }
          printf("\n");

          printf("Traveling Salesman Problem Solver Configuration:\n");
          printf("\tnnodes: %i\n", tw_nnodes());
          printf("\tg_tw_nlp: %lu\n", g_tw_nlp);

          printf("\tNLPs per PE: %i\n", nlp_per_pe);

          printf("\tTotal Actors: %i\n", total_actors);

          //Put your settings to print here

          for (int i = 0; i < 30; i++)
          {
               printf("*");
          }
          printf("\n");

     }
}

void initialize()
{
     total_cities = num_cities;

     unsigned int W[4][4] = {
          {0,6,8,11},
          {6,0,7,9},
          {8,7,0,5},
          {11,9,5,0}
     };

     weight_matrix = (int**) calloc(4,sizeof(int*));
     int i;
     for(i = 0; i < 4; i++)
     {
         weight_matrix[i] = (int*)calloc(4,sizeof(int));
     }

     for(i = 0; i < 4; i++)
     {
        int j;
         for(j = 0; j < 4; j++)
         {
             weight_matrix[i][j] = W[i][j];
         }
     }
}



//for doxygen
#define lif_main main
int lif_main(int argc, char** argv, char **env)
{
     // tw_opt_add(model_opts);
     tw_init(&argc, &argv);

     initialize();

     nlp_per_pe = 1;
     custom_LPs_per_pe = 1;

     g_tw_events_per_pe = 1000000;

     total_actors = total_cities*total_cities + total_cities; //N^2 + N, One for each city-position, and one for each ending city

     g_tw_nlp = (total_actors);
     g_tw_lookahead = .0001;
     custom_LPs_per_pe = (g_tw_nlp / g_tw_npe)/tw_nnodes();

     displayModelSettings();

     g_tw_lp_types = model_lps;
     // g_tw_lp_typemap = lpTypeMapper;

     tw_define_lps(custom_LPs_per_pe, sizeof(tsp_mess));

     tw_lp_setup_types();

     tw_run();
     tw_end();

     // exportFinalData();


     return 0;
}
