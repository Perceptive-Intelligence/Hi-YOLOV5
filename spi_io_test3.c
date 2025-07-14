/*io������ͷ�ļ�*/
#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
/*��������ͷ�ļ�*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <errno.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>

struct gpiod_chip *chip2;      // GPIO���������
struct gpiod_chip *chip5;      // GPIO���������
struct gpiod_line *line[8];      // GPIO���ž��


const char * path = "/dev/input/by-path/platform-gpio-keys-event";

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

uint16_t MySPI_SwapWord_MOSI1(uint16_t TxData1, uint16_t TxData2, uint16_t TxData3,
                              uint16_t TxData4, uint16_t TxData5, uint16_t TxData6) {
    uint16_t RxData = 0;

    printf("Transmit Start!!\n");

    for (uint8_t i = 0; i < 16; i++) {
        MySPI_W_SCK(0);

        MySPI_W_MOSI1((TxData1 & (0x8000 >> i)) ? 1 : 0);
        //printf("DATA1 : %d\n",((TxData1 & (0x8000 >> i)) ? 1 : 0));
        MySPI_W_MOSI2((TxData2 & (0x8000 >> i)) ? 1 : 0);
        //printf("DATA2 : %d\n",((TxData2 & (0x8000 >> i)) ? 1 : 0));      
        MySPI_W_MOSI3((TxData3 & (0x8000 >> i)) ? 1 : 0);
        printf("DATA3 : %d\n",((TxData3 & (0x8000 >> i)) ? 1 : 0));  
        MySPI_W_MOSI4((TxData4 & (0x8000 >> i)) ? 1 : 0);
        //printf("DATA4 : %d\n",((TxData4 & (0x8000 >> i)) ? 1 : 0));
        MySPI_W_MOSI5((TxData5 & (0x8000 >> i)) ? 1 : 0);
        //printf("DATA5 : %d\n",((TxData2 & (0x8000 >> i)) ? 1 : 0));
        MySPI_W_MOSI6((TxData6 & (0x8000 >> i)) ? 1 : 0);
        //printf("DATA6 : %d\n",((TxData6 & (0x8000 >> i)) ? 1 : 0));
        MySPI_W_SCK(1);
        
       // usleep(1000000);  //��ʱ1s
        // if(MySPI_R_MISO()) RxData |= (0x8000 >> i);
    }
    
    printf("Transmit Stop!!\n");
    

    return RxData;
}

/*********������*******/

int key(void)
{
   int ret;
   int fd;
   
   struct input_event event;

   fd = open(path,O_RDONLY);
   if(fd < 0)
   {
      perror(path);
      exit(-1);
   }

   while(1)
   {
      ret = read(fd,&event,sizeof(struct input_event));
      if(ret == sizeof(struct input_event))
      {
            //δ��������
            
            //EV_SYN���¼��ָ���־������ӡ
            if(event.type != EV_SYN)
            {
               printf("Event: time %ld.%ld,",event.time.tv_sec,event.time.tv_usec);
               printf("type:%d,code:%d,value:%d\n", event.type,event.code,event.value);
            }
      }
      break;
   }

   close(fd);

   return 0;

}

int main(int argc, char **argv) {
    int ret;

    // ��ȡGPIO������
    chip5 = gpiod_chip_open("/dev/gpiochip4");
    if (chip5 == NULL) {
        perror("gpiod_chip_open error\n");
        return -1;
    }
    
    chip2 = gpiod_chip_open("/dev/gpiochip1");
     if (chip2 == NULL) {
        perror("gpiod_chip_open error\n");
        return -1;
    }
    // ��ȡGPIO����
    int pin_numbers[8] = {16, 17, 10, 11, 20, 21, 4, 3};
    for (int i = 0; i < 8; i++) {
        /*
        if(i == 0)
        {
          line[i] = gpiod_chip_get_line(chip2, pin_numbers[i]);
        }
        else if(i == 1)
        {
          line[i] = gpiod_chip_get_line(chip2, pin_numbers[i]);
        }
        else
        {
          line[i] = gpiod_chip_get_line(chip5, pin_numbers[i]);
        }
        */

        line[i] = gpiod_chip_get_line(chip5, pin_numbers[i]);
        
        
        if (line[i] == NULL) {
            printf("gpiod_chip_get_line error for pin %d\n", pin_numbers[i]);
            goto release_chip;
        }
    }

    // ����GPIOΪ���ģʽ
    for (int i = 0; i < 8; i++) {
        ret = gpiod_line_request_output(line[i], "gpio", 0);
        if (ret < 0) {
            printf("gpiod_line_request_output error for pin %d\n", pin_numbers[i]);
            goto release_line;
        }
    }

    /*��ʼ��Ĭ�ϵ�ƽ*/
    MySPI_W_SS(1);   // SS��ʼ�ߵ�ƽ
    MySPI_W_SCK(1);  // SCK��ʼ�ߵ�ƽ
    MySPI_W_MOSI1(0);
    MySPI_W_MOSI2(0);
    MySPI_W_MOSI3(0);
    MySPI_W_MOSI4(0);
    MySPI_W_MOSI5(0);
    MySPI_W_MOSI6(0);
    
    //usleep(2000000);

    uint16_t spi1=12345;
	  uint16_t spi2=22222;
	  uint16_t spi3=33333;
	  uint16_t spi4=44444;
	  uint16_t spi5=55555;
	  uint16_t spi6=60000; 
     
    //key();

    //while(1){
    
      MySPI_Start();
      MySPI_SwapWord_MOSI1 (spi1,spi2,spi3,spi4,spi5,spi6);
      MySPI_Stop();
      
      printf("FINISH and RESET!!\n");
      
      MySPI_W_MOSI1(0);    //��������������0
      MySPI_W_MOSI2(0);
      MySPI_W_MOSI3(0);
      MySPI_W_MOSI4(0);
      MySPI_W_MOSI5(0);
      MySPI_W_MOSI6(0);
     
     // usleep(2000000);    //��ÿ�η�������֮����ʱ2�룬������Ƭѡ����
    //}
    //if(key() == 0)break;
    
    
    
    
    /*
        MySPI_Start();
        //��Ҫ��ʱ��
        MySPI_SwapWord_MOSI1 (spi1,spi2,spi3,spi4,spi5,spi6);
        MySPI_Stop();
    */
    


    
           /*��ʣ���ִ���������*/
    /*for (uint8_t i = 0; i < 1; i++) {                     
        // ���ݵ�ǰ�ִ�ѡ������
        uint16_t *currentRound;
        switch (i) {
            case 0:
                currentRound = round1;
                break;
            case 1:
                currentRound = round2;
                break;
            case 2:
                currentRound = round3;
                break;
            case 3:
                currentRound = round4;
                break;
            case 4:
                currentRound = round5;
                break;
            case 5:
                currentRound = round6;
                break;
            case 6:
                currentRound = round7;
                break;
            case 7:
                currentRound = round8;
                break;
            case 8:
                currentRound = round9;
                break;
            case 9:
                currentRound = round10;
                break;
        }
        
        // ִ��SPI����
        MySPI_Start();
        uint16_t receiveData = MySPI_SwapWord_MOSI1(
                currentRound[0], currentRound[1], currentRound[2],
                currentRound[3], currentRound[4], currentRound[5]);
        MySPI_Stop();
    
      }
    */
    release_line:
    // �ͷ�GPIO����
    for (int i = 0; i < 8; i++) {
        if (line[i] != NULL) {
            gpiod_line_release(line[i]);
        }
    }
    

    release_chip:
    // �ͷ�GPIO������
      if (chip5 != NULL) {
        gpiod_chip_close(chip5);
      }
      /*
      if (chip2 != NULL) {
        gpiod_chip_close(chip2);
      }
      */
    
    return 0;
}    


