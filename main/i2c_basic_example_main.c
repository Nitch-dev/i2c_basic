#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include "ssd1306.h"
#include "font8x8_basic.h"
#include "driver/gpio.h"

SSD1306_t dev;
#define BUZZER_PIN 27
#define GPIO_NUM_2 2

TaskHandle_t buzzerTaskHandler;


void IRAM_ATTR isr_button_handler(void *p) 
{

    BaseType_t higherPriorityTaskWokenUp = pdFALSE;
    vTaskNotifyGiveFromISR(buzzerTaskHandler,&higherPriorityTaskWokenUp);

    if(higherPriorityTaskWokenUp == pdTRUE) {
        portYIELD_FROM_ISR();
    }

}

void wifiScan() 
{
    while(1) {

        wifi_scan_config_t config = {
            .ssid = NULL,
            .bssid = NULL,
            .channel = 0,
            .show_hidden = true,
            .scan_type = WIFI_SCAN_TYPE_ACTIVE
        };
        uint16_t number = 5;
        wifi_ap_record_t ap_records[number];

        esp_wifi_scan_start(&config,true);
        esp_wifi_scan_get_ap_records(&number,ap_records);
        ESP_LOGI("WIFI","Found the AP's Their Count is: %d",number);

        for (int i=0; i < number; i++) {
            ESP_LOGI("SSID: ","%s",ap_records[i].ssid);
        };

        int center, top, bottom;
        char lineChar[20];

        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        for (int i=0; i < number; i++) {
            char *ssidStr = (char *)ap_records[i].ssid;

            ssd1306_display_text(&dev, i, ssidStr, strlen(ssidStr), false);
        };
        

        vTaskDelay(pdMS_TO_TICKS(1000)); //wait 1 sec and then start over again 
    };
    
}

void buzzerTask() 
{

    ESP_LOGI("Buzzer","Waiting for the Notification from ISR!");
    
    while(1) {
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(50));
        const char *msg = "BELL RINGS !!";
        ssd1306_display_text(&dev, 6, msg, strlen(msg), true);
        ESP_LOGI("Buzzer","GOt Notification from the ISR Making Buzzer go BOOOO NOW!!");
        gpio_reset_pin(BUZZER_PIN);
        gpio_set_direction(BUZZER_PIN,GPIO_MODE_OUTPUT);
        gpio_set_level(BUZZER_PIN,1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(BUZZER_PIN,0);
    }
}

void flashingLED() {
    gpio_reset_pin(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2,GPIO_MODE_OUTPUT);

    while(1) {
        gpio_set_level(GPIO_NUM_2,1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(GPIO_NUM_2,0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main() {


    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t confg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&confg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 64);

    xTaskCreatePinnedToCore(
    buzzerTask,
    "Buzzer Task",
    5048,
    NULL,
    10,
    &buzzerTaskHandler,
    1 // PINNED TO CORE 1
   );

    xTaskCreatePinnedToCore(
    flashingLED,
    "Flashing Task",
    5048,
    NULL,
    12,
    NULL,
    1 // PINNED TO CORE 1
   );
   
    xTaskCreatePinnedToCore(
    wifiScan,
    "wifi Scan Display",
    5048,
    NULL,
    10,
    NULL,
    0 // PINNED TO CORE 0
   );

    

   gpio_config_t io_conf = {};

    io_conf.intr_type = GPIO_INTR_NEGEDGE;      // Trigger on falling edge (button press)
    io_conf.mode = GPIO_MODE_INPUT;             // Set as input
    io_conf.pin_bit_mask = (1ULL << 18); // Set the pin mask for GPIO 1
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;    // Enable pull-up resistor
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;

    gpio_config(&io_conf);

    gpio_install_isr_service(0);

    gpio_isr_handler_add(18,isr_button_handler,NULL);

}