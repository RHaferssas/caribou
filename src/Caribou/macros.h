#ifndef CARIBOU_MACROS_H
#define CARIBOU_MACROS_H

#define IN_CLOSED_INTERVAL(a,x,b) (((x)-(a)) * ((x)-(b)) <= 0)
#define IN_OPEN_INTERVAL(a,x,b) (((x)-(a)) * ((x)-(b)) < 0)

#ifdef CARIBOU_DEBUG
#define caribou_assert(x) assert(x)
#else
#define caribou_assert(x)
#endif

#endif //CARIBOU_MACROS_H
