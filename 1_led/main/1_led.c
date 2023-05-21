#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <esp_log.h>

#include "driver/gpio.h"


static const char *TAG = "LED"; 

void app_main(void)
{
	ESP_LOGI(TAG,"LED is starting~");//报告程序已启动
	
	gpio_pad_select_gpio(2);// 选择要操作的GPIO
	gpio_set_direction(2, GPIO_MODE_OUTPUT);// 设置GPIO为推挽输出模式
	
	while(1)
	{
		ESP_LOGI(TAG,"Trun down");//打印电平即将调低
		gpio_set_level(2,0);//将IO2的电平拉高
		vTaskDelay(1000/portTICK_PERIOD_MS);//延时1s
		
		ESP_LOGI(TAG,"Trun up");//打印电平即将调高
		gpio_set_level(2,1);//将IO2的电平拉高
		vTaskDelay(1000/portTICK_PERIOD_MS);//延时1s
	}
}
