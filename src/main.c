/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *  @brief >1 Mbps testing w uart00 on nrf54l15 soc
 */
#include <zephyr/kernel.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/uart.h>

#define SLEEP_TIME_MS 200
#define RECEIVE_BUF_SIZE 10
#define TRANSMIT_BUF_SIZE 8
#define RECEIVE_TIMEOUT 1000

#define TRANSMITTER true

const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart00));
static uint8_t rx_buf[RECEIVE_BUF_SIZE] = {0};
static uint8_t tx_buf[TRANSMIT_BUF_SIZE] = {0x00, 0x0B, 0x0A, 0x0D, 0x0C, 0x00, 0x0D, 0x0E};
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
        switch (evt->type)
        {
        case UART_TX_DONE:
                break;
        case UART_TX_ABORTED:
                break;
        case UART_RX_RDY:
                uart_tx(uart, evt->data.rx.buf, evt->data.rx.len, 2000);
                break;
        case UART_RX_BUF_REQUEST:
                break;
        case UART_RX_BUF_RELEASED:
                break;
        case UART_RX_DISABLED:
                uart_rx_enable(dev, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
                break;
        case UART_RX_STOPPED:
                break;
        default:
                break;
        }
}

int main(void)
{
        int ret = 0;
        ret =  device_init(uart);
        if(ret)
        {
                return 1; // unable to init
        }
        if (!device_is_ready(uart))
        {
                return 2; // dev not ready
        }
        ret = uart_callback_set(uart, uart_cb, NULL);
        if (ret)
        {
                return 3; // issue setting cb (check irq_conn)
        }
        uart_rx_enable(uart, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);

        if (!TRANSMITTER)
        {
                ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_MS);
                k_msleep(10);
                while (1)
                {
                        k_yield();
                }
        }
        else
        {
                for(;;)
                {
                        ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_MS);
                        k_msleep(SLEEP_TIME_MS);
                }
        }

        return 0;
}
