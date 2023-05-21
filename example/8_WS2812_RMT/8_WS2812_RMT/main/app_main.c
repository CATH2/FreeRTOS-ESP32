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
	WS2812_RMT示例
		演示使用RMT控制RGB灯WS2812实现彩虹变幻 

注意！！！！！！！
	请修改工程目录中的.vscode/c_cpp_properties.json中的库路径，更改为自己电脑的IDF库路径
	请修改工程目录中的main/CMakeLists.txt中的包含库路径，更改为自己电脑的IDF库路径（如果有）
**************************************************************************/
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <soc/rmt_struct.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include "../build/config/sdkconfig.h"

#include "WS2812.h"



#define TAG				"WS2812_RMT_Demo"

#define delay_ms(ms) vTaskDelay((ms) / portTICK_RATE_MS)

typedef enum {
	GreenAdd,		// 绿色值+
	RedMinus,		// 红色值-
	BlueAdd,		// 蓝色值+
	GreenMinus,		// 绿色值-
	RedAdd,			// 红色值+
	BlueMinus,		// 蓝色值-
} color_change_t;


// RGB灯彩虹效果，如果有多个灯串联可以看到彩虹效果
void WS2812_Rainbow_Task(void *pvParameters)
{
	const uint8_t anim_step = 10;		// 颜色值步进，0-255，每次变化1
	const uint8_t anim_max = 250;		// 最大值
	const uint8_t pixel_count = 6;		// 灯的数量（开发板只有一个，WS2812支持单总线串联控制）
	const uint8_t delay = 20;			// 单次变化间隔延时
	WS2812_Init();						// 初始化WS2812

	rgbVal color = makeRGBVal(anim_max, 0, 0);
	uint8_t step = 0;
	rgbVal color2 = makeRGBVal(anim_max, 0, 0);
	uint8_t step2 = 0;
	rgbVal *pixels;
	pixels = malloc(sizeof(rgbVal) * pixel_count);
	while (1) {
		color = color2;
		step = step2;
		for (uint8_t i = 0; i < pixel_count; i++) {
			pixels[i] = color;
			if (i == 1) {
				color2 = color;
				step2 = step;
			}
			switch (step) {
				case GreenAdd:
					color.g += anim_step;
					if (color.g >= anim_max)
						step++;
				break;
				case RedMinus:
					color.r -= anim_step;
					if (color.r == 0)
						step++;
				break;
				case BlueAdd:
					color.b += anim_step;
					if (color.b >= anim_max)
						step++;
				break;
				case GreenMinus:
					color.g -= anim_step;
					if (color.g == 0)
						step++;
				break;
				case RedAdd:
					color.r += anim_step;
					if (color.r >= anim_max)
						step++;
				break;
				case BlueMinus:
					color.b -= anim_step;
					if (color.b == 0)
						step = 0;
				break;
			}
		}
		WS2812_SetColors(pixel_count, pixels);// 写入颜色(灯数量，颜色值数组)
		//ESP_LOGI(TAG, "Color Value R:%d G:%d B:%d",pixels[0].r,pixels[0].g,pixels[0].b);
		//ESP_LOGI(TAG, "Color Value R:%d G:%d B:%d",pixels[1].r,pixels[1].g,pixels[1].b);
		//ESP_LOGI(TAG, "Color Value R:%d G:%d B:%d",pixels[2].r,pixels[2].g,pixels[2].b);

		delay_ms(delay);
	}
}

void app_main()
{
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_LOGI(TAG, "[APP] APP Is Start!~\r\n");
	ESP_LOGI(TAG, "[APP] IDF Version is %d.%d.%d",ESP_IDF_VERSION_MAJOR,ESP_IDF_VERSION_MINOR,ESP_IDF_VERSION_PATCH);
	ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
	ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
	
	xTaskCreate(WS2812_Rainbow_Task, "WS2812_Rainbow_Task", 4096, NULL, 10, NULL);
	return;
}