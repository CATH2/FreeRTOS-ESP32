#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

#define BUTTON_PIN  0

void button_task(void *arg)
{
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    while (1) {
        if (gpio_get_level(BUTTON_PIN) == 0) {
            printf("Button pressed!\n");
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
}