#pragma once

#define NS_IN_SEC 1000000000ULL

#include <stdint.h>
#include <time.h>

uint64_t delta_time(struct timespec end, struct timespec start);
int min(int a, int b);
int max(int a, int b);
double normalize(int value, int range);
int denormalize(double value, int range);
