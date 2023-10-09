#include "tft.h"
#include "common.h"
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[TFT_WIDTH * TFT_HEIGHT / 10];

void tft_flush(lv_disp_drv_t *display, const lv_area_t *area, lv_color_t *color_p);

void tft_initialize(void)
{
    lv_init();
    tft.begin();
    tft.setRotation(3);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_WIDTH * TFT_HEIGHT / 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_HEIGHT;
    disp_drv.ver_res = TFT_WIDTH;
    disp_drv.flush_cb = tft_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    lv_obj_set_style_pad_all(lv_scr_act(), 2, LV_PART_MAIN);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
}

void tft_clear_screen(void)
{
    lv_obj_clean(lv_scr_act());
}

void tft_timer_handler(void)
{
    lv_timer_handler();
}

void tft_wait_for_connection(void)
{
    tft_clear_screen();
    static lv_obj_t *wait_label = lv_label_create(lv_scr_act());
    lv_label_set_text(wait_label, "Waiting for connection...");
    lv_obj_align(wait_label, LV_ALIGN_CENTER, 0, 40);
    lv_obj_set_style_text_font(wait_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(wait_label, lv_color_white(), LV_PART_MAIN);
    lv_timer_handler();
}

void tft_sensor_calibration(uint16_t value, int32_t minimum, int32_t maximum, const char *sensor_name)
{
    static lv_obj_t *calibrate_label = lv_label_create(lv_scr_act());
    static lv_obj_t *sensor_name_label = lv_label_create(lv_scr_act());
    static lv_obj_t *value_name_label = lv_label_create(lv_scr_act());
    static lv_obj_t *minimum_name_label = lv_label_create(lv_scr_act());
    static lv_obj_t *maximum_name_label = lv_label_create(lv_scr_act());
    static lv_obj_t *current_value_label = lv_label_create(lv_scr_act());
    static lv_obj_t *minimum_current_value_label = lv_label_create(lv_scr_act());
    static lv_obj_t *maximum_current_value_label = lv_label_create(lv_scr_act());

    if (strcmp(lv_label_get_text(calibrate_label), "Calibrate") != 0)
    {
        // Title label
        lv_label_set_text(calibrate_label, "Calibrate");
        lv_obj_align(calibrate_label, LV_ALIGN_TOP_MID, 0, 40);
        lv_obj_set_style_text_font(calibrate_label, &lv_font_montserrat_10, 0);
        lv_obj_set_style_text_color(calibrate_label, lv_color_white(), LV_PART_MAIN);

        // Sensor name title
        lv_obj_align(sensor_name_label, LV_ALIGN_TOP_MID, 0, 60);
        lv_obj_set_style_text_font(sensor_name_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_color(sensor_name_label, lv_color_white(), LV_PART_MAIN);

        // Value name label
        lv_label_set_text(value_name_label, "Value:");
        lv_obj_align(value_name_label, LV_ALIGN_BOTTOM_LEFT, 50, -80);
        lv_obj_set_style_text_font(value_name_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_color(value_name_label, lv_color_white(), LV_PART_MAIN);

        // Minimum name label
        lv_label_set_text(minimum_name_label, "Minimum:");
        lv_obj_align(minimum_name_label, LV_ALIGN_BOTTOM_LEFT, 50, -60);
        lv_obj_set_style_text_font(minimum_name_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_color(minimum_name_label, lv_color_white(), LV_PART_MAIN);

        // Maximum name label
        lv_label_set_text(maximum_name_label, "Maximum:");
        lv_obj_align(maximum_name_label, LV_ALIGN_BOTTOM_LEFT, 50, -40);
        lv_obj_set_style_text_font(maximum_name_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_color(maximum_name_label, lv_color_white(), LV_PART_MAIN);

        // Value label
        lv_obj_align(current_value_label, LV_ALIGN_BOTTOM_RIGHT, -50, -80);
        lv_obj_set_style_text_font(current_value_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_color(current_value_label, lv_color_white(), LV_PART_MAIN);

        // Minimum label
        lv_obj_align(minimum_current_value_label, LV_ALIGN_BOTTOM_RIGHT, -50, -60);
        lv_obj_set_style_text_font(minimum_current_value_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_color(minimum_current_value_label, lv_color_white(), LV_PART_MAIN);

        // Maximum label
        lv_obj_align(maximum_current_value_label, LV_ALIGN_BOTTOM_RIGHT, -50, -40);
        lv_obj_set_style_text_font(maximum_current_value_label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_color(maximum_current_value_label, lv_color_white(), LV_PART_MAIN);
    }

    lv_label_set_text(sensor_name_label, sensor_name);
    lv_label_set_text_fmt(current_value_label, "%d", value);
    lv_label_set_text_fmt(minimum_current_value_label, "%d", minimum);
    lv_label_set_text_fmt(maximum_current_value_label, "%d", maximum);

    lv_timer_handler();
}

void tft_flush(lv_disp_drv_t *display, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *) &color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(display);
}
