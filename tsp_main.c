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
          (commit_f) tsp_commit,
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
     TWOPT_GROUP("TSP Models"),
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
          printf("\tnnodes:       %i\n", tw_nnodes());
          printf("\tg_tw_nlp:     %llu\n", g_tw_nlp);
          printf("\tNLPs per PE:  %i\n", nlp_per_pe);
          printf("\tTotal Actors: %i\n", total_actors);
          printf("\tTotal Cities: %i\n", num_cities);
          printf("\n");
          printf("\tuint64s needed: %i\n",MAX_INTS_NEEDED);

          printf("\tAvl node count: %i\n", g_tw_avl_node_count);

          // printf("\tMin Fanout:   %i\n", min_fanout);
          // printf("\tMax Fanout:   %i\n", max_fanout);


          //Put your settings to print here

          for (int i = 0; i < 30; i++)
          {
               printf("*");
          }
          printf("\n");

     }
}

//DO NOT USE THIS METHOD WITHIN AN LP
int rand_range(int low, int high)
{
     return((int) (rand() % (high-low+1)) + low);
}

void initialize()
{
     total_cities = num_cities;

     weight_matrix = (int**) calloc(num_cities,sizeof(int*));
     int i;
     for(i = 0; i < num_cities; i++)
     {
         weight_matrix[i] = (int*)calloc(num_cities,sizeof(int));
     }

     srand(100);

     //make symmetrical graph with random integer weights
     for(int i = 0; i < num_cities; i++)
     {
          for(int j = 0; j < num_cities; j++)
          {
               if(j > i)
                    weight_matrix[i][j] = rand_range(MIN_CITY_SEPARATION,MAX_CITY_SEPARATION); //rand int in [MIN, MAX]
               else if(i > j)
                    weight_matrix[i][j] = weight_matrix[j][i];
          }
     }
}



//for doxygen
#define lif_main main
int lif_main(int argc, char** argv, char **env)
{
     tw_opt_add(model_opts);
     tw_init(&argc, &argv);

     initialize();

     nlp_per_pe = 1;
     custom_LPs_per_pe = 1;

     jitter = .0001;
     weight_multiplier = 1;

     g_tw_events_per_pe = 1000000;
     // g_tw_avl_node_count = g_tw_avl_node_count * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;

     total_actors = total_cities*total_cities + total_cities; //N^2 + N, One for each city-position, and one for each ending city


     g_tw_nlp = (total_actors);
     g_tw_lookahead = jitter * .001;
     custom_LPs_per_pe = (g_tw_nlp / g_tw_npe)/tw_nnodes();
     nlp_per_pe = custom_LPs_per_pe;

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
