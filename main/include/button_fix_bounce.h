#ifndef BUTTON_FIX_BOUNCE_H
#define BUTTON_FIX_BOUNCE_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h" 
#include "esp_log.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_timer.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"


#define WAIT_BOUNCE_MS 50 / portTICK_PERIOD_MS
#define BUTTONS_NUM 3

typedef struct  {
    int key;
    int status;
} butt_data_t;


void buttonFixInit();
void buttonFixPressed(int button_num);
void buttonFixReleased(int button_num);

int get_key();
int get_status();


#endif // BUTTON_FIX_BOUNCE_H