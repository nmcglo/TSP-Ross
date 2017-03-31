#include "tsp.h"


//Given an LP's GID (global ID)
//return the PE (aka node, MPI Rank)
tw_peid tsp_map(tw_lpid gid)
{
     return (tw_peid) gid / g_tw_nlp;
}
