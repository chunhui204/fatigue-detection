#include "utils.hpp"
#include <iomanip>


unsigned long get_cur_time(void)
{
    struct timeval tv;
    unsigned long ts;

    gettimeofday(&tv,NULL);

    ts=tv.tv_sec*1000000+tv.tv_usec;

    return ts;
}
