#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gpiod.h>


struct gpiod_chip *chip5;      // GPIO控制器句柄
struct gpiod_line *line[8];      // GPIO引脚句柄

void MySPI_W_SS(uint8_t BitValue) {
    gpiod_line_set_value(line[7], BitValue);
}

void MySPI_W_SCK(uint8_t BitValue) {
    gpiod_line_set_value(line[6], BitValue);
}


void MySPI_W_MOSI1(uint8_t BitValue) {
    gpiod_line_set_value(line[0], BitValue);
}

void MySPI_W_MOSI2(uint8_t BitValue) {
    gpiod_line_set_value(line[1], BitValue);
}

void MySPI_W_MOSI3(uint8_t BitValue) {
    gpiod_line_set_value(line[2], BitValue);
}

void MySPI_W_MOSI4(uint8_t BitValue) {
    gpiod_line_set_value(line[3], BitValue);
}

void MySPI_W_MOSI5(uint8_t BitValue) {
    gpiod_line_set_value(line[4], BitValue);
}

void MySPI_W_MOSI6(uint8_t BitValue) {
    gpiod_line_set_value(line[5], BitValue);
}

void MySPI_Start(void) {
    MySPI_W_SS(0);
}

void MySPI_Stop(void) {
    MySPI_W_SS(1);
}

                                                    /************************/
                                                    /*   引脚初始化部分     */
                                                    /************************/
int port_init(void) {
    int ret;

    // 获取GPIO控制器
    chip5 = gpiod_chip_open("/dev/gpiochip4");
    if (chip5 == NULL) {
        perror("gpiod_chip_open error\n");
        gpiod_chip_close(chip5);
        return -1;
    }
    
    // 获取GPIO引脚
    int pin_numbers[8] = {16, 17, 10, 11, 20, 21, 4, 3};
    for (int i = 0; i < 8; i++) {
        line[i] = gpiod_chip_get_line(chip5, pin_numbers[i]);
        
        
        if (line[i] == NULL) {
            printf("gpiod_chip_get_line error for pin %d\n", pin_numbers[i]);
              gpiod_line_release(line[i]);
            }
        }
    

    // 设置GPIO为输出模式
    for (int i = 0; i < 8; i++) {
        ret = gpiod_line_request_output(line[i], "gpio", 0);
        if (ret < 0) {
            printf("gpiod_line_request_output error for pin %d\n", pin_numbers[i]);
            
            for (int i = 0; i < 8; i++) 
            {
              gpiod_line_release(line[i]);
            }
        }
    }
    
    
        /*初始化默认电平*/
    MySPI_W_SS(1);   // SS初始高电平
    MySPI_W_SCK(1);  // SCK初始高电平
    MySPI_W_MOSI1(0);
    MySPI_W_MOSI2(0);
    MySPI_W_MOSI3(0);
    MySPI_W_MOSI4(0);
    MySPI_W_MOSI5(0);
    MySPI_W_MOSI6(0);


}


int port_deinit(void)
{

  for(int i=0; i<8; i++) 
  {
    gpiod_line_release(line[i]);
  }
  
  gpiod_chip_close(chip5);
  
}





uint16_t MySPI_SwapWord_MOSI1(uint16_t TxData1, uint16_t TxData2, uint16_t TxData3,
                              uint16_t TxData4, uint16_t TxData5, uint16_t TxData6) {
    uint16_t RxData = 0;
    printf("Transmit Start!!\n");

    for (uint8_t i = 0; i < 16; i++) {
        MySPI_W_SCK(0);

        MySPI_W_MOSI1((TxData1 & (0x8000 >> i)) ? 1 : 0);
        printf("DATA1 : %d\n",((TxData1 & (0x8000 >> i)) ? 1 : 0));
        MySPI_W_MOSI2((TxData2 & (0x8000 >> i)) ? 1 : 0);
        printf("      DATA2 : %d\n",((TxData2 & (0x8000 >> i)) ? 1 : 0));      
        MySPI_W_MOSI3((TxData3 & (0x8000 >> i)) ? 1 : 0);
        printf("            DATA3 : %d\n",((TxData3 & (0x8000 >> i)) ? 1 : 0));
        MySPI_W_MOSI4((TxData4 & (0x8000 >> i)) ? 1 : 0);
        printf("                  DATA4 : %d\n",((TxData4 & (0x8000 >> i)) ? 1 : 0));
        MySPI_W_MOSI5((TxData5 & (0x8000 >> i)) ? 1 : 0);
        //printf("DATA5 : %d\n",((TxData2 & (0x8000 >> i)) ? 1 : 0));
        MySPI_W_MOSI6((TxData6 & (0x8000 >> i)) ? 1 : 0);
        //printf("DATA6 : %d\n",((TxData6 & (0x8000 >> i)) ? 1 : 0));
        MySPI_W_SCK(1);
        
        usleep(100000);  //延时1s
        // if(MySPI_R_MISO()) RxData |= (0x8000 >> i);
    }
    

    printf("Transmit Stop!!\n");
    

    return RxData;
}


                                                                          /**********************/
                                                                          /*********驱动函数*******/
                                                                          /**********************/

uint16_t transmit_drive(uint16_t rx_data) {

    printf("enter transmit_drive()\n");
    //static uint8_t count_rounds = 0;
    static uint8_t count_prt = 0;
    bool state = 0; 
    uint16_t data[6];
    
    data[count_prt++] = rx_data;
    printf("count_prt = %d\n",count_prt);
    
    if(count_prt == 6)
    {
      printf("--------------------------\n");
      printf("data1 = %d\n",data[0]);
      printf("data2 = %d\n",data[1]);
      printf("data3 = %d\n",data[2]);
      printf("data4 = %d\n",data[3]);
      printf("data5 = %d\n",data[4]);
      printf("data6 = %d\n",data[5]);
      printf("--------------------------\n");
      printf("MySPI_Start!!\n");
      MySPI_Start();      //有问题
      printf("MySPI_SwapWord_MOSI1 !!\n");
      MySPI_SwapWord_MOSI1 (data[0],data[1],data[2],data[3],data[4],data[5]);  //每轮数据的每个数据通过每条线传输
      
      MySPI_Stop();
    
      printf("MySPI_SwapWord_MOSI1 FINISH !!\n");
      
      MySPI_W_MOSI1(0);    //将所有数据线置0
      MySPI_W_MOSI2(0);
      MySPI_W_MOSI3(0);
      MySPI_W_MOSI4(0);
      MySPI_W_MOSI5(0);
      MySPI_W_MOSI6(0);
      
      usleep(2000000);    //在每次发送数据之间延时2秒，体现在片选线上
      count_prt = 0;
    }
    
      
      
      return 0;
}

uint16_t send(uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4, uint16_t data5, uint16_t data6, uint8_t type_of_transmit) //传输数据和传输参数用同一个函数接口
{

  uint8_t state = type_of_transmit;
  uint16_t data[6];  //data是一轮数据
  data[0] = data1;
  data[1] = data2;
  data[2] = data3;
  data[3] = data4;
  data[4] = data5;
  data[5] = data6;
  
  printf("--------------------------\n");
  printf("data1 = %d\n",data[0]);
  printf("data2 = %d\n",data[1]);
  printf("data3 = %d\n",data[2]);
  printf("data4 = %d\n",data[3]);
  printf("data5 = %d\n",data[4]);
  printf("data6 = %d\n",data[5]);
  printf("--------------------------\n");
  
  if(state == 0)
  {
    /*参数*/
    printf("state = %d\n",state);
    for(int i=0; i<6; i++)
    {
      transmit_drive(data[i]); //传输一轮数据
    }
    printf("Finish Sending One Round!!\n");
  }
  else if(state == 1)
  {
    /*数据*/
    printf("state = %d\n",state);
    
  }
  else 
  {
    printf("The type error!\n");
  }
  
  return 0;
}


int main(int argc, char *argv[])
{
      port_init();
      printf("Test Start!!\n");

      send(1,10,100,1000,500,5000,0);
      printf("Test Finish!!\n");
      
      usleep(2000000);
      
      printf("Test Start!!\n");
      send(1,10,100,1000,10000,2,0);
      printf("Test Finish!!\n");
      
      port_deinit();
      printf("port_deinit !!\n");
}


