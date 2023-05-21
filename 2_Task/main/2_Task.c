#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "../build/config/sdkconfig.h"
#include "esp_log.h"

#include "freertos/semphr.h"

static const char *TAG = "递归信号量";

SemaphoreHandle_t mutexHandle;
 
void Task1(void*pvParam)
{
    while(1)
    {
        ESP_LOGI(TAG,"-----------------------------------------\n");
        ESP_LOGI(TAG,"Task1 is begin!\n");
        xSemaphoreTakeRecursive(mutexHandle,portMAX_DELAY);//创建递归互斥信号量A
        ESP_LOGI(TAG,"Task1 take A!\n");
        for(int i=0;i<10;i++)
        {
            ESP_LOGI(TAG,"Task1 i=%d for A!\n", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
 
        xSemaphoreTakeRecursive(mutexHandle,portMAX_DELAY);//创建递归互斥信号量B
        ESP_LOGI(TAG,"Task1 take B!\n");
        for(int i=0;i<10;i++)
        {
            ESP_LOGI(TAG,"Task1 i=%d for B!\n", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }  
        ESP_LOGI(TAG,"Task1 give B!\n");
        xSemaphoreGiveRecursive(mutexHandle);//释放递归互斥信号量B
        vTaskDelay(pdMS_TO_TICKS(3000));//延时3s
 
        ESP_LOGI(TAG,"Task1 give A!\n");
        xSemaphoreGiveRecursive(mutexHandle);//释放递归互斥信号量A
        vTaskDelay(pdMS_TO_TICKS(3000));//延时3s
    }
 
}
void Task2(void*pvParam)
{
    vTaskDelay(pdMS_TO_TICKS(1000));
    while(1)
    {
        ESP_LOGI(TAG,"-----------------------------------------\n");
        ESP_LOGI(TAG,"Task2 is begin!\n");
        xSemaphoreTakeRecursive(mutexHandle,portMAX_DELAY);//创建递归互斥信号量
        ESP_LOGI(TAG,"Task2 take!\n");
        for(int i=0;i<10;i++)
        {
            ESP_LOGI(TAG,"Task2 i=%d!\n", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }  
        ESP_LOGI(TAG,"Task2 give!\n");
        xSemaphoreGiveRecursive(mutexHandle);//释放递归互斥信号量
        vTaskDelay(pdMS_TO_TICKS(3000));//延时3s
    }
} 
 
 
 
void app_main(void)
{
    mutexHandle = xSemaphoreCreateRecursiveMutex();//创建递归互斥信号量
    vTaskSuspendAll();//挂起所有任务
    xTaskCreate(Task2,"Task2",1024*5,NULL,1,NULL);
    xTaskCreate(Task1,"Task1",1024*5,NULL,1,NULL);
    
    xTaskResumeAll();//恢复所有任务
}


