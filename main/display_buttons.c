/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
// #define LV_COLOR_SCREEN_TRANSP 1
#include "display_buttons.h"

// static lv_obj_t *img_logo;
// LV_IMG_DECLARE(esp_logo)

// void draw_image() {
//     // Create image
//     img_logo = lv_img_create(lv_scr_act());
//     lv_img_set_src(img_logo, &esp_logo);
//     // lv_obj_center(img_logo);
//     lv_obj_set_pos(img_logo, 50, 50);    
// }

static float lat = 55.553953; // широта
static float lon = 37.500529; // долгота
// INIT INPUT_________________________________________________________________

lv_indev_drv_t indev_drv;
lv_indev_t * my_indev;
lv_group_t * g;

void keyboard_read(lv_indev_drv_t * drv, lv_indev_data_t * data){
    data->key = get_key();
    switch (get_key()) {
        case 0:
            data->key = LV_KEY_RIGHT;
            break;
        case 1:
            data->key = LV_KEY_ENTER;
            break;
        case 2:
            data->key = LV_KEY_LEFT;
            break;
    }
    if(get_status()) {
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void init_buttons() {
    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = keyboard_read;
    /*Register the driver in LVGL and save the created input device object*/
    my_indev = lv_indev_drv_register(&indev_drv);
    g = lv_group_create();
}

// SLIDER _________________________________________________________________________________

static void slider_event_cb(lv_event_t * e);
static lv_obj_t * slider_label;

/**
 * A default slider with a label displaying the current value
 */
void lv_example_slider_1(void)
{
    /*Create a slider in the center of the display*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act());
    lv_obj_center(slider);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /*Create a label below the slider*/
    slider_label = lv_label_create(lv_scr_act());
    lv_label_set_text(slider_label, "0%");

    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_group_add_obj(g, slider);
}

static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, buf);
    // lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}




//  METER ______________________________________________________________________________

// static lv_obj_t *meter = NULL;

// static void update_meter_value(void *indic, int32_t v)
// {
//     lv_meter_set_indicator_end_value(meter, indic, v);
// }
// static lv_style_t style_screen;
// void create_demo_ui()
// {
//     // Set the background color of the display to black.
//     lv_style_init(&style_screen);
//     lv_style_set_bg_color(&style_screen, lv_color_white());
//     lv_obj_add_style(lv_scr_act(), &style_screen, LV_STATE_DEFAULT);

//     // Create a meter which can be animated.
//     meter = lv_meter_create(lv_scr_act());
//     lv_obj_center(meter);
//     lv_obj_set_y(meter, -120);
//     lv_obj_set_size(meter, 200, 200);

//     // Add a scale first
//     lv_meter_scale_t *scale = lv_meter_add_scale(meter);
//     lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
//     lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);

//     lv_meter_indicator_t *indic;

//     // Add a blue arc to the start
//     indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
//     lv_meter_set_indicator_start_value(meter, indic, 0);
//     lv_meter_set_indicator_end_value(meter, indic, 20);

//     // Make the tick lines blue at the start of the scale
//     indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE), false, 0);
//     lv_meter_set_indicator_start_value(meter, indic, 0);
//     lv_meter_set_indicator_end_value(meter, indic, 20);

//     // Add a red arc to the end
//     indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
//     lv_meter_set_indicator_start_value(meter, indic, 80);
//     lv_meter_set_indicator_end_value(meter, indic, 100);

//     // Make the tick lines red at the end of the scale
//     indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
//     lv_meter_set_indicator_start_value(meter, indic, 80);
//     lv_meter_set_indicator_end_value(meter, indic, 100);

//     // Add a needle line indicator
//     indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

//     // Create an animation to set the value
//     lv_anim_t a;
//     lv_anim_init(&a);
//     lv_anim_set_exec_cb(&a, update_meter_value);
//     lv_anim_set_var(&a, indic);
//     lv_anim_set_values(&a, 0, 100);
//     lv_anim_set_time(&a, 2000);
//     lv_anim_set_repeat_delay(&a, 100);
//     lv_anim_set_playback_time(&a, 500);
//     lv_anim_set_playback_delay(&a, 100);
//     lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
//     lv_anim_start(&a);
// }

// BUTTONS ___________________________________________________________________

// static void event_handler(lv_event_t * e)
// {
//     lv_event_code_t code = lv_event_get_code(e);

//     if(code == LV_EVENT_CLICKED) {
//         ESP_LOGI("DISP", "Clicked");
//     }
//     else if(code == LV_EVENT_VALUE_CHANGED) {
//         ESP_LOGI("DISP", "Toggled");
//     }
// }

// void lv_example_btn_1(void)
// {
//     lv_obj_t * label;

//     lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
//     lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
//     lv_obj_align(btn1, LV_ALIGN_CENTER, -50, 100);

//     label = lv_label_create(btn1);
//     lv_label_set_text(label, "Button");
//     lv_obj_center(label);

//     lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
//     lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
//     lv_obj_align(btn2, LV_ALIGN_CENTER, 50, 100);
//     lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
//     lv_obj_set_height(btn2, LV_SIZE_CONTENT);

//     label = lv_label_create(btn2);
//     lv_label_set_text(label, "Toggle");
//     lv_obj_center(label);

//     lv_group_add_obj(g, btn1);
//     lv_group_add_obj(g, btn2);
// }


//  ARC ___________________________________________________________________________


// static void value_changed_event_cb(lv_event_t * e)
// {
//     lv_obj_t * arc = lv_event_get_target(e);
//     lv_obj_t * label = lv_event_get_user_data(e);

//     lv_label_set_text_fmt(label, "%d%%", lv_arc_get_value(arc));

//     /*Rotate the label to the current position of the arc*/
//     lv_arc_rotate_obj_to_angle(arc, label, 25);
// }

// void lv_example_arc_1(void)
// {
//     lv_obj_t * label = lv_label_create(lv_scr_act());

//     /*Create an Arc*/
//     lv_obj_t * arc = lv_arc_create(lv_scr_act());
//     lv_obj_set_size(arc, 150, 150);
//     lv_arc_set_rotation(arc, 135);
//     lv_arc_set_bg_angles(arc, 0, 270);
//     lv_arc_set_value(arc, 50);
//     lv_obj_center(arc);
//     lv_obj_set_y(arc, 160);
//     lv_obj_add_flag(arc, LV_OBJ_FLAG_CHECKABLE);
//     lv_obj_add_event_cb(arc, value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label);
//     /*Manually update the label for the first time*/
//     lv_event_send(arc, LV_EVENT_VALUE_CHANGED, NULL);

//     lv_group_add_obj(g, arc);
// }

lv_obj_t * img_base;
lv_obj_t * img[9];
lv_obj_t * center_circle;


void centerCircle(void)
{
    center_circle = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(center_circle, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_radius(center_circle, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(center_circle, 20, 20);
    lv_obj_center(center_circle);
}

void mathTiles(int zoom, float lat, float lon, float* xtile, float* ytile) {
    *xtile = (1 << zoom) * ((lon + 180) / 360);
    float lat_rad = lat * M_PI / 180;
    float sin = sinf(lat_rad);
    float cos = cosf(lat_rad);
    float log = logf((sin + 1) / cos);
    *ytile = (1 << (zoom - 1)) * (1 - log / M_PI);
}

void mapInit() {
    img_base = lv_obj_create(lv_scr_act());
    // lv_obj_set_style_radius(img_base, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(img_base, true, 0);
    lv_obj_set_size(img_base, 320, 480);
    // lv_obj_clear_flag(img_base, LV_OBJ_FLAG_FLOATING);
    // lv_obj_remove_style(img_base, NULL, LV_PART_SCROLLBAR);

    // lv_obj_clear_flag(img_base, LV_OBJ_FLAG_SCROLLABLE);
    lv_img_cache_set_size(18);
    lv_obj_align(img_base, LV_ALIGN_CENTER, 0, 0);
    for (int i = 0; i < 9; i++) {
        // init
        if (img[i] == NULL) {
            img[i] = lv_img_create(img_base);
        }
    }
}

static float xtile_old;
static float ytile_old;

void drawTile(int zoom) {
    // int zoom = 17;
    // float lat = 55.553953; // широта
    // float lon = 37.500529; // долгота
    float xtile;
    float ytile;
    mathTiles(zoom, lat, lon, &xtile, &ytile);

    int offset_x = 0;
    int offset_y = 0;

    if ((int)xtile != (int)xtile_old || (int)ytile != (int)ytile_old) {
        char path[50];
        lv_png_init();

        for (int i = 0; i < 9; i++) {
            int ii = i / 3;
            int ij = i % 3;
            offset_x = 256 * ij;
            offset_y = 256 * ii;
            sprintf(path, "A:/sdcard/Tiles/%d/%d/%d.png", zoom, (int)xtile + ij - 1, (int)ytile + ii - 1);
            lv_img_set_src(img[i], path);
            lv_obj_align(img[i], LV_ALIGN_CENTER, offset_x, offset_y);
            // lv_img_set_pivot(img[i], -offset_x + 128, -offset_y + 128);
        }
    }

    offset_x = 128 - (xtile - (int)xtile) * 256;
    offset_y = 128 - (ytile - (int)ytile) * 256;
    lv_obj_scroll_to(img_base, -offset_x + 256, -offset_y + 256, LV_ANIM_OFF);

    xtile_old = xtile;
    ytile_old = ytile;
}

static int zoom = 17;

static void event_handler_tile(lv_event_t * e)
{
    uint32_t key = lv_event_get_key(e);
    if(key == LV_KEY_RIGHT) {
        if (zoom < 17) {
            zoom++;
            lv_img_cache_set_size(18);
            drawTile(zoom);
        }
    } else if(key == LV_KEY_LEFT) {
        if (zoom > 6) {
            zoom--;
            lv_img_cache_set_size(18);
            drawTile(zoom);
        }  
    }
}



void drawMap() {
    mapInit();
    drawTile(zoom);
    centerCircle();
    lv_group_add_obj(g, img[4]);
    lv_obj_add_event_cb(img[4], event_handler_tile, LV_EVENT_KEY, NULL);
}

void display_task(void* arg) {
    init_display();
    init_buttons();
    sd_mount();

    drawMap();
    

    // lv_example_btn_1();
    // lv_example_slider_1();

    lv_indev_set_group(my_indev, g);
    // TickType_t prew;
    int count = 0;
    while (1)
    {
        lv_timer_handler();


        // for (int i = 0; i < 9; i++) {
        //     lv_img_set_angle(img[i], count % 3600);
        // }

        // if (count == 10) {
            lat -= 0.00005; // широта
            lon -= 0.00003; // долгота
            drawTile(zoom);
            // count = 0;
        // }

        vTaskDelay(pdMS_TO_TICKS(20));

        count += 1;
    }
}


void app_main()
{
    buttonFixInit();
    xTaskCreatePinnedToCore(display_task, "demo", 10240, NULL, 20, NULL, 1);
}

