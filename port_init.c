#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/*�߳̿⺯������ͷ�ļ�*/
#include <pthread.h>
                                                    /************************/
                                                    /*�˲��ֶ������Ųٿغ���*/
                                                    /************************/
struct gpiod_chip *chip5;      // GPIO���������
struct gpiod_line *line[10];      // GPIO���ž��
struct gpiod_line *line_feedback; //����������
uint16_t value;

void MySPI_W_SS(uint8_t BitValue) {
    gpiod_line_set_value(line[7], BitValue);
    
   // printf("CS : %d\n",BitValue);
   // printk(KERN_INFO "Hello, Kernel World!\n");
}

void MySPI_W_SCK(uint8_t BitValue) {
    gpiod_line_set_value(line[6], BitValue);
    //usleep(2);//С��50΢�루10Khz���������ݳ���
    //printf("SCK : %d\n",BitValue);
}

void MySPI_W_RESET(uint8_t BitValue) {          //���ڸ�λ
    gpiod_line_set_value(line[8], BitValue);
   // printf("RESET : %d\n",BitValue);
}

uint16_t read_feedback_value(void)
{
    uint16_t val = gpiod_line_get_value(line_feedback);
    return val;
 }

void MySPI_W_MOSI1(uint8_t BitValue) {
    gpiod_line_set_value(line[0], BitValue);
    //printf("SPI1 : %d\n",BitValue);
}

void MySPI_W_MOSI2(uint8_t BitValue) {
    gpiod_line_set_value(line[1], BitValue);
    //printf("          SPI2 : %d\n",BitValue);
}

void MySPI_W_MOSI3(uint8_t BitValue) {
    gpiod_line_set_value(line[2], BitValue);
    //printf("                    SPI3 : %d\n",BitValue);
}

void MySPI_W_MOSI4(uint8_t BitValue) {
    gpiod_line_set_value(line[3], BitValue);
    //printf("                              SPI4 : %d\n",BitValue);
}

void MySPI_W_MOSI5(uint8_t BitValue) {
    gpiod_line_set_value(line[4], BitValue);
    //printf("                                        SPI5 : %d\n",BitValue);
}

void MySPI_W_MOSI6(uint8_t BitValue) {
    gpiod_line_set_value(line[5], BitValue);
    //printf("                                                  SPI6 : %d\n",BitValue);
}

void MySPI_Start(void) {
    MySPI_W_SS(0);
}

void MySPI_Stop(void) {
    MySPI_W_SS(1);
}

                                                    /************************/
                                                    /*   ���ų�ʼ������     */
                                                    /************************/
int feedback_init(void)
{
  int ret;

    // ��ȡGPIO������
    chip5 = gpiod_chip_open("/dev/gpiochip4");
    if (chip5 == NULL) {
        perror("gpiod_chip_open error\n");
        gpiod_chip_close(chip5);
        return -1;
    }
    line_feedback = gpiod_chip_get_line(chip5, 13);
    if (line_feedback == NULL) {
            printf("line_feedback error for pin\n" );
              gpiod_line_release(line_feedback);
            }
          
        
    ret = gpiod_line_request_input(line_feedback, "feedback_input");
    if (ret < 0) {
        printf("gpiod_line_request_input error\n");
        gpiod_line_release(line_feedback);
        line_feedback = NULL;
        gpiod_chip_close(chip5);
        chip5 = NULL;
        return -1;
    }   
}
                                                    
                                                    
int port_init(void) {
    int ret;

    // ��ȡGPIO������
    chip5 = gpiod_chip_open("/dev/gpiochip4");
    if (chip5 == NULL) {
        perror("gpiod_chip_open error\n");
        gpiod_chip_close(chip5);
        return -1;
    }
    
    // ��ȡGPIO����
    int pin_numbers[9] = {16, 17, 10, 11, 20, 21, 4, 3, 12};  //GPIO5
    
    for (int i = 0; i < 9; i++) {
        line[i] = gpiod_chip_get_line(chip5, pin_numbers[i]);
        
        
        if (line[i] == NULL) {
            printf("gpiod_chip_get_line error for pin %d\n", pin_numbers[i]);
              gpiod_line_release(line[i]);
            }
        }
    

    // ����GPIOΪ���ģʽ
    for (int i = 0; i < 9; i++) {
        ret = gpiod_line_request_output(line[i], "gpio", 0);
        if (ret < 0) {
            printf("gpiod_line_request_output error for pin %d\n", pin_numbers[i]);
            
            for (int i = 0; i < 9; i++) 
            {
              gpiod_line_release(line[i]);
            }
        }
    }
    
    feedback_init();
    
        /*��ʼ��Ĭ�ϵ�ƽ*/
    MySPI_W_SS(1);   // SS��ʼ�ߵ�ƽ
    MySPI_W_SCK(1);  // SCK��ʼ�ߵ�ƽ
    MySPI_W_RESET(1);// RESET��ʼ�ߵ�ƽ
    MySPI_W_MOSI1(0);
    MySPI_W_MOSI2(0);
    MySPI_W_MOSI3(0);
    MySPI_W_MOSI4(0);
    MySPI_W_MOSI5(0);
    MySPI_W_MOSI6(0);


}


void feedback_cleanup(void)
{
    if (line_feedback != NULL) {
        gpiod_line_release(line_feedback);
        line_feedback = NULL;
    }

    printf("GPIO feedback resources cleaned up.\n");
}

int port_deinit(void)
{

  for(int i=0; i<9; i++) 
  {
    gpiod_line_release(line[i]);
  }
  
  feedback_cleanup();
  gpiod_chip_close(chip5);
  
}