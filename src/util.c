#include "util.h"

uint64_t delta_time(struct timespec end, struct timespec start) {
    return (end.tv_sec - start.tv_sec) * NS_IN_SEC +
            (end.tv_nsec - start.tv_nsec);
}

inline int min(int a, int b) {
    return a < b ? a : b;
}

inline int max(int a, int b) {
    return a > b ? a : b;
}

inline double normalize(int value, int range) {
    return (double)value / range;
}

inline int denormalize(double value, int range) {
    return (int)(value * range);
}
