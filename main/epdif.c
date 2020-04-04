#include "epdif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#include <string.h>
#include <stdlib.h>

#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 5

#define RST_PIN PIN_NUM_RST 
#define DC_PIN PIN_NUM_DC
#define BUSY_PIN PIN_NUM_BUSY

#define GPIO_OUTPUT_PIN_SEL ((1ULL << RST_PIN) | (1ULL << DC_PIN))
#define GPIO_INPUT_PIN_SEL (1ULL << BUSY_PIN)

#define SPI_CLOCK_SPEED 2000000

static spi_device_handle_t spi;

int EpdIf_IfInit(void){
    gpio_config_t iocfg;
    iocfg.intr_type = GPIO_PIN_INTR_DISABLE;
    iocfg.mode = GPIO_MODE_OUTPUT;
    iocfg.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    iocfg.pull_down_en = 0;
    iocfg.pull_up_en = 0;
    gpio_config(&iocfg);
    
    iocfg.intr_type = GPIO_PIN_INTR_DISABLE;
    iocfg.mode = GPIO_MODE_INPUT;
    iocfg.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    iocfg.pull_down_en = 0;
    iocfg.pull_up_en = 0;
    gpio_config(&iocfg);

    esp_err_t ret;

    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32
    };

    spi_device_interface_config_t devcfg = {
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .clock_speed_hz = SPI_CLOCK_SPEED,
        .queue_size = 7
    };
    ret = spi_bus_initialize(VSPI_HOST, &buscfg, 2);
    ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(VSPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
    return 0;
}

void SpiTransfer(uint8_t toSend){
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &toSend;
    t.user = (void *) 0;
    ret = spi_device_polling_transmit(spi, &t);
    assert(ret == ESP_OK);
}