//
// Created by Ari Reen on 19/03/2021.
//

#ifndef BH1750_IDF_H
#define BH1750_IDF_H

struct bh1750_dev;
enum bh1750idf_mode {
    CONTINUOUS_HIGH_RES_MODE = 0x10, // Measurement at 1 lux resolution. Typical measurement time is 120ms
    CONTINUOUS_HIGH_RES_MODE_2 = 0x11, // Measurement at 0.5 lux resolution. Typical measurement time is 120ms.
    CONTINUOUS_LOW_RES_MODE = 0x13, // Measurement at 4 lux resolution. Typical measurement time is 16ms.
    ONE_TIME_HIGH_RES_MODE = 0x20, // Measurement at 1 lux resolution. Typical measurement time is 120ms.
    ONE_TIME_HIGH_RES_MODE_2 = 0x21, // Measurement at 0.5 lux resolution. Typical measurement time is 120ms.
    ONE_TIME_LOW_RES_MODE = 0x23 // Measurement at 4 lux resolution. Typical measurement time is 16ms.
};
#define BH1750IDF_MT_REG_MIN 31
#define BH1750IDF_MT_REG_MAX 254
#define BH1750IDF_MT_REG_DEFAULT 69

struct bh1750_dev *bh1750idf_init(i2c_port_t i2c_port);
void bh1750idf_set_mode(struct bh1750_dev *bh1750, enum bh1750idf_mode mode, uint8_t measurement_time);
float bh1750idf_read_(struct bh1750_dev *bh1750);



#endif //BH1750_IDF_H
