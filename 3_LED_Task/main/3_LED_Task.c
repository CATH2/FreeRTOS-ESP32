#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <esp_log.h>

#define LED 2

static const char *TAG = "LED_Task";

void LED_Task(void *param)
{
	gpio_pad_select_gpio(LED);//选择GPIO
	gpio_set_direction(LED, GPIO_MODE_OUTPUT);// 设置GPIO为推挽输出模式
	
	int LED_STATUE = 0;
	
	while(1) 
	{
		ESP_LOGI(TAG, "LED STATUE%d\n",LED_STATUE);
		gpio_set_level(LED, LED_STATUE);// GPIO输出电平(1或者0)
		vTaskDelay(1000 / portTICK_PERIOD_MS);//延时1s
		
		LED_STATUE = !LED_STATUE;
	}
	
}

void app_main(void)
{
	xTaskCreate(LED_Task,"LED_Task",2048,NULL,1,NULL);//创建任务
}
