#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "image.h"
#include "acme_5_outlines_font.h"
#include "bubblesstandard_font.h"
#include "crackers_font.h"
#include "BMSPA_font.h"

//SPI for 8xBME688 chips.
#define SPI_PORT    spi0
#define PIN_SCK     2
#define PIN_MOSI    3
#define PIN_MISO    4
#define PIN_CS      5

// I2C bus for Display, BME688 CS expander and BME688 RTC
#define I2C_0_PORT  i2c0
#define I2C_0_SDA   8
#define I2C_0_SCL   9

// I2C bus for both MCP4728 DACs
#define I2C_1_PORT  i2c1
#define I2C_1_SDA   19
#define I2C_1_SCL   18

// DAC I2C addresses
#define MCP_0_ADDR  0x01
#define MCP_1_ADDR  0x02

// I2C MCP4728 Load Analog pins
#define LDAC_0      21
#define LDAC_1      20
#define RDY_0       17
#define RDY_1       18

// BME CS expander output states
#define EXP_BME_0   0x01
#define EXP_BME_1   0x02
#define EXP_BME_2   0x04
#define EXP_BME_3   0x08
#define EXP_BME_4   0x10
#define EXP_BME_5   0x20
#define EXP_BME_6   0x40
#define EXP_BME_7   0x80

#define SLEEPTIME 25
const uint8_t num_chars_per_disp[]={7,7,7,5};
const uint8_t *fonts[4]= {acme_font, bubblesstandard_font, crackers_font, BMSPA_font};


void animation(ssd1306_t* disp) {
    const char *words[]= {"SSD1306", "DISPLAY", "DRIVER"};

    ssd1306_clear(disp);

    printf("ANIMATION!\n");

    char buf[8];

    for(;;) {
        for(int y=0; y<31; ++y) {
            ssd1306_draw_line(disp, 0, y, 127, y);
            ssd1306_show(disp);
            sleep_ms(SLEEPTIME);
            ssd1306_clear(disp);
        }

        for(int y=0, i=1; y>=0; y+=i) {
            ssd1306_draw_line(disp, 0, 31-y, 127, 31+y);
            ssd1306_draw_line(disp, 0, 31+y, 127, 31-y);
            ssd1306_show(disp);
            sleep_ms(SLEEPTIME);
            ssd1306_clear(disp);
            if(y==32) i=-1;
        }

        for(int i=0; i<sizeof(words)/sizeof(char *); ++i) {
            ssd1306_draw_string(disp, 8, 24, 2, words[i]);
            ssd1306_show(disp);
            sleep_ms(800);
            ssd1306_clear(disp);
        }

        for(int y=31; y<63; ++y) {
            ssd1306_draw_line(disp, 0, y, 127, y);
            ssd1306_show(disp);
            sleep_ms(SLEEPTIME);
            ssd1306_clear(disp);
        }

        for(size_t font_i=0; font_i<sizeof(fonts)/sizeof(fonts[0]); ++font_i) {
            uint8_t c=32;
            while(c<=126) {
                uint8_t i=0;
                for(; i<num_chars_per_disp[font_i]; ++i) {
                    if(c>126)
                        break;
                    buf[i]=c++;
                }
                buf[i]=0;

                ssd1306_draw_string_with_font(disp, 8, 24, 2, fonts[font_i], buf);
                ssd1306_show(disp);
                sleep_ms(800);
                ssd1306_clear(disp);
            }
        }

        ssd1306_bmp_show_image(disp, image_data, image_size);
        ssd1306_show(disp);
        sleep_ms(2000);
    }
}

int main()
{
    sleep_ms(2000);
    stdio_init_all();

    // SPI initialisation
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // GPIO setup
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_set_dir(LDAC_0, GPIO_OUT);
    gpio_set_dir(LDAC_1, GPIO_OUT);
    gpio_set_dir(RDY_0, GPIO_OUT);
    gpio_set_dir(RDY_1, GPIO_OUT);
    

    // I2C 0 Initialisation
    i2c_init(I2C_0_PORT, 400*1000);
    gpio_set_function(I2C_0_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_0_SCL, GPIO_FUNC_I2C);
    //gpio_pull_up(I2C_0_SDA);
    //gpio_pull_up(I2C_0_SCL);

    // I2C 1 Initialisation
    i2c_init(I2C_1_PORT, 400*1000);
    gpio_set_function(I2C_1_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_1_SCL, GPIO_FUNC_I2C);
    //gpio_pull_up(I2C_1_SDA);
    //gpio_pull_up(I2C_1_SCL);

    // Initialize display and write test to it.
    ssd1306_t disp;
    disp.external_vcc=true;
    ssd1306_init(&disp, 128, 64, 0x7A, I2C_0_PORT);
    //ssd1306_clear(&disp);
    //ssd1306_bmp_show_image(&disp, image_data, image_size);
    //ssd1306_show(&disp);
    sleep_ms(2000);
    animation(&disp);



    return 0;
}
