#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "esp_log.h"

#define BUTTON_PIN  GPIO_NUM_0
static const char *TAG = "Key_Input";

#define LONG_PRESS_TIME_MS  1000

static void button_task(void *arg)
{
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    TickType_t press_start_time = 0;
    bool is_button_pressed = false;

    while (1) {
        bool button_state = gpio_get_level(BUTTON_PIN);

        if (button_state && !is_button_pressed) {
            // Button is pressed
            is_button_pressed = true;
            press_start_time = xTaskGetTickCount();
        } else if (!button_state && is_button_pressed) {
            // Button is released
            is_button_pressed = false;
            TickType_t press_duration = xTaskGetTickCount() - press_start_time;

            if (press_duration >= pdMS_TO_TICKS(LONG_PRESS_TIME_MS)) {
                // 长按
                ESP_LOGI(TAG,"长按");
            } else {
                // 短按
                ESP_LOGI(TAG,"短按");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main()
{
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
}