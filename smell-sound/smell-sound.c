#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"
#include "image.h"

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


int main()
{
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
    gpio_pull_up(I2C_0_SDA);
    gpio_pull_up(I2C_0_SCL);

    // I2C 1 Initialisation
    i2c_init(I2C_1_PORT, 400*1000);
    gpio_set_function(I2C_1_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_1_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_1_SDA);
    gpio_pull_up(I2C_1_SCL);

    // Initialize display and write test to it.
    ssd1306_t disp;
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x78, i2c0);
    ssd1306_clear(&disp);
    ssd1306_bmp_show_image(&disp, image_data, image_size);
    ssd1306_show(&disp);
    sleep_ms(2000);


    return 0;
}
