/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-17     Meco Man      First version
 */
#include <lvgl.h>

lv_obj_t * ui_Screen1;
lv_obj_t * ui_Label1;
void lv_user_gui_init(void)
{
    lv_disp_t * dispp = lv_display_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label1 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label1, -194);
    lv_obj_set_y(ui_Label1, -73);
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "Hello world");
    lv_disp_load_scr(ui_Screen1);

    /*lv_obj_t * parent_obj = lv_obj_create(NULL);*/
    /*lv_obj_remove_flag(parent_obj, LV_OBJ_FLAG_SCROLLABLE);      /// Flags*/
    /*lv_obj_t * dsc = lv_label_create(parent_obj);*/
    /*lv_label_set_text(dsc, "The average FPS is");*/
    /*lv_obj_align(dsc, LV_ALIGN_TOP_MID, 0, 200);*/
    /*lv_disp_load_scr(parent_obj);*/
}
