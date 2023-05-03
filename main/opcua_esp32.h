#include <stdio.h>
#include <sys/param.h>
#include <unistd.h>
#include "esp_log.h"
#include "esp_netif.h"
#include <esp_flash_encrypt.h>
#include <esp_task_wdt.h>
#include <esp_sntp.h>
#include "esp_event.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "nvs_flash.h"
#include "lwip/ip_addr.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "driver/ledc.h"


#include "ethernet_connect.h"
#include "open62541.h"
#include "DHT22.h"
#include "model.h"
#include "accelstepper_wrapper.h"
#include "stepper_motor_encoder.h"
#include "stepper_motor_control.h"
/*-----------------------------------*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/*-----------*/
#include "freertos/semphr.h"
/*------------------*/
#include "driver/rmt_tx.h"


/*------------------------------------*/
