#ifndef __INCLUDED_INPUT_H__
#define __INCLUDED_INPUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

extern struct input_t {
    bool button_a;
    bool button_b;
    bool button_p2_a;
    bool button_p2_b;
    bool restart;
} input;

#ifdef __cplusplus
}
#endif

#endif

