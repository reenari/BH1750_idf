//
// Created by Ari Reen on 19/03/2021.
//
#include <esp_log.h>
#include <esp_sleep.h>
#include <driver/i2c.h>

#include "include/bh1750_idf.h"

static const char *TAG = "BH1750";

// 5C
// 23

struct bh1750_dev {
    i2c_port_t i2c_port;
    uint8_t dev_addr;
    enum bh1750idf_mode mode;
    int64_t mode_changed_time;
    uint8_t mt_reg;
};

struct bh1750_dev *bh1750idf_init(i2c_port_t i2c_port) {

    struct bh1750_dev *bh1750 = malloc(sizeof(struct bh1750_dev));
    bh1750->i2c_port = i2c_port;
    bh1750->dev_addr = 0x23;
    bh1750->mode = 0;
    bh1750->mode_changed_time = 0;
    bh1750->mt_reg = BH1750IDF_MT_REG_DEFAULT;
   // bh1750idf_read_(bh1750);

    return bh1750;
}


void bh1750idf_set_mode(struct bh1750_dev *bh1750, enum bh1750idf_mode mode, uint8_t measurement_time) {

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (bh1750->dev_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, mode, true);
    i2c_master_stop(cmd);
    esp_err_t  err = i2c_master_cmd_begin(bh1750->i2c_port, cmd, 100/portTICK_PERIOD_MS);

    i2c_cmd_link_delete(cmd);

    if (err != ESP_OK) {
        ESP_LOGE(TAG,"i2c write fail %d", err);
    }

    bh1750->mode_changed_time = esp_timer_get_time();
    bh1750->mode = mode;
    

}

float bh1750idf_read_(struct bh1750_dev *bh1750) {

    uint8_t high;
    uint8_t low;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (bh1750->dev_addr << 1) | I2C_MASTER_READ, true);

    i2c_master_read_byte(cmd, &high, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &low, I2C_MASTER_NACK);
    i2c_master_stop(cmd);

    esp_err_t  err = i2c_master_cmd_begin(bh1750->i2c_port, cmd, 100/portTICK_PERIOD_MS);

    i2c_cmd_link_delete(cmd);

    if (err != ESP_OK) {
        ESP_LOGE(TAG,"i2c write fail %d", err);
    }

    uint16_t level = high<<8 | low;

    float conv = 1.2 * BH1750IDF_MT_REG_DEFAULT / bh1750->mt_reg;
    float lux = level * conv;
    ESP_LOGI(TAG, "raw %d, lux %f\n ", level, lux);

    return lux;
}
