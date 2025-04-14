#pragma once

#include "button_fix_bounce.h"
#include "esp_lcd_ili9488.h"

#include <driver/gpio.h>
#include <driver/ledc.h>
#include <driver/spi_master.h>
#include <esp_err.h>
#include <esp_freertos_hooks.h>
#include <esp_log.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_ili9488.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lvgl.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_logo.h"
#include "freertos/queue.h"
#include "sd_card.h"
#include <math.h>

// static lv_indev_data_t* buttons_data;