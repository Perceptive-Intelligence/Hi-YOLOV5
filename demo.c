#include <gpiod.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>

 int main(int argc, char **argv)
 {
     int i;
     int ret;
     int value;

     struct gpiod_chip * chip;      //GPIO���������
     struct gpiod_line * line1;      //GPIO���ž��
     struct gpiod_line * line2;      //GPIO���ž��

     /*��ȡGPIO������*/
     chip = gpiod_chip_open("/dev/gpiochip4");
     if(chip == NULL)
     {
         perror("gpiod_chip_open error\n");
         return -1;
     }

     /*��ȡGPIO����*/
     line1 = gpiod_chip_get_line(chip,12);
     if(line1 == NULL)
     {
         printf("gpiod_chip_get_line error\n");
         goto release_line;
     }
     
     line2 = gpiod_chip_get_line(chip, 13);
     if(line2 == NULL)
     {
         printf("gpiod_chip_get_line error\n");
         goto release_line;
     }

     /*����GPIOΪ���ģʽ*/
     ret = gpiod_line_request_output(line1,"gpio",0);
     if(ret < 0)
     {
         printf("gpiod_line_request_output error\n");
         goto release_chip;
     }
     /*����GPIOΪ����ģʽ*/
     ret = gpiod_line_request_input(line2,"gpio");
     if(ret < 0)
     {
         printf("gpiod_line_request_input error\n");
         goto release_chip;
     }
      
     for(i = 0;i<10;i++)
     {
         gpiod_line_set_value(line1,1);
         usleep(1000000);  //��ʱ1s
         
         value = gpiod_line_get_value(line2);
         printf("value = %d\n",value);
         

         gpiod_line_set_value(line1,0);
         usleep(1000000);

         value = gpiod_line_get_value(line2);
         printf("value = %d\n",value);
     }

     release_line:{
     /*�ͷ�GPIO����*/
     gpiod_line_release(line1);
     gpiod_line_release(line2);
     }
     release_chip:
     /*�ͷ�GPIO������*/
     gpiod_chip_close(chip);


     return 0;
 }