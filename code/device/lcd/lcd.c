#include "lcd.h"

void lcd_init()
{
    tft180_set_dir(TFT180_CROSSWISE_180);
    tft180_set_color(DEFAULT_PEN_COLOR, DEFAULT_BACKGROUND_COLOR);
    tft180_init();
}

void lcd_clear()
{
    tft180_clear();
}

void lcd_show_string(vuint16 x, vuint16 y, const char *dat)
{
    tft180_show_string(x * CHAR_WIDTH, y * CHAR_HEIGHT, dat);
}

void lcd_show_string_color(vuint16 x,
                           vuint16 y,
                           const char *dat,
                           vuint16 pen_color,
                           vuint16 background_color)
{
    tft180_set_color(pen_color, background_color);
    lcd_show_string(x, y, dat);
    tft180_set_color(DEFAULT_PEN_COLOR, DEFAULT_BACKGROUND_COLOR);
}
void lcd_show_uint(vuint16 x, vuint16 y, vuint32 dat, vuint8 num)
{
    tft180_show_uint(x * CHAR_WIDTH, y * CHAR_HEIGHT, dat, num);
}

void lcd_show_uint_color(vuint16 x,
                         vuint16 y,
                         vuint32 dat,
                         vuint8 num,
                         vuint16 pen_color,
                         vuint16 background_color)
{
    tft180_set_color(pen_color, background_color);
    lcd_show_uint(x, y, dat, num);
    tft180_set_color(DEFAULT_PEN_COLOR, DEFAULT_BACKGROUND_COLOR);
}

void lcd_show_int(vuint16 x, vuint16 y, int32 dat, vuint8 num)
{
    tft180_show_int(x * CHAR_WIDTH, y * CHAR_HEIGHT, dat, num);
}

void lcd_show_int_color(vuint16 x,
                        vuint16 y,
                        int32 dat,
                        vuint8 num,
                        vuint16 pen_color,
                        vuint16 background_color)
{
    tft180_set_color(pen_color, background_color);
    lcd_show_int(x, y, dat, num);
    tft180_set_color(DEFAULT_PEN_COLOR, DEFAULT_BACKGROUND_COLOR);
}

void lcd_show_float(vuint16 x,
                    vuint16 y,
                    const double dat,
                    vuint8 num,
                    vuint8 pointnum)
{
    tft180_show_float(x * CHAR_WIDTH, y * CHAR_HEIGHT, dat, num, pointnum);
}

void lcd_show_float_color(vuint16 x,
                          vuint16 y,
                          const double dat,
                          vuint8 num,
                          vuint8 pointnum,
                          vuint16 pen_color,
                          vuint16 background_color)
{
    tft180_set_color(pen_color, background_color);
    lcd_show_float(x, y, dat, num, pointnum);
    tft180_set_color(DEFAULT_PEN_COLOR, DEFAULT_BACKGROUND_COLOR);
}

void lcd_show_char(vuint16 x, vuint16 y, char dat)
{
    tft180_show_char(x * CHAR_WIDTH, y * CHAR_HEIGHT, dat);
}

void lcd_show_char_color(vuint16 x,
                         vuint16 y,
                         char dat,
                         vuint16 pen_color,
                         vuint16 background_color)
{
    tft180_set_color(pen_color, background_color);
    lcd_show_char(x, y, dat);
    tft180_set_color(DEFAULT_PEN_COLOR, DEFAULT_BACKGROUND_COLOR);
}