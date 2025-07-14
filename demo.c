#include <gpiod.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>

 int main(int argc, char **argv)
 {
     int i;
     int ret;
     int value;

     struct gpiod_chip * chip;      //GPIO控制器句柄
     struct gpiod_line * line1;      //GPIO引脚句柄
     struct gpiod_line * line2;      //GPIO引脚句柄

     /*获取GPIO控制器*/
     chip = gpiod_chip_open("/dev/gpiochip4");
     if(chip == NULL)
     {
         perror("gpiod_chip_open error\n");
         return -1;
     }

     /*获取GPIO引脚*/
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

     /*设置GPIO为输出模式*/
     ret = gpiod_line_request_output(line1,"gpio",0);
     if(ret < 0)
     {
         printf("gpiod_line_request_output error\n");
         goto release_chip;
     }
     /*设置GPIO为输入模式*/
     ret = gpiod_line_request_input(line2,"gpio");
     if(ret < 0)
     {
         printf("gpiod_line_request_input error\n");
         goto release_chip;
     }
      
     for(i = 0;i<10;i++)
     {
         gpiod_line_set_value(line1,1);
         usleep(1000000);  //延时1s
         
         value = gpiod_line_get_value(line2);
         printf("value = %d\n",value);
         

         gpiod_line_set_value(line1,0);
         usleep(1000000);

         value = gpiod_line_get_value(line2);
         printf("value = %d\n",value);
     }

     release_line:{
     /*释放GPIO引脚*/
     gpiod_line_release(line1);
     gpiod_line_release(line2);
     }
     release_chip:
     /*释放GPIO控制器*/
     gpiod_chip_close(chip);


     return 0;
 }