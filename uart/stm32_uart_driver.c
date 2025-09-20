#include "stm32_uart.h"

#define STM32_UART_BASE_ADDRESS  PERI_UART4
#define STM32_UART_BASE_CHANNEL  (PERI_UART4-PERI_UART5)
#define STM32_UART_OFFSET 0x400
#define STM32_UART_REG_TYPE uart4_t

static uint32_t calculate_baudrate (uint32_t baudrate);

uint8_t STM32_uart_init(stm32_instance_ctrl *ptrl, stm32_cfg_t *p_cfg)
{
	/* TODO: check invalid  ptrl */
	stm32_uart_ctrl_t *p_instance_ctrl = (stm32_uart_ctrl_t *) ptrl;

	p_instance_ctrl->uart_cfg = p_cfg;
	/*TODO: check invalid config*/

	uint32_t base_addess = (uint32_t ) STM32_UART_BASE_ADDRESS;

	if (p_instance_ctrl->uart_cfg->uart_tx_cfg != 0)
	{
		uint32_t txchannel = (uint32_t) p_instance_ctrl->uart_cfg->uart_tx_cfg->channel;
		p_instance_ctrl->p_tx_uart_reg = (uint32_t *) (base_addess + (txchannel-STM32_UART_BASE_CHANNEL) * (uint32_t) STM32_UART_OFFSET);
	}

	/* TODO: Duplicate block code shoule improve */
	if (p_instance_ctrl->uart_cfg->uart_rx_cfg != 0)
	{
		uint32_t rxchannel = (uint32_t) p_instance_ctrl->uart_cfg->uart_rx_cfg->channel;
		p_instance_ctrl->p_rx_uart_reg = (uint32_t *) (base_addess + (rxchannel-STM32_UART_BASE_CHANNEL) * (uint32_t) STM32_UART_OFFSET);
	}

	if (p_instance_ctrl->p_tx_uart_reg)
	{
		STM32_UART_REG_TYPE *p_reg = (STM32_UART_REG_TYPE *) p_instance_ctrl->p_tx_uart_reg;
		p_reg->CR2_bits.STOP_bit = p_instance_ctrl->uart_cfg->uart_tx_cfg->stop_bit;
		p_reg->CR1_bits.PS_bit   = p_instance_ctrl->uart_cfg->uart_tx_cfg->parity;
		p_reg->BRR_reg = calculate_baudrate(p_instance_ctrl->uart_cfg->uart_tx_cfg->tx_baudrate);
		p_reg->CR1_bits.UE_bit   = 1;
	}

	if (p_instance_ctrl->p_rx_uart_reg)
	{
		STM32_UART_REG_TYPE *p_reg = (STM32_UART_REG_TYPE *) p_instance_ctrl->p_rx_uart_reg;
		p_reg->CR2_bits.STOP_bit = p_instance_ctrl->uart_cfg->uart_rx_cfg->stop_bit;
		p_reg->CR1_bits.PS_bit   = p_instance_ctrl->uart_cfg->uart_rx_cfg->parity;
		p_reg->BRR_reg = calculate_baudrate(p_instance_ctrl->uart_cfg->uart_rx_cfg->rx_baudrate);
		p_reg->CR1_bits.UE_bit   = 1;
	}
	return 0;
}

uint8_t STM32_uart_transmit_polling(stm32_instance_ctrl *ptrl, uint8_t *p_src, uint32_t lenght)
{
	stm32_uart_ctrl_t *p_instance_ctrl = (stm32_uart_ctrl_t *) ptrl;
	uint32_t len_transmit = 0;

	STM32_UART_REG_TYPE *p_reg = (STM32_UART_REG_TYPE *) p_instance_ctrl->p_tx_uart_reg;
	/**/
	while(p_reg->ISR_bits.TXE_bit != 1); // just make sure

	/* Enable transmit */
	p_reg->CR1_bits.TE_bit = 1;
	while(len_transmit < lenght)
	{
		p_reg->TDR_reg = p_src[len_transmit];
		while(p_reg->ISR_bits.TXE_bit != 1);
		len_transmit++;
	}

	/* Disable transmit */
	p_reg->CR1_bits.TE_bit = 0;
	return 1;
}

uint8_t STM32_uart_receive_polling(stm32_instance_ctrl *ptrl, uint8_t *p_dest, uint32_t lenght)
{
	stm32_uart_ctrl_t *p_instance_ctrl = (stm32_uart_ctrl_t *) ptrl;
	uint32_t len_receive = 0;

	STM32_UART_REG_TYPE *p_reg = (STM32_UART_REG_TYPE *) p_instance_ctrl->p_rx_uart_reg;

	/* Enable transmit */
	p_reg->CR1_bits.RE_bit = 1;
	while(len_receive < lenght)
	{
		while(p_reg->ISR_bits.RXNE_bit != 1);
		p_dest[len_receive] = p_reg->RDR_reg;
		len_receive++;
	}

	/* Disable transmit */
	p_reg->CR1_bits.RE_bit = 0;
	return 1;
}
static uint32_t calculate_baudrate(uint32_t baudrate)
{
	uint32_t brr = ((system_clock + (baudrate >> 1))/baudrate); // round up 0.5
	return brr;
}


