#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <new_spi_io.h>
#include <string.h>
#include <port_init.h>

uint16_t send(uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4, uint16_t data5, uint16_t data6, uint8_t type_of_transmit) //�������ݺʹ��������ͬһ�������ӿ�
{

  uint8_t state = type_of_transmit;
  uint16_t data[6];  //data��һ������
  data[0] = data1;
  data[1] = data2;
  data[2] = data3;
  data[3] = data4;
  data[4] = data5;
  data[5] = data6;
  /*
  printf("--------------------------\n");
  printf("data1 = %d\n",data[0]);
  printf("data2 = %d\n",data[1]);
  printf("data3 = %d\n",data[2]);
  printf("data4 = %d\n",data[3]);
  printf("data5 = %d\n",data[4]);
  printf("data6 = %d\n",data[5]);
  printf("--------------------------\n");
  */
  if(state == 0)
  {
    /*����*/
    //printf("state = %d\n",state);
    //printf("enter transmit_drive()\n");
    for(int i=0; i<6; i++)
    {
      transmit_drive(data[i]); //����һ������
    }
    //printf("Finish Sending One Round!!\n");
  }
  else if(state == 1)
  {
    /*����*/
    //printf("state = %d\n",state);
    
  }
  else 
  {
    printf("The type error!\n");
  }
  
  return 0;
}

void reset(void)
{
  MySPI_W_RESET(0);
  usleep(1);
  MySPI_W_RESET(1);
  usleep(1);
  
  char *argv[] = { "final_program_no_reset", NULL }; 
  char *envp[] = { NULL };
  printf("restart...\n");

    // ���� execve ����ִ�г���
    if (execve("/home/share/spi_io_test/final_program_no_reset", argv, envp) == -1) {
        perror("execve ����ʧ��");
        exit(EXIT_FAILURE);
    }
}
