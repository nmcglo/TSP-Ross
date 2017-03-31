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
// //Define command line arguments default values
// int total_mailboxes= 0;
// int total_post_offices = 0;
//
// //Command line opts
// const tw_optdef model_opts[] = {
//      TWOPT_GROUP("Mail Model"),
//      TWOPT_UINT("mailboxes", total_mailboxes, "Number of mailboxes in simulation"),
//      TWOPT_UINT("postoffices", total_post_offices, "Number of post offices in simulation"),
//      TWOPT_END()
// };


//Displays simple settings of the simulation
void displayModelSettings()
{
     // if(g_tw_mynode ==0)
     // {
     //      for (int i = 0; i < 30; i++)
     //      {
     //           printf("*");
     //      }
     //      printf("\n");
     //
     //      printf("Traveling Salesman Problem Solver Configuration:\n");
     //      printf("\tnnodes: %i\n", tw_nnodes());
     //      printf("\tg_tw_nlp: %llu\n", g_tw_nlp);
     //
     //      printf("\tNLPs per PE: %i\n", nlp_per_pe);
     //
     //      printf("\tTotal Neurons: %i\n", total_neurons);
     //
     //      //Put your settings to print here
     //
     //      for (int i = 0; i < 30; i++)
     //      {
     //           printf("*");
     //      }
     //      printf("\n");
     //
     // }
}



//for doxygen
#define lif_main main
int lif_main(int argc, char** argv, char **env)
{
     // tw_opt_add(model_opts);
     tw_init(&argc, &argv);

     nlp_per_pe = 1;
     custom_LPs_per_pe = 1;

     simulation_length = 1000;

     g_tw_events_per_pe = 1000000;

     g_tw_nlp = (total_neurons);
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
