/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-18     Meco Man     The first version
 */
#include <lvgl.h>
#include <drv_clcd.h>

static rt_device_t lcd_device = 0;
static struct rt_device_graphic_info info;

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void lcd_fb_flush(lv_display_t *display, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t width = lv_area_get_width(area);
    uint32_t height = lv_area_get_height(area);

    /* 优化: 直接获取16位颜色指针 */
    lv_color16_t *fb16 = (lv_color16_t *)info.framebuffer;
    lv_color16_t *color16 = (lv_color16_t *)px_map;

    /* 计算目标位置的起始偏移 */
    uint32_t fb_offset = area->y1 * info.width + area->x1;

    /* 逐行复制优化 */
    for (uint32_t y = 0; y < height; y++) {
        rt_memcpy(&fb16[fb_offset], color16, width * sizeof(lv_color16_t));
        fb_offset += info.width;
        color16 += width;
    }
    lv_display_flush_ready(display);
}

void lv_port_disp_init(void)
{
    rt_err_t result;
    lv_color_t *fbuf1, *fbuf2;

    lcd_device = rt_device_find("lcd");
    if (lcd_device == 0)
    {
        rt_kprintf("error!\n");
        return;
    }
    result = rt_device_open(lcd_device, 0);
    if (result != RT_EOK)
    {
        rt_kprintf("error!\n");
        return;
    }
    /* get framebuffer address */
    result = rt_device_control(lcd_device, RTGRAPHIC_CTRL_GET_INFO, &info);
    if (result != RT_EOK)
    {
        rt_kprintf("error!\n");
        /* get device information failed */
        return;
    }

    RT_ASSERT(info.bits_per_pixel == 8 || info.bits_per_pixel == 16 ||
              info.bits_per_pixel == 24 || info.bits_per_pixel == 32);

    lv_display_t *display = lv_display_create(info.width, info.height);
    lv_color_format_t cf = lv_display_get_color_format(display);
    uint32_t px_size = lv_color_format_get_size(cf);
    size_t buf_size = info.width * info.height * px_size;

    fbuf1 = rt_malloc(buf_size);
    if (fbuf1 == RT_NULL)
    {
        rt_kprintf("Error: alloc disp buf fail\n");
        return;
    }

    fbuf2 = rt_malloc(buf_size);
    if (fbuf2 == RT_NULL)
    {
        rt_kprintf("Error: alloc disp buf fail\n");
        rt_free(fbuf1);
        return;
    }

    lv_display_set_flush_cb(display, lcd_fb_flush);
    lv_display_set_buffers(display, fbuf1, fbuf2, buf_size, LV_DISPLAY_RENDER_MODE_DIRECT);
}
