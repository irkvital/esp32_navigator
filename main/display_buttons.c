// #define LV_COLOR_SCREEN_TRANSP 1
#include "display_buttons.h"


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


// MAP _______________________________________________________________________________


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
    lv_obj_set_size(img_base, 320, 320);
    lv_obj_remove_style(img_base, NULL, LV_PART_SCROLLBAR);

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
    float xtile;
    float ytile;
    mathTiles(zoom, lat, lon, &xtile, &ytile);

    int offset_x = 0;
    int offset_y = 0;
    int offset_tile_x = (xtile - (int)xtile) * 256;
    int offset_tile_y = (ytile - (int)ytile) * 256;

    char path[50];
    for (int i = 0; i < 9; i++) {
        int ii = i / 3;
        int ij = i % 3;
        offset_x = 256 * ij;
        offset_y = 256 * ii;
        if ((int)xtile != (int)xtile_old || (int)ytile != (int)ytile_old) {
            lv_png_init();
            sprintf(path, "A:/sdcard/Tiles/%d/%d/%d.png", zoom, (int)xtile + ij - 1, (int)ytile + ii - 1);
            lv_img_set_src(img[i], path);
            lv_obj_align(img[i], LV_ALIGN_CENTER, offset_x, offset_y);
        }
        lv_img_set_pivot(img[i], -offset_x + 256 + offset_tile_x, -offset_y + 256 + offset_tile_y);
    }

    lv_obj_scroll_to(img_base, offset_tile_x + 128, offset_tile_y + 128, LV_ANIM_OFF);

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

    lv_indev_set_group(my_indev, g);
    int count = 0;
    while (1)
    {
        lv_timer_handler();


        for (int i = 0; i < 9; i++) {
            lv_img_set_angle(img[i], count % 3600);
        }

        if (count % 600 == 0) {
            lat += 0.00005; // широта
            lon += 0.00003; // долгота
            drawTile(zoom);
            // count = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(20));

        count += 60;
    }
}


void app_main()
{
    buttonFixInit();
    xTaskCreatePinnedToCore(display_task, "demo", 10240, NULL, 20, NULL, 1);
}

