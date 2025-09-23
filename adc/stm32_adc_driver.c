#include "stm32_adc.h"

#define STM32_ADC_BASE_ADDRESS            PERI_ADC1
#define STM32_ADC_BASE_UNIT               1
#define STM32_ADC_UNIT_OFFSET             ((uint32_t)PERI_ADC2-(uint32_t)PERI_ADC1)
#define STM32_ADC_COMMON_UNIT_OFFSET      ((uint32_t)PERI_ADC12_COMMON-(uint32_t)PERI_ADC1)
#define STM32_ADC_BUS_OFFSET              ((uint32_t)PERI_ADC3-(uint32_t)STM32_ADC_BASE_ADDRESS)
#define STM32_ADC_BUS_UNIT_STEP           3
#define STM32_ADC_REG_TYPE                adc3_t
#define STM32_ADC_COMMON_REG_TYPE         adc3_common_t

uint8_t STM32_adc_init(stm32_instance_ctrl *ptrl, stm32_adc_cfg_t *p_cfg)
{

    stm32_adc_ctrl_t *p_instance_ctrl = (stm32_adc_ctrl_t *) ptrl;
    p_instance_ctrl->adc_cfg = p_cfg;

    uint32_t unit = (uint32_t) p_instance_ctrl->adc_cfg->unit;

    uint32_t offset = ((unit - (uint32_t) STM32_ADC_BASE_UNIT)%(STM32_ADC_BUS_UNIT_STEP-1)) * (uint32_t) STM32_ADC_UNIT_OFFSET + 
                       (uint32_t) (unit/STM32_ADC_BUS_UNIT_STEP) * (uint32_t) STM32_ADC_BUS_OFFSET;

    p_instance_ctrl->p_adc_reg = (uint32_t *) ( (uint32_t) STM32_ADC_BASE_ADDRESS + offset);
    p_instance_ctrl->p_adc_common_reg = (uint32_t *) ( (uint32_t) STM32_ADC_COMMON_UNIT_OFFSET + (uint32_t) p_instance_ctrl->p_adc_reg);

    STM32_ADC_REG_TYPE *p_reg = (STM32_ADC_REG_TYPE *) p_instance_ctrl->p_adc_reg;

    p_reg->CR_reg = 0;
    p_reg->CR_bits.ADVREGEN_bit = 1;
    for (volatile int i = 0; i < 1000; i++);

    while (p_reg->ISR_bits.LDORDY_bit == 0);

    // 010: 47.5 ADC cycles
    p_reg->CFGR_bits.CONT_bit = p_instance_ctrl->adc_cfg->conversion_mode;
    p_reg->CFGR_bits.RES_bit  = p_instance_ctrl->adc_cfg->resolution;

    return 0;
}

uint8_t STM32_adc_pin_cfg(stm32_instance_ctrl *ptrl, stm32_adc_pin_cfg_t *p_pin_cfg)
{
    stm32_adc_ctrl_t *p_instance_ctrl = (stm32_adc_ctrl_t *) ptrl;
    p_instance_ctrl->adc_pin_cfg = p_pin_cfg;

    STM32_ADC_REG_TYPE *p_reg = (STM32_ADC_REG_TYPE *) p_instance_ctrl->p_adc_reg;

    if (p_instance_ctrl->adc_pin_cfg->regular_sequence_length > 16 || p_instance_ctrl->adc_pin_cfg->regular_sequence_length < 1)
    {
        return 1; // invalid length
    }

    p_reg->SQR1_bits.L3_bit = p_instance_ctrl->adc_pin_cfg->regular_sequence_length - 1;

    uint32_t *SQR_array[4] = {(uint32_t *) &p_reg->SQR1_reg, (uint32_t *) &p_reg->SQR2_reg, (uint32_t *) &p_reg->SQR3_reg, (uint32_t *) &p_reg->SQR4_reg};
    for (uint8_t i = 1; i <= p_instance_ctrl->adc_pin_cfg->regular_sequence_length; i++)
    {
        *(SQR_array[i/5]) &= ~(0x1F << ((i % 5) * 6)); // clear bits
        *(SQR_array[i/5]) |= (p_instance_ctrl->adc_pin_cfg->regular_channel_seq[i-1] << ((i%5) * 6));
    }

    // temp
    STM32_ADC_COMMON_REG_TYPE *p_common_reg = (STM32_ADC_COMMON_REG_TYPE *) p_instance_ctrl->p_adc_common_reg;
    p_common_reg->CCR_reg &= ~(3U << 16);
    p_common_reg->CCR_reg |=  (2U << 16);  // CKMODE = HCLK/2

    p_reg->ISR_bits.ADRDY_bit = 1;
    p_reg->CR_bits.ADEN_bit = 1; // enable ADC

    while (p_reg->ISR_bits.ADRDY_bit == 0); // wait until ADC is ready

    return 0;
}

uint8_t STM32_adc_start(stm32_instance_ctrl *ptrl)
{
    stm32_adc_ctrl_t *p_instance_ctrl = (stm32_adc_ctrl_t *) ptrl;

    STM32_ADC_REG_TYPE *p_reg = (STM32_ADC_REG_TYPE *) p_instance_ctrl->p_adc_reg;

    p_reg->CR_bits.ADSTART_bit = 1; // start conversion

    return 0;
}

uint8_t STM32_adc_read(stm32_instance_ctrl *ptrl, uint32_t *data)
{
    stm32_adc_ctrl_t *p_instance_ctrl = (stm32_adc_ctrl_t *) ptrl;

    STM32_ADC_REG_TYPE *p_reg = (STM32_ADC_REG_TYPE *) p_instance_ctrl->p_adc_reg;

    while (p_reg->ISR_bits.EOC_bit == 0); // wait until conversion is complete

    *data = p_reg->DR_reg;

    return 0;
}
