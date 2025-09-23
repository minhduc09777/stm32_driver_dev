#include "stm32_adc.h"

void init_pin_adc3();
void supply_clock_adc3();

uint32_t g_adc_data = 0;

stm32_adc_ctrl_t g_adc_instance;
stm32_adc_cfg_t g_adc_cfg = {
		.unit = 3,
		.resolution = STM32_ADC_16BIT_RESOLUTION,
		.conversion_mode = STM32_ADC_SINGLE_CONVERSION
};

stm32_adc_pin_cfg_t g_adc_pin_cfg = {
		.regular_sequence_length = 1,
		.regular_channel_seq =
		{
			[0] = STM32_ADC_CHANNEL_9,
		}
};


void supply_clock_adc3()
{
	PERI_RCC->AHB4ENR_bits.ADC3EN_bit = 1;
}

void init_pin_adc3()
{
	/* ADC3, PF4 */
	PERI_RCC->AHB4ENR_bits.GPIOFEN_bit = 1;

	PERI_GPIOF->MODER_bits.MODE4_bit = 3;
}


int main(void)
{
	init_pin_adc3();

	supply_clock_adc3();

	STM32_adc_init(&g_adc_instance, &g_adc_cfg);

	STM32_adc_pin_cfg(&g_adc_instance, &g_adc_pin_cfg);

    /* Loop forever */
	for(;;)
	{
		STM32_adc_start(&g_adc_instance);

		STM32_adc_read(&g_adc_instance, &g_adc_data);
	}
}
