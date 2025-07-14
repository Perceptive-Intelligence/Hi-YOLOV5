                                                    /* whole project version */

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
/*��������ͷ�ļ�*/
#include <errno.h>
/*������������ͷ�ļ�*/
#include <stdbool.h>
/*��������ͷ�ļ�*/
#include <linux/input.h>
#include <linux/input-event-codes.h>
/*����ע���ע������ͷ�ļ�*/
#include <port_init.h>
#include <api.h>
//#include <monitor.h>

                                                    /**************/
                                                    /*���ݷ����߼�*/
                                                    /**************/
                                                    
uint16_t MySPI_SwapWord_MOSI1(uint16_t TxData1, uint16_t TxData2, uint16_t TxData3,
                              uint16_t TxData4, uint16_t TxData5, uint16_t TxData6) {
    uint16_t RxData = 0;
    uint16_t stop_state;
    int ret;
    //printf("Transmit Start!!\n");

    for (uint8_t i = 0; i < 16; i++) {

        //while(1){
        MySPI_W_SCK(0);
        
        //stop_state_process();
        
        //printf("*****************************************************\n");
        
        MySPI_W_MOSI1((TxData1 & (0x8000 >> i)) ? 1 : 0);

        MySPI_W_MOSI2((TxData2 & (0x8000 >> i)) ? 1 : 0);
  
        MySPI_W_MOSI3((TxData3 & (0x8000 >> i)) ? 1 : 0);

        MySPI_W_MOSI4((TxData4 & (0x8000 >> i)) ? 1 : 0);

        MySPI_W_MOSI5((TxData5 & (0x8000 >> i)) ? 1 : 0);
      
        MySPI_W_MOSI6((TxData6 & (0x8000 >> i)) ? 1 : 0);

        //printf("*****************************************************\n");

        
        MySPI_W_SCK(1);
        //}
        

        // if(MySPI_R_MISO()) RxData |= (0x8000 >> i);
    }
    

    //printf("Transmit Stop!!\n");
    

    return RxData;
}


                                                                          /**********************/
                                                                          /*********��������*******/
                                                                          /**********************/

uint16_t transmit_drive(uint16_t rx_data) {

    static uint8_t count_prt = 0;
    bool state = 0; 
    uint16_t data[6];
    uint16_t count = 0;
    
    data[count_prt++] = rx_data;
    //printf("count_prt = %d\n",count_prt);
    
    if(count_prt == 6)
    {
      /*
      printf("--------------------------\n");
      printf("data1 = %d\n",data[0]);
      printf("data2 = %d\n",data[1]);
      printf("data3 = %d\n",data[2]);
      printf("data4 = %d\n",data[3]);
      printf("data5 = %d\n",data[4]);
      printf("data6 = %d\n",data[5]);
      printf("--------------------------\n");
      printf("MySPI_Start!!\n");
      */
      
        while(read_feedback_value() == 1)
        {
          count++;
          //printf("ENTER feedback");
          if(count == 10000)
            {
              count = 0;
              //warning();���ظ���λ���źŵĺ�����
            }
        }
        
      
      MySPI_Start(); 
      //usleep(100);     
      //printf("CS DOWN !!\n");
      MySPI_SwapWord_MOSI1 (data[0],data[1],data[2],data[3],data[4],data[5]);  //ÿ�����ݵ�ÿ������ͨ��ÿ���ߴ���
      
      MySPI_Stop();
      //printf("CS UP !!\n");
    
      //printf("MySPI_SwapWord_MOSI1 FINISH !!\n");
      //printf("RESET !!\n");
      MySPI_W_MOSI1(0);    //��������������0
      MySPI_W_MOSI2(0);
      MySPI_W_MOSI3(0);
      MySPI_W_MOSI4(0);
      MySPI_W_MOSI5(0);
      MySPI_W_MOSI6(0);
      
      count_prt = 0;
    }
    
      
      
      return 0;
}
       