#ifndef __PORT_INIT_H
#define __PORT_INIT_H

int port_init(void);
int port_deinit(void);
void MySPI_Start(void);
void MySPI_Stop(void);  //ÄÚ²¿ÊÇMySPI_W_SS(0)

void MySPI_W_SCK(uint8_t BitValue);
void MySPI_W_RESET(uint8_t BitValue);

void MySPI_W_MOSI1(uint8_t BitValue);
void MySPI_W_MOSI2(uint8_t BitValue);
void MySPI_W_MOSI3(uint8_t BitValue);
void MySPI_W_MOSI4(uint8_t BitValue);
void MySPI_W_MOSI5(uint8_t BitValue);
void MySPI_W_MOSI6(uint8_t BitValue);

uint16_t read_feedback_value(void);
#endif
