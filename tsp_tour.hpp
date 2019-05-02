#ifndef _tsp_tour_h
#define _tsp_tour_h

#include "tsp.hpp"


uint8_t get_bit(uint64_t bits, uint8_t pos);

uint8_t get_bit_rev(uint64_t bits, uint8_t pos);

uint64_t set_bit(uint64_t bits, uint8_t pos, uint8_t value);

uint8_t isInTour(int city, int endPlace, compact_tour_part_t *tour);

void addToTour(int city, int place, compact_tour_part_t *tour);

void decodeTour(compact_tour_part_t *encodedTour, int *decodedTour);

tw_lpid get_first_gid_in_tour(compact_tour_part_t *tour);

#endif