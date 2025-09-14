#include "stm32_uart.h"

uint32_t system_clock = 64000000;

stm32_uart_ctrl_t g_uart_instance;

stm32_tx_cfg_t g_uart_tx_cfg = {
 .channel  = 4,
 .parity   = STM32_UART_ODD_PARITY,
 .stop_bit = STM32_UART_2BIT_STOP,
 .tx_baudrate = 115200,
};

stm32_rx_cfg_t g_uart_rx_cfg = {
 .channel  = 4,
 .parity   = STM32_UART_ODD_PARITY,
 .stop_bit = STM32_UART_2BIT_STOP,
 .rx_baudrate = 115200,
};

stm32_cfg_t g_uart_cfg = {
 .uart_tx_cfg = &g_uart_tx_cfg,
 .uart_rx_cfg = &g_uart_rx_cfg
};

void supply_clock_uart4()
{
	PERI_RCC->APB1LENR_bits.UART4EN_bit = 1;
}

void init_pin_uart4()
{
	/* TX(PC10), RX(PC11) */
	PERI_RCC->AHB4ENR_bits.GPIOCEN_bit = 1;

	PERI_GPIOC->MODER_bits.MODE10_bit = 2;
	PERI_GPIOC->MODER_bits.MODE11_bit = 2;

	/* AF8 */
	PERI_GPIOC->AFRH_bits.AFSEL10_bit = 8;
	PERI_GPIOC->AFRH_bits.AFSEL11_bit = 8;
}

uint8_t data_transfer[] = "hello world\r\n";
uint8_t data_read[10] = {0};

int main(void)
{
	init_pin_uart4();
	supply_clock_uart4();

	STM32_uart_init(&g_uart_instance, &g_uart_cfg);

	STM32_uart_transmit_polling(&g_uart_instance, data_transfer, 13);

    /* Loop forever */
	for(;;)
	{
		STM32_uart_receive_polling(&g_uart_instance, data_read, 1);
		STM32_uart_transmit_polling(&g_uart_instance, data_read, 1);
	}
}
