#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
 
#define DS18_PIN 12    //定义引脚
#define ESP_HIGH gpio_set_level(DS18_PIN, 1)    //设置高电平
#define ESP_LOW gpio_set_level(DS18_PIN, 0)    //设置低电平
#define ESP_GET gpio_get_level(DS18_PIN)    //读取电平
#define ESP_OUT_MODE gpio_set_direction(DS18_PIN, GPIO_MODE_OUTPUT)    //设置输出模式
#define ESP_INPUT_MODE gpio_set_direction(DS18_PIN, GPIO_MODE_INPUT)    //设置输入模式
 
#define SKIP_ROM 0xCC    //跳过ROM操作
#define TRANSFORMATION_TEMP 0x44    //转换温度操作
#define READ_TEMP 0xBE    //读数据操作
 
/*************************初始化操作*************************/
unsigned char init_DS18(void)
{
    unsigned int init_state = 0;
    ESP_OUT_MODE;
    ESP_LOW;
    ets_delay_us(480);
    ESP_HIGH;
 
    ESP_INPUT_MODE;//设置输入模式，就是高阻态，此时上拉电阻提供高电平
    ets_delay_us(70);
    init_state = (gpio_get_level(DS18_PIN) == 0);
 
    ets_delay_us(410);
    printf("初始化状态：%d", init_state);
    return init_state;
}
/*************************写位操作*************************/
void DS18_Write_bit(char bit)
{
/*******写1操作********/
    if (bit & 1)
    {
        ESP_OUT_MODE;
        ESP_LOW;
        ets_delay_us(6);
        ESP_INPUT_MODE;//设置输入模式，就是高阻态，此时上拉电阻提供高电平
        ets_delay_us(64);
    }
/*******写0操作********/
    else
    {
        ESP_OUT_MODE;
        ESP_LOW;
        ets_delay_us(60);
        ESP_INPUT_MODE;//设置输入模式，就是高阻态，此时上拉电阻提供高电平
        ets_delay_us(10);
    }
}
/*************************读位操作*************************/
unsigned char DS18_Read_bit(void)
{
    unsigned char bit = 0;
    ESP_OUT_MODE;
/*******拉低电平，开始读取数据********/
    ESP_LOW;
    ets_delay_us(6);
    ESP_INPUT_MODE;//设置输入数据
    ets_delay_us(9);
    bit = ESP_GET;
    ets_delay_us(55);
 
    return bit;
}
/*************************写数据操作*************************/
void DS18_Write_byte(unsigned char data)
{
    unsigned char i;
    unsigned char a_data;
/*******写8次，一次为一位********/
    for (i = 0; i < 8; i++)
    {
        a_data = data >> i;//每写一位数据，就向右移一位，保证8位数据都能写入
        a_data &= 0x01;//与操作，只留刚刚右移的一位数据
        DS18_Write_bit(a_data);//写入一位数据
    }
    ets_delay_us(100);
}
/*************************读数据操作*************************/
unsigned char DS18_Read_byte(void)
{
    unsigned char i;
    unsigned char data = 0;
/*******读取8次，一次为一位********/
    for (i = 0; i < 8; i++)
    {
        if (DS18_Read_bit())
            data |= 0x01 << i;//读取到1就会向左移相应的位数
        ets_delay_us(15);
    }
    return data;
}
 
void app_main(void)
{
    gpio_pad_select_gpio(DS18_PIN);
    while (1)
    {
        unsigned char TEMP_LOW, TEMP_HIGH;
        float TEMP = 0;
/*************************温度转换过程*************************/
        while(!init_DS18())
        {
            printf("正在尝试初始化...");
        }
        DS18_Write_byte(SKIP_ROM);
        DS18_Write_byte(TRANSFORMATION_TEMP);
 
        vTaskDelay(750 / portTICK_PERIOD_MS);
/*************************读取温度过程*************************/
        while(!init_DS18())
        {
            printf("正在尝试初始化...");
        }
        DS18_Write_byte(SKIP_ROM);
        DS18_Write_byte(READ_TEMP);
        TEMP_LOW = DS18_Read_byte();
        TEMP_HIGH = DS18_Read_byte();
 
/*************************把数据转换成十进制*************************/
        TEMP = (float)(TEMP_LOW + (TEMP_HIGH * 256)) / 16;
 
        printf("温度=%f", TEMP);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}