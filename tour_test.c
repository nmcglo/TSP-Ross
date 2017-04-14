#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define MAX_TOUR_LENGTH 13

#define BITS_TO_ENCODE_TOUR ((MAX_TOUR_LENGTH*MAX_TOUR_LENGTH) + MAX_TOUR_LENGTH)//equivalent to number of max unique gids
#define MAX_INTS_NEEDED ((BITS_TO_ENCODE_TOUR / 64) + 1)

int total_cities = 10;


typedef uint64_t compact_tour_part_t;
typedef int tw_lpid;



tw_lpid get_lp_gid(int city, int place);
int get_city_from_gid(tw_lpid gid);
int get_place_from_gid(tw_lpid gid);



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

int get_city_from_gid(tw_lpid gid)
{
     return gid%total_cities;
}

int get_place_from_gid(tw_lpid gid)
{
     return gid/total_cities;
}


int get_lp_gid(int city, int place)
{
     return place*total_cities + city;
}

int main()
{
     printf("%d ints needed\n", MAX_INTS_NEEDED);

     compact_tour_part_t testTour[MAX_INTS_NEEDED];
     for(int i = 0; i < MAX_INTS_NEEDED; i++)
     {
          testTour[i] = 0;
     }

     addToTour(0,0,testTour);
     addToTour(1,1,testTour);
     addToTour(3,2,testTour);
     addToTour(2,3,testTour);
     addToTour(4,4,testTour);
     addToTour(5,5,testTour);
     addToTour(6,6,testTour);
     addToTour(9,7,testTour);
     addToTour(8,8,testTour);
     addToTour(7,9,testTour);


     for(int i = 0; i < total_cities; i++)
     {
          if(isInTour(i,MAX_TOUR_LENGTH,testTour))
               printf("%i is in tour\n",i);
          else
               printf("%i is not in tour\n",i);
     }

     for(int i =0; i < MAX_INTS_NEEDED; i++)
     {
          for(int j = 0; j < 64; j++)
          {
               printf("%d",get_bit_rev(testTour[i],j));
               // if(j % total_cities == total_cities-1)
               //      printf(" ");
          }
          printf("\n");
     }

     int actualTour[total_cities+1];

     decodeTour(testTour,actualTour);

     actualTour[total_cities] = 0;

     for(int i =0; i<total_cities+1; i++)
     {
          printf("%d ",actualTour[i]);
     }
     printf("\n");


     return 0;
}
