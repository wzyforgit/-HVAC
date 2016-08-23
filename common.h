#ifndef COMMON_H
#define COMMON_H

static inline int ceiling(double num)
{
    return num-(int)num>0?(int)(num+1):(int)num;
}

#endif // COMMON_H
