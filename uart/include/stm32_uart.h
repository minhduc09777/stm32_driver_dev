/**
 * @file stm32_uart.h
 * @brief UART configuration and control structures for STM32H743 series.
 *
 * This header defines data structures and enumerations used for configuring
 * and controlling UART peripherals on STM32H743 microcontrollers.
 *
 * @date 2025-09-13
 * @author Duc Nguyen
 */

#ifndef STM32_UART_H
#define STM32_UART_H

#include "STM32H743_io.h"

/**
 * @enum uart_stop_bit_t
 * @brief UART stop bit configuration.
 *
 * This enumeration defines the different stop bit modes available for UART communication.
 */
typedef enum enum_uart_stop_bit
{
    STM32_UART_1BIT_STOP,        /**< 1 stop bit. */
    STM32_UART_1BIT_HALF_STOP,   /**< 1.5 stop bits. */
    STM32_UART_2BIT_STOP,        /**< 2 stop bits. */
    STM32_UART_2BIT_HALF_STOP    /**< 2.5 stop bits (if supported). */
} uart_stop_bit_t;

/**
 * @enum uart_parity_t
 * @brief UART parity configuration.
 *
 * This enumeration defines the supported UART parity modes.
 */
typedef enum enum_uart_parity
{
    STM32_UART_EVENT_PARITY, /**< Even parity. */
    STM32_UART_ODD_PARITY    /**< Odd parity. */
} uart_parity_t;

/**
 * @struct stm32_tx_cfg_t
 * @brief UART TX configuration structure.
 *
 * This structure contains parameters used to configure UART transmission.
 */
typedef struct st_stm32_tx_cfg
{
    uint32_t        channel;     /**< UART channel index (e.g., USART1 = 1). */
    uint32_t        tx_baudrate;
    uart_stop_bit_t stop_bit;    /**< Stop bit configuration, see @ref uart_stop_bit_t. */
    uart_parity_t   parity;      /**< Parity configuration, see @ref uart_parity_t. */
    void           *extend_cfg;  /**< Pointer to extended configuration for custom use. */
} stm32_tx_cfg_t;

/**
 * @struct stm32_rx_cfg_t
 * @brief UART TX configuration structure.
 *
 * This structure contains parameters used to configure UART transmission.
 */
typedef struct st_stm32_rx_cfg
{
    uint32_t        channel;     /**< UART channel index (e.g., USART1 = 1). */
    uint32_t        rx_baudrate;
    uart_stop_bit_t stop_bit;    /**< Stop bit configuration, see @ref uart_stop_bit_t. */
    uart_parity_t   parity;      /**< Parity configuration, see @ref uart_parity_t. */
    void           *extend_cfg;  /**< Pointer to extended configuration for custom use. */
} stm32_rx_cfg_t;

/**
 * @struct stm32_cfg_t
 * @brief UART configuration container.
 *
 * This structure is a wrapper to hold UART configuration structures.
 */
typedef struct st_stm32_cfg
{
    stm32_tx_cfg_t *uart_tx_cfg; /**< Pointer to UART TX configuration. */
    stm32_rx_cfg_t *uart_rx_cfg; /**< Pointer to UART RX configuration. */
} stm32_cfg_t;

/**
 * @struct stm32_uart_ctrl_t
 * @brief UART control structure.
 *
 * This structure is used to manage UART peripheral operations at runtime.
 */
typedef struct st_stm32_uart_ctrl
{
    stm32_cfg_t *uart_cfg;        /**< Pointer to the main UART configuration structure. */
    uint32_t    *p_tx_uart_reg;   /**< Pointer to the UART register base address. */
    uint32_t    *p_rx_uart_reg;   /**< Pointer to the UART register base address. */
} stm32_uart_ctrl_t;

typedef void stm32_instance_ctrl;

extern uint32_t system_clock;
uint8_t STM32_uart_init (stm32_instance_ctrl *ptrl, stm32_cfg_t *p_cfg);
uint8_t STM32_uart_transmit_polling (stm32_instance_ctrl *ptrl, uint8_t *p_src, uint32_t lenght);
uint8_t STM32_uart_receive_polling (stm32_instance_ctrl *ptrl, uint8_t *p_dest, uint32_t lenght);
#endif // STM32_UART_H
