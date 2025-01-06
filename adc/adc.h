#include <string.h>

/* lib_adc.c */
void ADC_init (void);
void ADC_start_conversion (void);
unsigned short read_AD_current();

/* IRQ_adc.c */
void ADC_IRQHandler(void);
