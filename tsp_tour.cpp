
#include "tsp_tour.hpp"

uint8_t get_bit(uint64_t bits, uint8_t pos)
{
   return (bits >> pos) & 0x01;
}

uint8_t get_bit_rev(uint64_t bits, uint8_t pos)
{
     return get_bit(bits, (63-pos)%64);
}

uint64_t set_bit(uint64_t bits, uint8_t pos, uint8_t value)
{
   uint64_t mask = 1LL << (63 - pos);
   if (value)
       bits |= mask;
   else
       bits &= ~mask;
   return bits;
}

uint8_t isInTour(int city, int endPlace, compact_tour_part_t *tour)
{
     //need to check all city-loc pairs (encoded in the GID) to determine if the city has been visited yet
     for(int place = 0; place < endPlace; place++)
     {
          tw_lpid gid = get_lp_gid(city, place); //get the gid of the city for this particular place

          int tour_partition = gid / 64; //get the partiition of the tour that the gid's bool value is found
          uint8_t pos = gid % 64; //get the offset that the gid's bool val is found in the partition

          if(get_bit_rev(tour[tour_partition],pos))
               return 1;
     }
     return 0;
}

void addToTour(int city, int place, compact_tour_part_t *tour)
{
     tw_lpid gid = get_lp_gid(city, place);

     int tour_partition = gid / 64; //get the partiition of the tour that the gid's bool value is found
     uint8_t pos = gid % 64; //get the offset that the gid's bool val is found in the partition

     tour[tour_partition] = set_bit(tour[tour_partition], pos, 1);
}

void decodeTour(compact_tour_part_t *encodedTour, int *decodedTour)
{
     for(int part = 0; part < MAX_INTS_NEEDED; part++)
     {
          for(int offset = 0; offset < 64; offset++)
          {
               uint8_t bit = get_bit_rev(encodedTour[part],offset);
               if(bit)
               {
                    tw_lpid gid = part*64 + offset;
                    int city = get_city_from_gid(gid);
                    int place = get_place_from_gid(gid);

                    decodedTour[place] = city;
               }
          }
     }
}

tw_lpid get_first_gid_in_tour(compact_tour_part_t *tour)
{
     int found = 0;
     int part = 0;
     int offset = 0;
     for(part = 0; part < MAX_INTS_NEEDED; part++)
     {
          for(offset = 0; offset < 64; offset++)
          {
               if(get_bit_rev(tour[part],offset))
               {
                    found = 1;
               }
               if(found)
                    break;
          }
          if(found)
               break;
     }

     return(64*part + offset);
}

