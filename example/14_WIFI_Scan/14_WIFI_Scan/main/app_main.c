// WIFI_Scan Example
/*************************************************************************
 * 作者：		路过人间
 * 邮箱：		cnicfhnui@126.com
 * QQ：			125745255
 * 淘宝店铺：	https://hellobug.taobao.com/
 * 博客地址：	https://hellobug.blog.csdn.net/
 * 在线教程：	https://hellobug.blog.csdn.net/category_10350929.html
 * ESP_IDF版本：V4.4.2
 * 例程说明简介：	
				WIFI扫描示例
**************************************************************************/

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/event_groups.h"
#include "esp_idf_version.h"

#define DEFAULT_SCAN_LIST_SIZE CONFIG_EXAMPLE_SCAN_LIST_SIZE

static const char *TAG = "WIFI_Scan Demo";

// wifi 事件
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	switch (event->event_id)
	{
	case SYSTEM_EVENT_WIFI_READY:			// ESP32 WIFI就绪
		ESP_LOGI(TAG, "event-->ESP32 WiFi ready");
		break;
	case SYSTEM_EVENT_SCAN_DONE:			// 扫描AP完成
		ESP_LOGI(TAG, "event-->ESP32 finish scanning AP");
		break;
	case SYSTEM_EVENT_STA_START:			// 开始STA模式
		ESP_LOGI(TAG, "event-->ESP32 station start");
		break;
	case SYSTEM_EVENT_STA_STOP:				// 停止STA模式
		ESP_LOGI(TAG, "event-->ESP32 station stop");
		break;
	case SYSTEM_EVENT_STA_CONNECTED:		// STA模式连接到AP
		ESP_LOGI(TAG, "event-->ESP32 station connected to AP");
		break;
	case SYSTEM_EVENT_STA_DISCONNECTED:		// STA模式断开与AP的连接
		ESP_LOGI(TAG, "event-->ESP32 station disconnected from AP");
		break;
	case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:	// ESP32 Station连接的AP的验证模式已更改
		ESP_LOGI(TAG, "event-->the auth mode of AP connected by ESP32 station changed");
		break;
	case SYSTEM_EVENT_STA_GOT_IP:			// ESP32 Station从连接的AP获取到IP
		ESP_LOGI(TAG, "event-->ESP32 station got IP from connected AP");
		break;		
	case SYSTEM_EVENT_STA_LOST_IP:			// ESP32 Station丢失IP或IP重置
		ESP_LOGI(TAG, "event-->ESP32 station lost IP and the IP is reset to 0");
		break;
	case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:	// ESP32 station wps在注册模式下成功
		ESP_LOGI(TAG, "event--> ESP32 station wps succeeds in enrollee mode");
		break;
	case SYSTEM_EVENT_STA_WPS_ER_FAILED:	// ESP32 station wps在注册模式下失败
		ESP_LOGI(TAG, "event--> ESP32 station wps fails in enrollee mode");
		break;
	case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:	// ESP32 station wps在注册模式超时
		ESP_LOGI(TAG, "event--> ESP32 station wps timeout in enrollee mode");
		break;
	case SYSTEM_EVENT_STA_WPS_ER_PIN:		// ESP32 Station WPS密码在登记人模式下
		ESP_LOGI(TAG, "event--> ESP32 station wps pin code in enrollee mode");
		break;
	case SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP:// ESP32 station wps在登记人模式下重叠
		ESP_LOGI(TAG, "event--> ESP32 station wps overlap in enrollee mode");
		break;
	case SYSTEM_EVENT_AP_START:				// ESP32 AP模式开始
		ESP_LOGI(TAG, "event--> ESP32 soft-AP start");
		break;
	case SYSTEM_EVENT_AP_STOP:				// ESP32 AP模式停止
		ESP_LOGI(TAG, "event--> ESP32 soft-AP stop");
		break;
	case SYSTEM_EVENT_AP_STACONNECTED:		// ESP32 AP模式下，有站接入此AP
		ESP_LOGI(TAG, "event--> a station connected to ESP32 soft-AP ");
		break;
	case SYSTEM_EVENT_AP_STADISCONNECTED:	// ESP32 AP模式下，有站断开此AP
		ESP_LOGI(TAG, "event--> a station disconnected from ESP32 soft-AP ");
		break;
	case SYSTEM_EVENT_AP_STAIPASSIGNED:		// ESP32 AP模式下，为连接的站分配IP
		ESP_LOGI(TAG, "event--> ESP32 soft-AP assign an IP to a connected station");
		break;
	case SYSTEM_EVENT_AP_PROBEREQRECVED:	// ESP32 AP模式下，在soft-AP接口中接收探测请求数据包
		ESP_LOGI(TAG, "event--> Receive probe request packet in soft-AP interface");
		break;
	default:
		break;
	}
	return ESP_OK;
}

// 打印WIFI的认证方式
static void print_auth_mode(int authmode)
{
	switch (authmode) {
	case WIFI_AUTH_OPEN:
		ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OPEN");
		break;
	case WIFI_AUTH_WEP:
		ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WEP");
		break;
	case WIFI_AUTH_WPA_PSK:
		ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_PSK");
		break;
	case WIFI_AUTH_WPA2_PSK:
		ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_PSK");
		break;
	case WIFI_AUTH_WPA_WPA2_PSK:
		ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_WPA2_PSK");
		break;
	case WIFI_AUTH_WPA2_ENTERPRISE:
		ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_ENTERPRISE");
		break;
	case WIFI_AUTH_WPA3_PSK:
		ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_PSK");
		break;
	case WIFI_AUTH_WPA2_WPA3_PSK:
		ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_WPA3_PSK");
		break;
	default:
		ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_UNKNOWN");
		break;
	}
}

// 打印WIFI的加密方式
static void print_cipher_type(int pairwise_cipher, int group_cipher)
{
	switch (pairwise_cipher) {
	case WIFI_CIPHER_TYPE_NONE:
		ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_NONE");
		break;
	case WIFI_CIPHER_TYPE_WEP40:
		ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP40");
		break;
	case WIFI_CIPHER_TYPE_WEP104:
		ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP104");
		break;
	case WIFI_CIPHER_TYPE_TKIP:
		ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP");
		break;
	case WIFI_CIPHER_TYPE_CCMP:
		ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_CCMP");
		break;
	case WIFI_CIPHER_TYPE_TKIP_CCMP:
		ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
		break;
	default:
		ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
		break;
	}

	switch (group_cipher) {
	case WIFI_CIPHER_TYPE_NONE:
		ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_NONE");
		break;
	case WIFI_CIPHER_TYPE_WEP40:
		ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP40");
		break;
	case WIFI_CIPHER_TYPE_WEP104:
		ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP104");
		break;
	case WIFI_CIPHER_TYPE_TKIP:
		ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP");
		break;
	case WIFI_CIPHER_TYPE_CCMP:
		ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_CCMP");
		break;
	case WIFI_CIPHER_TYPE_TKIP_CCMP:
		ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
		break;
	default:
		ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
		break;
	}
}

// 初始化WIFI 配置扫描模式 开始扫描 打印扫描到的AP信息
static void wifi_scan(void)
{
	ESP_ERROR_CHECK(esp_netif_init());					// 初始化底层TCP/IP堆栈。在应用程序启动时，应该调用此函数一次
	ESP_ERROR_CHECK(esp_event_loop_create_default());	// 创建默认事件循环
	esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();// 创建一个默认的WIFI-STA网络接口，
	assert(sta_netif);									// 如果初始化错误，此API将中止
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));				// 使用默认wifi初始化配置

	uint16_t number = DEFAULT_SCAN_LIST_SIZE;			// 默认扫描列表大小
	wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];	// AP信息结构体大小
	uint16_t ap_count = 0;								// 初始化AP数量0
	memset(ap_info, 0, sizeof(ap_info));				// 初始化AP信息结构体

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));	// 设置WiFi的工作模式为 STA
	ESP_ERROR_CHECK(esp_wifi_start());					// 启动WiFi连接
	ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));	// 开始扫描WIFI(默认配置，阻塞方式)
	ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));// 获取搜索的具体AP信息
	ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));		//接入点的数量
	ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
	for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) {
		ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);		// 打印WIFI名称
		ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);		// 打印信号强度	
		print_auth_mode(ap_info[i].authmode);				// 打印认证方式
		if (ap_info[i].authmode != WIFI_AUTH_WEP) {
			print_cipher_type(ap_info[i].pairwise_cipher, ap_info[i].group_cipher);
		}
		ESP_LOGI(TAG, "Channel \t\t%d\n", ap_info[i].primary);
	}
}

// 主函数
void app_main(void)
{
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	// 设置串口日志级别
	esp_log_level_set(TAG, ESP_LOG_INFO);
	ESP_LOGI(TAG,"APP Is Start!~\r\n");
	ESP_LOGI(TAG,"IDF Version is %d.%d.%d",ESP_IDF_VERSION_MAJOR,ESP_IDF_VERSION_MINOR,ESP_IDF_VERSION_PATCH);
	ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
	ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

	wifi_scan();
}
