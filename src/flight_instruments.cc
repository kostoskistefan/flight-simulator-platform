#include <flight_instruments.h>
#include <lvgl.h>

static lv_obj_t *airspeed_indicator_meter;
static lv_meter_indicator_t *airspeed_indicator_needle;

static lv_obj_t *altimeter_meter;
static lv_meter_indicator_t *altimeter_one_thousand_feet_needle;
static lv_meter_indicator_t *altimeter_ten_thousand_feet_needle;

static lv_obj_t *vertical_speed_indicator_meter;
static lv_meter_indicator_t *vertical_speed_indicator_needle;

static lv_obj_t *heading_meter;
static lv_meter_indicator_t *heading_needle;

// Version 1 - Requires a lot of ram
// static lv_obj_t *attitude_indicator_roll_container;
// static lv_obj_t *attitude_indicator_pitch_container;
// static lv_obj_t *attitude_indicator_top_arc;
// static lv_obj_t *attitude_indicator_bottom_arc;

// Version 2 - Low resource usage
static lv_obj_t *attitude_meter;
static lv_meter_indicator_t *attitude_roll_needle;
static lv_meter_indicator_t *attitude_pitch_needle;

void airspeed_indicator_set_value(uint8_t speed)
{
    lv_meter_set_indicator_value(airspeed_indicator_meter, airspeed_indicator_needle, speed);
}

void altimeter_set_value(int32_t altitude)
{
    lv_meter_set_indicator_value(altimeter_meter, altimeter_one_thousand_feet_needle, (altitude / 10) % 100);
    lv_meter_set_indicator_value(altimeter_meter, altimeter_ten_thousand_feet_needle, (altitude / 100) % 100);
}

void attitude_indicator_set_roll_value(int8_t roll)
{
    lv_meter_set_indicator_value(attitude_meter, attitude_roll_needle, roll);
}

void attitude_indicator_set_pitch_value(int8_t pitch)
{
    lv_meter_set_indicator_value(attitude_meter, attitude_pitch_needle, pitch);
}

void heading_indicator_set_value(uint16_t heading)
{
    lv_meter_set_indicator_value(heading_meter, heading_needle, heading % 360);
}

void vertical_speed_indicator_set_value(int32_t vertical_speed)
{
    lv_meter_set_indicator_value(vertical_speed_indicator_meter, vertical_speed_indicator_needle, vertical_speed);
}

void airspeed_indicator(void)
{
    airspeed_indicator_meter = lv_meter_create(lv_scr_act());
    lv_obj_set_size(airspeed_indicator_meter, 116, 116);
    lv_obj_align(airspeed_indicator_meter, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_pad_all(airspeed_indicator_meter, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(airspeed_indicator_meter, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_color(airspeed_indicator_meter, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_color(airspeed_indicator_meter, lv_color_white(), LV_PART_TICKS);
    lv_obj_set_style_text_font(airspeed_indicator_meter, &lv_font_montserrat_10, LV_PART_TICKS);

    lv_meter_scale_t *scale = lv_meter_add_scale(airspeed_indicator_meter);
    lv_meter_set_scale_range(airspeed_indicator_meter, scale, 0, 200, 330, 270);
    lv_meter_set_scale_ticks(airspeed_indicator_meter, scale, 21, 2, 7, lv_color_white());
    lv_meter_set_scale_major_ticks(airspeed_indicator_meter, scale, 2, 2, 9, lv_color_white(), 11);

    lv_meter_indicator_t *indicator;

    indicator = lv_meter_add_arc(airspeed_indicator_meter, scale, 4, lv_color_white(), 1);
    lv_meter_set_indicator_start_value(airspeed_indicator_meter, indicator, 0);
    lv_meter_set_indicator_end_value(airspeed_indicator_meter, indicator, 40);

    indicator = lv_meter_add_arc(airspeed_indicator_meter, scale, 4, lv_palette_main(LV_PALETTE_GREEN), 1);
    lv_meter_set_indicator_start_value(airspeed_indicator_meter, indicator, 40);
    lv_meter_set_indicator_end_value(airspeed_indicator_meter, indicator, 130);

    indicator = lv_meter_add_arc(airspeed_indicator_meter, scale, 4, lv_palette_main(LV_PALETTE_YELLOW), 1);
    lv_meter_set_indicator_start_value(airspeed_indicator_meter, indicator, 130);
    lv_meter_set_indicator_end_value(airspeed_indicator_meter, indicator, 160);

    indicator = lv_meter_add_arc(airspeed_indicator_meter, scale, 4, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(airspeed_indicator_meter, indicator, 160);
    lv_meter_set_indicator_end_value(airspeed_indicator_meter, indicator, 200);

    indicator = lv_meter_add_scale_lines(airspeed_indicator_meter, scale, lv_color_white(), lv_color_white(), 0, 0);
    lv_meter_set_indicator_start_value(airspeed_indicator_meter, indicator, 0);
    lv_meter_set_indicator_end_value(airspeed_indicator_meter, indicator, 40);

    indicator = lv_meter_add_scale_lines(airspeed_indicator_meter,
                                         scale,
                                         lv_palette_main(LV_PALETTE_GREEN),
                                         lv_palette_main(LV_PALETTE_GREEN),
                                         0,
                                         0);
    lv_meter_set_indicator_start_value(airspeed_indicator_meter, indicator, 40);
    lv_meter_set_indicator_end_value(airspeed_indicator_meter, indicator, 130);

    indicator = lv_meter_add_scale_lines(airspeed_indicator_meter,
                                         scale,
                                         lv_palette_main(LV_PALETTE_YELLOW),
                                         lv_palette_main(LV_PALETTE_YELLOW),
                                         0,
                                         0);
    lv_meter_set_indicator_start_value(airspeed_indicator_meter, indicator, 130);
    lv_meter_set_indicator_end_value(airspeed_indicator_meter, indicator, 160);

    indicator = lv_meter_add_scale_lines(airspeed_indicator_meter,
                                         scale,
                                         lv_palette_main(LV_PALETTE_RED),
                                         lv_palette_main(LV_PALETTE_RED),
                                         0,
                                         0);
    lv_meter_set_indicator_start_value(airspeed_indicator_meter, indicator, 160);
    lv_meter_set_indicator_end_value(airspeed_indicator_meter, indicator, 200);

    lv_obj_t *label = lv_label_create(airspeed_indicator_meter);
    lv_label_set_text(label, "KTS");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 18);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);

    airspeed_indicator_needle = lv_meter_add_needle_line(airspeed_indicator_meter, scale, 2, lv_color_white(), -10);
}

void altimeter(void)
{
    altimeter_meter = lv_meter_create(lv_scr_act());
    lv_obj_set_size(altimeter_meter, 116, 116);
    lv_obj_align(altimeter_meter, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_style_pad_all(altimeter_meter, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(altimeter_meter, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_color(altimeter_meter, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
    lv_obj_set_style_text_color(altimeter_meter, lv_color_white(), LV_PART_TICKS);

    lv_meter_scale_t *one_thousand_feet_scale = lv_meter_add_scale(altimeter_meter);
    lv_meter_scale_t *one_thousand_feet_scale_visual = lv_meter_add_scale(altimeter_meter);

    lv_meter_scale_t *ten_thousand_feet_scale = lv_meter_add_scale(altimeter_meter);
    lv_meter_scale_t *ten_thousand_feet_scale_visual = lv_meter_add_scale(altimeter_meter);

    lv_meter_set_scale_range(altimeter_meter, one_thousand_feet_scale, 0, 100, 360, 270);
    lv_meter_set_scale_range(altimeter_meter, ten_thousand_feet_scale, 0, 100, 360, 270);

    lv_meter_set_scale_range(altimeter_meter, one_thousand_feet_scale_visual, 0, 50, 360, 270);
    lv_meter_set_scale_range(altimeter_meter, ten_thousand_feet_scale_visual, 0, 9, 324, 270);

    lv_meter_set_scale_ticks(altimeter_meter, one_thousand_feet_scale_visual, 51, 1, 7, lv_color_white());
    lv_meter_set_scale_ticks(altimeter_meter, ten_thousand_feet_scale_visual, 10, 1, 5, lv_color_white());

    lv_meter_set_scale_major_ticks(altimeter_meter, ten_thousand_feet_scale_visual, 1, 2, 9, lv_color_white(), 10);

    lv_obj_t *label = lv_label_create(altimeter_meter);
    lv_label_set_text(label, "ALT");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 16);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

    altimeter_one_thousand_feet_needle =
        lv_meter_add_needle_line(altimeter_meter, one_thousand_feet_scale, 2, lv_palette_main(LV_PALETTE_RED), -10);
    altimeter_ten_thousand_feet_needle =
        lv_meter_add_needle_line(altimeter_meter, ten_thousand_feet_scale, 4, lv_palette_main(LV_PALETTE_RED), -20);
}

void attitude_indicator(void)
{
    // lv_obj_t *global_container = lv_obj_create(lv_scr_act());
    // lv_obj_set_size(global_container, 116, 116);
    // lv_obj_align(global_container, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_pad_all(global_container, 0, LV_PART_MAIN);
    // lv_obj_set_style_bg_color(global_container, lv_color_black(), LV_PART_MAIN);
    // lv_obj_set_style_radius(global_container, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    // lv_obj_set_style_border_width(global_container, 0, LV_PART_MAIN);

    // attitude_indicator_roll_container = lv_obj_create(global_container);
    // lv_obj_set_size(attitude_indicator_roll_container, 116, 116);
    // lv_obj_align(attitude_indicator_roll_container, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_border_color(attitude_indicator_roll_container, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_pad_all(attitude_indicator_roll_container, 0, LV_PART_MAIN);
    // lv_obj_set_style_bg_color(attitude_indicator_roll_container,
    //                           lv_color_darken(lv_palette_main(LV_PALETTE_BLUE), 128),
    //                           LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_color(attitude_indicator_roll_container,
    //                                lv_color_darken(lv_palette_main(LV_PALETTE_BROWN), 128),
    //                                LV_PART_MAIN);
    // lv_obj_set_style_bg_main_stop(attitude_indicator_roll_container, 128, LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_stop(attitude_indicator_roll_container, 128, LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_dir(attitude_indicator_roll_container, LV_GRAD_DIR_VER, LV_PART_MAIN);
    // lv_obj_set_style_radius(attitude_indicator_roll_container, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    // lv_obj_set_style_transform_pivot_x(attitude_indicator_roll_container, 58, 0);
    // lv_obj_set_style_transform_pivot_y(attitude_indicator_roll_container, 58, 0);

    // attitude_indicator_pitch_container = lv_obj_create(attitude_indicator_roll_container);
    // lv_obj_set_size(attitude_indicator_pitch_container, 80, 60);
    // lv_obj_align(attitude_indicator_pitch_container, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_bg_color(attitude_indicator_pitch_container, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_color(attitude_indicator_pitch_container, lv_palette_main(LV_PALETTE_BROWN), LV_PART_MAIN);
    // lv_obj_set_style_bg_main_stop(attitude_indicator_pitch_container, 128, LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_stop(attitude_indicator_pitch_container, 128, LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_dir(attitude_indicator_pitch_container, LV_GRAD_DIR_VER, LV_PART_MAIN);
    // lv_obj_set_style_pad_all(attitude_indicator_pitch_container, 0, LV_PART_MAIN);
    // lv_obj_set_style_border_width(attitude_indicator_pitch_container, 0, LV_PART_MAIN);
    // lv_obj_set_style_radius(attitude_indicator_pitch_container, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    // lv_obj_set_style_shadow_opa(attitude_indicator_pitch_container, 90, LV_PART_MAIN);
    // lv_obj_set_style_shadow_width(attitude_indicator_pitch_container, 10, LV_PART_MAIN);
    // lv_obj_set_style_shadow_spread(attitude_indicator_pitch_container, 2, LV_PART_MAIN);

    // lv_obj_t *pitch_center_line = lv_line_create(attitude_indicator_pitch_container);
    // static lv_point_t pitch_center_line_points[] = {
    //     { 1, 30},
    //     {79, 30}
    // };
    // lv_line_set_points(pitch_center_line, pitch_center_line_points, 2);
    // lv_obj_set_style_line_color(pitch_center_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(pitch_center_line, 1, LV_PART_MAIN);

    // lv_obj_t *pitch_plus_ten_line = lv_line_create(attitude_indicator_pitch_container);
    // static lv_point_t pitch_plus_ten_line_points[] = {
    //     {30, 20},
    //     {50, 20}
    // };
    // lv_line_set_points(pitch_plus_ten_line, pitch_plus_ten_line_points, 2);
    // lv_obj_set_style_line_color(pitch_plus_ten_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(pitch_plus_ten_line, 1, LV_PART_MAIN);

    // lv_obj_t *pitch_plus_twenty_line = lv_line_create(attitude_indicator_pitch_container);
    // static lv_point_t pitch_plus_twenty_line_points[] = {
    //     {20, 10},
    //     {60, 10}
    // };
    // lv_line_set_points(pitch_plus_twenty_line, pitch_plus_twenty_line_points, 2);
    // lv_obj_set_style_line_color(pitch_plus_twenty_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(pitch_plus_twenty_line, 1, LV_PART_MAIN);

    // lv_obj_t *pitch_minus_ten_line = lv_line_create(attitude_indicator_pitch_container);
    // static lv_point_t pitch_minus_ten_line_points[] = {
    //     {30, 40},
    //     {50, 40}
    // };
    // lv_line_set_points(pitch_minus_ten_line, pitch_minus_ten_line_points, 2);
    // lv_obj_set_style_line_color(pitch_minus_ten_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(pitch_minus_ten_line, 1, LV_PART_MAIN);

    // lv_obj_t *pitch_minus_twenty_line = lv_line_create(attitude_indicator_pitch_container);
    // static lv_point_t pitch_minus_twenty_line_points[] = {
    //     {20, 50},
    //     {60, 50}
    // };
    // lv_line_set_points(pitch_minus_twenty_line, pitch_minus_twenty_line_points, 2);
    // lv_obj_set_style_line_color(pitch_minus_twenty_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(pitch_minus_twenty_line, 1, LV_PART_MAIN);

    // attitude_indicator_top_arc = lv_arc_create(attitude_indicator_roll_container);
    // lv_obj_set_size(attitude_indicator_top_arc, 112, 112);
    // lv_obj_align(attitude_indicator_top_arc, LV_ALIGN_TOP_MID, 0, 0);
    // lv_arc_set_bg_angles(attitude_indicator_top_arc, 180, 0);
    // lv_obj_set_style_pad_all(attitude_indicator_top_arc, 0, LV_PART_MAIN);
    // lv_obj_set_style_arc_color(attitude_indicator_top_arc,
    //                            lv_color_darken(lv_palette_main(LV_PALETTE_BLUE), 50),
    //                            LV_PART_MAIN);
    // lv_obj_set_style_arc_rounded(attitude_indicator_top_arc, 0, LV_PART_MAIN);
    // lv_obj_set_style_arc_width(attitude_indicator_top_arc, 12, LV_PART_MAIN);
    // lv_obj_remove_style(attitude_indicator_top_arc, NULL, LV_PART_KNOB);
    // lv_obj_remove_style(attitude_indicator_top_arc, NULL, LV_PART_INDICATOR);
    // lv_obj_clear_flag(attitude_indicator_top_arc, LV_OBJ_FLAG_CLICKABLE);

    // attitude_indicator_bottom_arc = lv_arc_create(attitude_indicator_roll_container);
    // lv_obj_set_size(attitude_indicator_bottom_arc, 112, 112);
    // lv_obj_align(attitude_indicator_bottom_arc, LV_ALIGN_BOTTOM_MID, 0, 0);
    // lv_arc_set_bg_angles(attitude_indicator_bottom_arc, 0, 180);
    // lv_obj_set_style_pad_all(attitude_indicator_bottom_arc, 0, LV_PART_MAIN);
    // lv_obj_set_style_arc_color(attitude_indicator_bottom_arc,
    //                            lv_color_darken(lv_palette_main(LV_PALETTE_BROWN), 50),
    //                            LV_PART_MAIN);
    // lv_obj_set_style_arc_rounded(attitude_indicator_bottom_arc, 0, LV_PART_MAIN);
    // lv_obj_set_style_arc_width(attitude_indicator_bottom_arc, 12, LV_PART_MAIN);
    // lv_obj_remove_style(attitude_indicator_bottom_arc, NULL, LV_PART_KNOB);
    // lv_obj_remove_style(attitude_indicator_bottom_arc, NULL, LV_PART_INDICATOR);
    // lv_obj_clear_flag(attitude_indicator_bottom_arc, LV_OBJ_FLAG_CLICKABLE);

    // lv_obj_t *overlay_left_center_line = lv_line_create(global_container);
    // static lv_point_t overlay_left_center_line_points[] = {
    //     {68, 58},
    //     {88, 58}
    // };
    // lv_line_set_points(overlay_left_center_line, overlay_left_center_line_points, 2);
    // lv_obj_set_style_line_color(overlay_left_center_line, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_left_center_line, 3, LV_PART_MAIN);

    // lv_obj_t *overlay_right_center_line = lv_line_create(global_container);
    // static lv_point_t overlay_right_center_line_points[] = {
    //     {28, 58},
    //     {48, 58}
    // };
    // lv_line_set_points(overlay_right_center_line, overlay_right_center_line_points, 2);
    // lv_obj_set_style_line_color(overlay_right_center_line, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_right_center_line, 3, LV_PART_MAIN);

    // lv_obj_t *overlay_pointer_triangle = lv_line_create(global_container);
    // static lv_point_t overlay_pointer_triangle_points[] = {
    //     {58, 16},
    //     {54, 29},
    //     {62, 29},
    //     {58, 16}
    // };
    // lv_line_set_points(overlay_pointer_triangle, overlay_pointer_triangle_points, 4);
    // lv_obj_set_style_line_color(overlay_pointer_triangle, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_pointer_triangle, 3, LV_PART_MAIN);
    // lv_obj_set_style_line_rounded(overlay_pointer_triangle, 1, LV_PART_MAIN);

    // lv_obj_t *overlay_zero_degree_roll_angle_line = lv_line_create(attitude_indicator_roll_container);
    // static lv_point_t overlay_zero_degree_roll_angle_line_points[] = {
    //     {56,  0},
    //     {56, 12}
    // };
    // lv_line_set_points(overlay_zero_degree_roll_angle_line, overlay_zero_degree_roll_angle_line_points, 2);
    // lv_obj_set_style_line_color(overlay_zero_degree_roll_angle_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_zero_degree_roll_angle_line, 3, LV_PART_MAIN);

    // lv_obj_t *overlay_plus_ten_degree_roll_angle_line = lv_line_create(attitude_indicator_roll_container);
    // static lv_point_t overlay_plus_ten_degree_roll_angle_line_points[] = {
    //     {66,  1},
    //     {63, 13}
    // };
    // lv_line_set_points(overlay_plus_ten_degree_roll_angle_line, overlay_plus_ten_degree_roll_angle_line_points, 2);
    // lv_obj_set_style_line_color(overlay_plus_ten_degree_roll_angle_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_plus_ten_degree_roll_angle_line, 1, LV_PART_MAIN);

    // lv_obj_t *overlay_plus_twenty_degree_roll_angle_line = lv_line_create(attitude_indicator_roll_container);
    // static lv_point_t overlay_plus_twenty_degree_roll_angle_line_points[] = {
    //     {75,  4},
    //     {70, 15}
    // };
    // lv_line_set_points(overlay_plus_twenty_degree_roll_angle_line,
    //                    overlay_plus_twenty_degree_roll_angle_line_points,
    //                    2);
    // lv_obj_set_style_line_color(overlay_plus_twenty_degree_roll_angle_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_plus_twenty_degree_roll_angle_line, 1, LV_PART_MAIN);

    // lv_obj_t *overlay_plus_thirty_degree_roll_angle_line = lv_line_create(attitude_indicator_roll_container);
    // static lv_point_t overlay_plus_thirty_degree_roll_angle_line_points[] = {
    //     {84,  8},
    //     {78, 18}
    // };
    // lv_line_set_points(overlay_plus_thirty_degree_roll_angle_line,
    //                    overlay_plus_thirty_degree_roll_angle_line_points,
    //                    2);
    // lv_obj_set_style_line_color(overlay_plus_thirty_degree_roll_angle_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_plus_thirty_degree_roll_angle_line, 3, LV_PART_MAIN);

    // lv_obj_t *overlay_plus_sixty_degree_roll_angle_line = lv_line_create(attitude_indicator_roll_container);
    // static lv_point_t overlay_plus_sixty_degree_roll_angle_line_points[] = {
    //     {105, 28},
    //     { 94, 34}
    // };
    // lv_line_set_points(overlay_plus_sixty_degree_roll_angle_line, overlay_plus_sixty_degree_roll_angle_line_points, 2);
    // lv_obj_set_style_line_color(overlay_plus_sixty_degree_roll_angle_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_plus_sixty_degree_roll_angle_line, 3, LV_PART_MAIN);

    // lv_obj_t *overlay_minus_ten_degree_roll_angle_line = lv_line_create(attitude_indicator_roll_container);
    // static lv_point_t overlay_minus_ten_degree_roll_angle_line_points[] = {
    //     {46,  1},
    //     {48, 12}
    // };
    // lv_line_set_points(overlay_minus_ten_degree_roll_angle_line, overlay_minus_ten_degree_roll_angle_line_points, 2);
    // lv_obj_set_style_line_color(overlay_minus_ten_degree_roll_angle_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_minus_ten_degree_roll_angle_line, 1, LV_PART_MAIN);

    // lv_obj_t *overlay_minus_twenty_degree_roll_angle_line = lv_line_create(attitude_indicator_roll_container);
    // static lv_point_t overlay_minus_twenty_degree_roll_angle_line_points[] = {
    //     {36,  4},
    //     {41, 14}
    // };
    // lv_line_set_points(overlay_minus_twenty_degree_roll_angle_line,
    //                    overlay_minus_twenty_degree_roll_angle_line_points,
    //                    2);
    // lv_obj_set_style_line_color(overlay_minus_twenty_degree_roll_angle_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_minus_twenty_degree_roll_angle_line, 1, LV_PART_MAIN);

    // lv_obj_t *overlay_minus_thirty_degree_roll_angle_line = lv_line_create(attitude_indicator_roll_container);
    // static lv_point_t overlay_minus_thirty_degree_roll_angle_line_points[] = {
    //     {28,  8},
    //     {34, 18}
    // };
    // lv_line_set_points(overlay_minus_thirty_degree_roll_angle_line,
    //                    overlay_minus_thirty_degree_roll_angle_line_points,
    //                    2);
    // lv_obj_set_style_line_color(overlay_minus_thirty_degree_roll_angle_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_minus_thirty_degree_roll_angle_line, 3, LV_PART_MAIN);

    // lv_obj_t *overlay_minus_sixty_degree_roll_angle_line = lv_line_create(attitude_indicator_roll_container);
    // static lv_point_t overlay_minus_sixty_degree_roll_angle_line_points[] = {
    //     { 8, 28},
    //     {18, 34}
    // };
    // lv_line_set_points(overlay_minus_sixty_degree_roll_angle_line,
    //                    overlay_minus_sixty_degree_roll_angle_line_points,
    //                    2);
    // lv_obj_set_style_line_color(overlay_minus_sixty_degree_roll_angle_line, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_line_width(overlay_minus_sixty_degree_roll_angle_line, 3, LV_PART_MAIN);

    // Version 2 - Low ram usage
    attitude_meter = lv_meter_create(lv_scr_act());
    lv_obj_set_size(attitude_meter, 116, 116);
    lv_obj_align(attitude_meter, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(attitude_meter, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(attitude_meter, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_color(attitude_meter, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_PART_MAIN);
    lv_obj_set_style_text_color(attitude_meter, lv_color_white(), LV_PART_TICKS);

    lv_meter_scale_t *roll_scale = lv_meter_add_scale(attitude_meter);
    lv_meter_set_scale_range(attitude_meter, roll_scale, -60, 60, 120, 210);
    lv_meter_set_scale_ticks(attitude_meter, roll_scale, 13, 1, 5, lv_color_white());
    lv_meter_set_scale_major_ticks(attitude_meter, roll_scale, 3, 2, 9, lv_color_white(), 11);

    lv_meter_scale_t *pitch_scale = lv_meter_add_scale(attitude_meter);
    lv_meter_set_scale_range(attitude_meter, pitch_scale, -20, 20, 120, 30);
    lv_meter_set_scale_ticks(attitude_meter, pitch_scale, 9, 1, 5, lv_color_white());
    lv_meter_set_scale_major_ticks(attitude_meter, pitch_scale, 2, 2, 9, lv_color_white(), 11);

    lv_meter_indicator_t *indicator;

    indicator = lv_meter_add_arc(attitude_meter, pitch_scale, 10, lv_palette_main(LV_PALETTE_BLUE), 1);
    lv_meter_set_indicator_start_value(attitude_meter, indicator, 0);
    lv_meter_set_indicator_end_value(attitude_meter, indicator, 20);

    indicator = lv_meter_add_arc(attitude_meter, pitch_scale, 10, lv_palette_main(LV_PALETTE_BROWN), 1);
    lv_meter_set_indicator_start_value(attitude_meter, indicator, -20);
    lv_meter_set_indicator_end_value(attitude_meter, indicator, 0);

    indicator = lv_meter_add_arc(attitude_meter, roll_scale, 10, lv_palette_main(LV_PALETTE_BLUE), 1);
    lv_meter_set_indicator_start_value(attitude_meter, indicator, -60);
    lv_meter_set_indicator_end_value(attitude_meter, indicator, 60);

    lv_obj_t *instrument_label = lv_label_create(attitude_meter);
    lv_label_set_text(instrument_label, "ATT");
    lv_obj_align(instrument_label, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_obj_set_style_text_font(instrument_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(instrument_label, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_PART_MAIN);

    lv_obj_t *roll_label = lv_label_create(attitude_meter);
    lv_label_set_text(roll_label, "Roll");
    lv_obj_align(roll_label, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_font(roll_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(roll_label, lv_color_white(), LV_PART_MAIN);

    lv_obj_t *pitch_label = lv_label_create(attitude_meter);
    lv_label_set_text(pitch_label, "Pitch");
    lv_obj_align(pitch_label, LV_ALIGN_CENTER, 0, 17);
    lv_obj_set_style_text_font(pitch_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(pitch_label, lv_color_white(), LV_PART_MAIN);

    attitude_roll_needle = lv_meter_add_needle_line(attitude_meter, roll_scale, 2, lv_color_white(), -10);
    attitude_pitch_needle = lv_meter_add_needle_line(attitude_meter, pitch_scale, 2, lv_color_white(), -10);
}

void heading_indicator(void)
{
    heading_meter = lv_meter_create(lv_scr_act());
    lv_obj_set_size(heading_meter, 116, 116);
    lv_obj_align(heading_meter, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_pad_all(heading_meter, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(heading_meter, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_color(heading_meter, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);
    lv_obj_set_style_text_color(heading_meter, lv_color_white(), LV_PART_TICKS);
    lv_obj_set_style_transform_pivot_x(heading_meter, 57, 0);
    lv_obj_set_style_transform_pivot_y(heading_meter, 57, 0);

    lv_meter_scale_t *scale = lv_meter_add_scale(heading_meter);
    lv_meter_set_scale_range(heading_meter, scale, 0, 359, 360, 270);

    lv_meter_scale_t *scale_visual = lv_meter_add_scale(heading_meter);
    lv_meter_set_scale_range(heading_meter, scale_visual, 0, 35, 350, 270);
    lv_meter_set_scale_ticks(heading_meter, scale_visual, 36, 1, 4, lv_color_white());
    lv_meter_set_scale_major_ticks(heading_meter, scale_visual, 3, 2, 5, lv_color_white(), 11);

    lv_obj_t *label = lv_label_create(heading_meter);
    lv_label_set_text(label, "HDG");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 16);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);

    heading_needle = lv_meter_add_needle_line(heading_meter, scale, 2, lv_palette_main(LV_PALETTE_ORANGE), -10);
}

void vertical_speed_indicator(void)
{
    vertical_speed_indicator_meter = lv_meter_create(lv_scr_act());
    lv_obj_set_size(vertical_speed_indicator_meter, 116, 116);
    lv_obj_align(vertical_speed_indicator_meter, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_set_style_pad_all(vertical_speed_indicator_meter, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(vertical_speed_indicator_meter, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_color(vertical_speed_indicator_meter, lv_palette_main(LV_PALETTE_CYAN), LV_PART_MAIN);
    lv_obj_set_style_text_color(vertical_speed_indicator_meter, lv_color_white(), LV_PART_TICKS);

    lv_meter_scale_t *scale = lv_meter_add_scale(vertical_speed_indicator_meter);
    lv_meter_set_scale_range(vertical_speed_indicator_meter, scale, -2000, 2000, 300, 30);

    lv_meter_scale_t *scale_visual = lv_meter_add_scale(vertical_speed_indicator_meter);
    lv_meter_set_scale_range(vertical_speed_indicator_meter, scale_visual, -20, 20, 300, 30);
    lv_meter_set_scale_ticks(vertical_speed_indicator_meter, scale_visual, 41, 1, 5, lv_color_white());
    lv_meter_set_scale_major_ticks(vertical_speed_indicator_meter, scale_visual, 5, 2, 9, lv_color_white(), 11);

    lv_obj_t *label = lv_label_create(vertical_speed_indicator_meter);
    lv_label_set_text(label, "VSI");
    lv_obj_align(label, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_CYAN), LV_PART_MAIN);

    vertical_speed_indicator_needle =
        lv_meter_add_needle_line(vertical_speed_indicator_meter, scale, 2, lv_palette_main(LV_PALETTE_CYAN), -10);
}
