/**
 * @file stm32_adc.h
 * @brief ADC configuration and control structures for STM32H743 series.
 *
 * This header defines data structures and enumerations used for configuring
 * and controlling ADC peripherals on STM32H743 microcontrollers.
 *
 * @date 2025-09-20
 * @author Duc Nguyen
 */

#ifndef STM32_ADC_H
#define STM32_ADC_H

#include "STM32H743_io.h"

/**
 * @enum stm32_adc_channel_t
 * @brief ADC input channel indices for STM32H743.
 *
 * Enumerates logical ADC channels (as used in sequence configuration).
 * The actual pin mapping depends on the selected ADC instance and device package.
 */
typedef enum st_stm32_adc_channel
{
    STM32_ADC_CHANNEL_0  = 0,  /**< ADC Channel 0  */
    STM32_ADC_CHANNEL_1  = 1,  /**< ADC Channel 1  */
    STM32_ADC_CHANNEL_2  = 2,  /**< ADC Channel 2  */
    STM32_ADC_CHANNEL_3  = 3,  /**< ADC Channel 3  */
    STM32_ADC_CHANNEL_4  = 4,  /**< ADC Channel 4  */
    STM32_ADC_CHANNEL_5  = 5,  /**< ADC Channel 5  */
    STM32_ADC_CHANNEL_6  = 6,  /**< ADC Channel 6  */
    STM32_ADC_CHANNEL_7  = 7,  /**< ADC Channel 7  */
    STM32_ADC_CHANNEL_8  = 8,  /**< ADC Channel 8  */
    STM32_ADC_CHANNEL_9  = 9,  /**< ADC Channel 9  */
    STM32_ADC_CHANNEL_10 = 10, /**< ADC Channel 10 */
    STM32_ADC_CHANNEL_11 = 11, /**< ADC Channel 11 */
    STM32_ADC_CHANNEL_12 = 12, /**< ADC Channel 12 */
    STM32_ADC_CHANNEL_13 = 13, /**< ADC Channel 13 */
    STM32_ADC_CHANNEL_14 = 14, /**< ADC Channel 14 */
    STM32_ADC_CHANNEL_15 = 15, /**< ADC Channel 15 */
    STM32_ADC_CHANNEL_16 = 16, /**< ADC Channel 16 */
    STM32_ADC_CHANNEL_17 = 17, /**< ADC Channel 17 */
    STM32_ADC_CHANNEL_18 = 18, /**< ADC Channel 18 */
    STM32_ADC_CHANNEL_19 = 19, /**< ADC Channel 19 */
} stm32_adc_channel_t;

/**
 * @enum stm32_adc_resolution_t
 * @brief ADC conversion resolution selection.
 *
 * Encodes the hardware resolution setting for the ADC. The numeric values
 * correspond to device register encodings (not the bit-width itself).
 *
 * @note The effective resolution (in bits) is given in each enumerator's comment.
 */
typedef enum e_stm32_adc_resolution
{
    STM32_ADC_16BIT_RESOLUTION = 0, /**< 16-bit conversion resolution */
    STM32_ADC_14BIT_RESOLUTION = 5, /**< 14-bit conversion resolution */
    STM32_ADC_12BIT_RESOLUTION = 6, /**< 12-bit conversion resolution */
    STM32_ADC_10BIT_RESOLUTION = 3, /**< 10-bit conversion resolution */
    STM32_ADC_8BIT_RESOLUTION  = 7, /**< 8-bit  conversion resolution */
} stm32_adc_resolution_t;

/**
 * @enum stm32_adc_conversion_t
 * @brief ADC conversion mode selection.
 *
 * Selects between single-shot and continuous conversion modes.
 */
typedef enum e_stm32_adc_conversion
{
    STM32_ADC_SINGLE_CONVERSION     = 0, /**< Single conversion mode */
    STM32_ADC_CONTINUOUS_CONVERSION = 1, /**< Continuous conversion mode */
} stm32_adc_conversion_t;

/**
 * @struct stm32_adc_cfg_t
 * @brief Static configuration for an ADC instance.
 *
 * Provides high-level ADC settings that typically remain constant after initialization.
 */
typedef struct st_stm32_adc_cfg
{
    uint8_t                unit;             /**< ADC unit index (e.g., 1 for ADC1, 2 for ADC2, ...). */
    stm32_adc_resolution_t resolution;       /**< ADC conversion resolution (8/10/12/14/16 bits). */
    stm32_adc_conversion_t conversion_mode;  /**< Conversion mode: single or continuous. */
} stm32_adc_cfg_t;

/**
 * @struct stm32_adc_pin_cfg_t
 * @brief Regular conversion sequence configuration.
 *
 * Describes the regular channel sequence (SQ1..SQn) used during conversions.
 */
typedef struct st_stm32_adc_pin_cfg
{
    uint8_t             regular_sequence_length; /**< Number of sequence slots used (starting at SQ1). */
    stm32_adc_channel_t regular_channel_seq[15]; /**< Channel list mapped to SQ1..SQ15 (index 0 -> SQ1). */
} stm32_adc_pin_cfg_t;

/**
 * @struct stm32_adc_ctrl_t
 * @brief Runtime control block for an ADC instance.
 *
 * Holds pointers to configuration and the base addresses for the ADC registers.
 * This structure is typically owned by the driver and passed to API calls.
 */
typedef struct st_stm32_adc_d_ctrl
{
    stm32_adc_cfg_t     *adc_cfg;          /**< Pointer to static ADC configuration. */
    stm32_adc_pin_cfg_t *adc_pin_cfg;      /**< Pointer to regular sequence configuration. */
    uint32_t            *p_adc_reg;        /**< Base address of the ADC instance registers. */
    uint32_t            *p_adc_common_reg; /**< Base address of the ADC common registers. */
} stm32_adc_ctrl_t;

/** @brief Opaque instance control marker for compatibility with generic driver code. */
typedef void stm32_instance_ctrl;
uint8_t STM32_adc_init (stm32_instance_ctrl *ptrl, stm32_adc_cfg_t *p_cfg);
uint8_t STM32_adc_start (stm32_instance_ctrl *ptrl);
uint8_t STM32_adc_read (stm32_instance_ctrl *ptrl, uint32_t *data);

uint8_t STM32_adc_pin_cfg (stm32_instance_ctrl *ptrl, stm32_adc_pin_cfg_t *p_pin_cfg);
#endif // STM32_ADC_H
