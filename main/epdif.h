#ifndef EPDIF_H
#define EPDIF_H

#include <inttypes.h>

#define PIN_NUM_RST 14
#define PIN_NUM_DC 12
#define PIN_NUM_BUSY 13

int EpdIf_IfInit(void);
void SpiTransfer(uint8_t toSend);

#endif