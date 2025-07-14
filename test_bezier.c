#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include <api.h>
#include <new_spi_io.h>
#include <port_init.h>
//#include <monitor.h>

/*线程库函数所需头文件*/
#include <pthread.h>

int main(int argc, char *argv[])
{
      port_init();
      
      //monitor_create();
            
      printf("Test Start!!\n");


      for(int i=0; i<9;i++)
      {
        send(32828,32768,0,20000,0,15000,0);
        //usleep(500000);
        send(500,500,500,500,500,15000,0);
        //usleep(500000);
        send(5000,500,500,0,0,0,0);
        //usleep(500000);
      }
      //usleep(1000000);
     for(int i =0; i<100; i++){
      send(0,10000,10000,30000,30000,50000,0);
      send(5000,0,0,0,0,0,0);
      
      //usleep(1000000);
      
      send(1024,10000,10000,30000,30000,50000,0);
      send(5000,30000,10000,0,0,0,0);
      
      //usleep(1000000);
     
      send(2048,10000,10000,30000,30000,50000,0);
      send(5000,30000,10000,0,0,0,0);
      
      //usleep(1000000);
      
      send(3072,10000,10000,30000,30000,50000,0);       
      send(5000,10000,30000,30000,10000,0,0);
      printf("Test Finish!!\n");
   }
      
    
      port_deinit();
      printf("port_deinit !!\n");
      //monitor_gpio_deinit();
      //printf("monitor_gpio_deinit !!\n");
}
