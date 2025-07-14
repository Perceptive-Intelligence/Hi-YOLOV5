#include <stdio.h>
#include <stdlib.h>
#include <gpiod.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#define MONITORED_GPIO_LINE_NUMBER 17

struct gpiod_chip *chip;      // GPIO���������
struct gpiod_line *line;      // GPIO���ž��

uint16_t stop_state;


void stop_state_process(void)
{
      while (stop_state)
      {
        printf("stop_state_process !!\n");
        usleep(2000000); // �������ߵȴ�
      }
        
}



// �̺߳��������ڼ���ض�����
void* monitor_gpio(void* arg) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    struct gpiod_line_event event;
    int ret;
    uint16_t value;
    uint16_t count;

    // ��GPIOоƬ
    chip = gpiod_chip_open("/dev/gpiochip4");
    if (!chip) {
        perror("Failed to open GPIO chip");
        return NULL;
    }

    // ��ȡҪ����GPIO��
    line = gpiod_chip_get_line(chip, 13);
    if (!line) {
        perror("Failed to get GPIO line");
        gpiod_chip_close(chip);
        return NULL;
    }

    // ������GPIO�ߵı�Ե�¼�
    ret = gpiod_line_request_both_edges_events(line, "gpio-monitor");
    if (ret < 0) {
        perror("Failed to request edge events");
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        return NULL;
    }

    while (1) {
    
       value = gpiod_line_get_value(line);
       count = 0;
       
       if(value == 1)
       {
         stop_state = 1;
         while(gpiod_line_get_value(line) == 1)
         {
           printf("count = %d\n",count);
           count++;
           
           if(count == 1000)
             feedback = 1;
           else
             feedback = 0;
             
         }
         stop_state = 0;
       }
       
       usleep(10000); // ÿ�� 10 ������һ�ε�ƽ״̬
    
    /*
        ret = gpiod_line_event_wait(line, NULL);
        if (ret < 0) {
            perror("Event wait error");
            break;
        } 
        else if (ret > 0) {
            ret = gpiod_line_event_read(line, &event);
            if (ret < 0) {
                perror("Event read error");
                break;
            }
           if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {      //�����ؼ��
                
                printf("GPIOD_LINE_EVENT_RISING_EDGE Happen !! \n");
                exit(EXIT_SUCCESS);
                
                
            }
        }
    */
        
    }
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return NULL;
}

void monitor_create(void)
{
      pthread_t monitor_thread;
    // ��������߳�
      printf("Enter Monitor_gpio !!\n");
     if (pthread_create(&monitor_thread, NULL, monitor_gpio, NULL) != 0) {
        perror("Failed to create monitor thread");

    }
}

/*
    �ֶ��ͷż������
*/
void monitor_gpio_deinit(void)
{

    gpiod_line_release(line);
    gpiod_chip_close(chip);
  
}




