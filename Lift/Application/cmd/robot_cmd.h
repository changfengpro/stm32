/*
 * robot_cmd.c
 *
 *  Created on: Aug 26, 2024
 *      Author: auroranebulas
 */

#ifndef CMD_ROBOT_CMD_H_
#define CMD_ROBOT_CMD_H_

#include "main.h"

#define RC_CH_VALUE_MIN 364
#define RC_CH_VALUE_OFFSET 1024
#define RC_CH_VALUE_MAX 1684


typedef union
{
    struct
    {
        int ch0 : 11;
        int ch1 : 11;
        int ch2 : 11;
        int ch3 : 11;
        // int ch4 : 11;
        uint8_t s1 : 2;
        uint8_t s2 : 2;
        // uint8_t reserved : 2;   //保留位，确保对齐
    }bits;
    uint8_t buffer[36];
} rc_union;

typedef struct 
{
    rc_union data;
    int ch0;
    int ch1;
    int ch2;
    int ch3;
    int ch4;
    uint8_t s1;
    uint8_t s2;
    uint8_t last_s1;
    uint8_t last_ch4;
} rc;



#endif /* CMD_ROBOT_CMD_H_ */
