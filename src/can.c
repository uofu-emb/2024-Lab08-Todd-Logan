#include "can2040.h"
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

int task_id = 1
;


static struct can2040 cbus;
QueueHandle_t message;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    xQueueSendToBack(message, msg, portMAX_DELAY);
}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

void canbus_setup(void)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = 8, gpio_tx = 7;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, PICO_DEFAULT_IRQ_PRIORITY - 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

void sender_task(__unused void *params)
{
    struct can2040_msg msg;

    if(task_id ==1) {
        msg.id = 0x1;
    }
    else{
        msg.id = 0x2;
    }
    msg.dlc = 1;
    msg.data[0] = 0x01;

    while (1) {
        if (can2040_transmit(&cbus, &msg))
        {
            printf("If Transmission failed.\n");
        }
        else
        {
            printf("Else Transmission sent.\n");
        }
        sleep_ms(1000);
     
    }
}

void receiver_task(__unused void *params)
{
    struct can2040_msg data;
    while (1) {

        xQueueReceive(message, &data, portMAX_DELAY);
        printf("Got message\n");
    }
}

int main(void)
{
    stdio_init_all();
    canbus_setup();
    TaskHandle_t sender_task_handle, receiver_task_handle;
    xTaskCreate(sender_task, "TxThread",
                configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1UL, &sender_task_handle);
             //   sleep_ms(1000);
    xTaskCreate(receiver_task, "RxThread",
               configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1UL, &receiver_task_handle);
    vTaskStartScheduler();
    return 0;
}