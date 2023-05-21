/*************************************************************************
 * 作者：		路过人间
 * 邮箱：		cnicfhnui@126.com
 * QQ：			125745255
 * 微信：		18559979152
 * 淘宝店铺：	https://hellobug.taobao.com/
 * 博客地址：	https://hellobug.blog.csdn.net/
 * 在线教程：	https://hellobug.blog.csdn.net/category_10350929.html
 * ESP_IDF版本：V4.2
 * 例程说明简介：	
	演示使用RMT读取DHT11的温度湿度示例

注意！！！！！！！
	请修改工程目录中的.vscode/c_cpp_properties.json中的库路径，更改为自己电脑的IDF库路径
	请修改工程目录中的main/CMakeLists.txt中的包含库路径，更改为自己电脑的IDF库路径（如果有）
**************************************************************************/
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include "../build/config/sdkconfig.h"

#include "DHT11.h"

const static char *TAG = "DHT11_Demo";

#define DHT11_GPIO		26		// DHT11引脚定义

// 温度 湿度变量
int temp = 0,hum = 0;

// 主函数
void app_main(void)
{
	ESP_ERROR_CHECK(nvs_flash_init());
	vTaskDelay(100 / portTICK_PERIOD_MS);

	ESP_LOGI(TAG, "[APP] APP Is Start!~\r\n");
	ESP_LOGI(TAG, "[APP] IDF Version is %d.%d.%d",ESP_IDF_VERSION_MAJOR,ESP_IDF_VERSION_MINOR,ESP_IDF_VERSION_PATCH);
	ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
	ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
	
	DHT11_Init(DHT11_GPIO);
	while (1){
		if (DHT11_StartGet(&temp, &hum)){
			ESP_LOGI(TAG, "[%lld] temp->%i.%i C     hum->%i%%", esp_timer_get_time(), temp / 10, temp % 10, hum);
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
