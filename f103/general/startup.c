#include <stdint.h>

#define SRAM_START (0x20000000U)
#define SRAM_SIZE (20 * 1024U)
/*#define SRAM_SIZE 0x00005000*/
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)

#define ISR_VECTOR_SIZE_WORDS 84

void default_handler(void);

void reset_handler(void);
void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hard_fault_handler(void) __attribute__((weak, alias("default_handler")));
void mem_manage_handler(void) __attribute__((weak, alias("default_handler")));
void bus_fault_handler(void) __attribute__((weak, alias("default_handler")));
void usage_fault_handler(void) __attribute__((weak, alias("default_handler")));
/* 0 */
/* 0 */
/* 0 */
/* 0 */
void svcall_handler(void) __attribute__((weak, alias("default_handler")));
void debug_monitor_handler(void) __attribute__((weak, alias("default_handler")));
/* 0 */
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void systick_handler(void) __attribute__((weak, alias("default_handler")));
void wwdgt_handler(void) __attribute__((weak, alias("default_handler")));
void lvd_handler(void) __attribute__((weak, alias("default_handler")));
void tamper_handler(void) __attribute__((weak, alias("default_handler")));
void rtc_handler(void) __attribute__((weak, alias("default_handler")));
void fmc_handler(void) __attribute__((weak, alias("default_handler")));
void rcu_ctc_handler(void) __attribute__((weak, alias("default_handler")));
void exti0_handler(void) __attribute__((weak, alias("default_handler")));
void exti1_handler(void) __attribute__((weak, alias("default_handler")));
void exti2_handler(void) __attribute__((weak, alias("default_handler")));
void exti3_handler(void) __attribute__((weak, alias("default_handler")));
void exti4_handler(void) __attribute__((weak, alias("default_handler")));
void dma0_channel0_handler(void) __attribute__((weak, alias("default_handler")));
void dma0_channel1_handler(void) __attribute__((weak, alias("default_handler")));
void dma0_channel2_handler(void) __attribute__((weak, alias("default_handler")));
void dma0_channel3_handler(void) __attribute__((weak, alias("default_handler")));
void dma0_channel4_handler(void) __attribute__((weak, alias("default_handler")));
void dma0_channel5_handler(void) __attribute__((weak, alias("default_handler")));
void dma0_channel6_handler(void) __attribute__((weak, alias("default_handler")));
void adc0_adc1_handler(void) __attribute__((weak, alias("default_handler")));
void can1_tx_handler(void) __attribute__((weak, alias("default_handler")));
void can1_rx0_handler(void) __attribute__((weak, alias("default_handler")));
void can1_rx1_handler(void) __attribute__((weak, alias("default_handler")));
void can1_sce_handler(void) __attribute__((weak, alias("default_handler")));
void exti9_5_handler(void) __attribute__((weak, alias("default_handler")));
void tim1_brk_handler(void) __attribute__((weak, alias("default_handler")));
void tim1_up_handler(void) __attribute__((weak, alias("default_handler")));
void tim1_trg_com_handler(void) __attribute__((weak, alias("default_handler")));
void tim1_cc_handler(void) __attribute__((weak, alias("default_handler")));
void tim2_handler(void) __attribute__((weak, alias("default_handler")));
void tim3_handler(void) __attribute__((weak, alias("default_handler")));
void tim4_handler(void) __attribute__((weak, alias("default_handler")));
void i2c1_ev_handler(void) __attribute__((weak, alias("default_handler")));
void i2c1_err_handler(void) __attribute__((weak, alias("default_handler")));
void i2c2_ev_handler(void) __attribute__((weak, alias("default_handler")));
void i2c2_err_handler(void) __attribute__((weak, alias("default_handler")));
void spi1_handler(void) __attribute__((weak, alias("default_handler")));
void spi2_handler(void) __attribute__((weak, alias("default_handler")));
void usart1_handler(void) __attribute__((weak, alias("default_handler")));
void usart2_handler(void) __attribute__((weak, alias("default_handler")));
void usart3_handler(void) __attribute__((weak, alias("default_handler")));
void exti15_10_handler(void) __attribute__((weak, alias("default_handler")));
void rtc_alarm_handler(void) __attribute__((weak, alias("default_handler")));
void usbfs_wkup_handler(void) __attribute__((weak, alias("default_handler")));
/*0*/
/*0*/
/*0*/
/*0*/
/*0*/
/*0*/
void tim5_handler(void) __attribute__((weak, alias("default_handler")));
void spi3_handler(void) __attribute__((weak, alias("default_handler")));
void uart4_handler(void) __attribute__((weak, alias("default_handler")));
void uart5_handler(void) __attribute__((weak, alias("default_handler")));
void tim6_handler(void) __attribute__((weak, alias("default_handler")));
void tim7_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_channel0_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_channel1_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_channel2_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_channel3_handler(void) __attribute__((weak, alias("default_handler")));
void dma1_channel4_handler(void) __attribute__((weak, alias("default_handler")));
void eth_handler(void) __attribute__((weak, alias("default_handler")));
void eth_wkup_handler(void) __attribute__((weak, alias("default_handler")));
void can2_tx_handler(void) __attribute__((weak, alias("default_handler")));
void can2_rx0_handler(void) __attribute__((weak, alias("default_handler")));
void can2_rx1_handler(void) __attribute__((weak, alias("default_handler")));
void can2_sce_handler(void) __attribute__((weak, alias("default_handler")));
void otg_fs_handler(void) __attribute__((weak, alias("default_handler")));

uint32_t isr_vector[ISR_VECTOR_SIZE_WORDS] __attribute__((section(".isr_vector"))) = {
	STACK_POINTER_INIT_ADDRESS,
	(uint32_t)&reset_handler,
	(uint32_t)&nmi_handler,
	(uint32_t)&hard_fault_handler,
	(uint32_t)&mem_manage_handler,
	(uint32_t)&bus_fault_handler,
	(uint32_t)&usage_fault_handler,
	0,
	0,
	0,
	0,
	(uint32_t)&svcall_handler,
	(uint32_t)&debug_monitor_handler,
	0,
	(uint32_t)&pendsv_handler,
	(uint32_t)&systick_handler,
	(uint32_t)&wwdgt_handler,
	(uint32_t)&lvd_handler,
	(uint32_t)&tamper_handler,
	(uint32_t)&rtc_handler,
	(uint32_t)&fmc_handler,
	(uint32_t)&rcu_ctc_handler,
	(uint32_t)&exti0_handler,
	(uint32_t)&exti1_handler,
	(uint32_t)&exti2_handler,
	(uint32_t)&exti3_handler,
	(uint32_t)&exti4_handler,
	(uint32_t)&dma0_channel0_handler,
	(uint32_t)&dma0_channel1_handler,
	(uint32_t)&dma0_channel2_handler,
	(uint32_t)&dma0_channel3_handler,
	(uint32_t)&dma0_channel4_handler,
	(uint32_t)&dma0_channel5_handler,
	(uint32_t)&dma0_channel6_handler,
	(uint32_t)&adc0_adc1_handler,
	(uint32_t)&can1_tx_handler,
	(uint32_t)&can1_rx0_handler,
	(uint32_t)&can1_rx1_handler,
	(uint32_t)&can1_sce_handler,
	(uint32_t)&exti9_5_handler,
	(uint32_t)&tim1_brk_handler,
	(uint32_t)&tim1_up_handler,
	(uint32_t)&tim1_trg_com_handler,
	(uint32_t)&tim1_cc_handler,
	(uint32_t)&tim2_handler,
	(uint32_t)&tim3_handler,
	(uint32_t)&tim4_handler,
	(uint32_t)&i2c1_ev_handler,
	(uint32_t)&i2c1_err_handler,
	(uint32_t)&i2c2_ev_handler,
	(uint32_t)&i2c2_err_handler,
	(uint32_t)&spi1_handler,
	(uint32_t)&spi2_handler,
	(uint32_t)&usart1_handler,
	(uint32_t)&usart2_handler,
	(uint32_t)&usart3_handler,
	(uint32_t)&exti15_10_handler,
	(uint32_t)&rtc_alarm_handler,
	(uint32_t)&usbfs_wkup_handler,
	/*0*/
	/*0*/
	/*0*/
	/*0*/
	/*0*/
	/*0*/
	/*0*/
	(uint32_t)&tim5_handler,
	(uint32_t)&spi3_handler,
	(uint32_t)&uart4_handler,
	(uint32_t)&uart5_handler,
	(uint32_t)&tim6_handler,
	(uint32_t)&tim7_handler,
	(uint32_t)&dma1_channel0_handler,
	(uint32_t)&dma1_channel1_handler,
	(uint32_t)&dma1_channel2_handler,
	(uint32_t)&dma1_channel3_handler,
	(uint32_t)&dma1_channel4_handler,
	(uint32_t)&eth_handler,
	(uint32_t)&eth_wkup_handler,
	(uint32_t)&can2_tx_handler,
	(uint32_t)&can2_rx0_handler,
	(uint32_t)&can2_rx1_handler,
	(uint32_t)&can2_sce_handler,
	(uint32_t)&otg_fs_handler
};

void default_handler(void)
{
	while(1) {}
	/*return;*/
}

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;
int main(void);

void reset_handler(void)
{
	uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
	uint8_t *flash_data = (uint8_t *)&_etext;
	uint8_t *sram_data = (uint8_t *)&_sdata;

	for(uint32_t i = 0; i < data_size; i++)
		sram_data[i] = flash_data[i];

	uint32_t bss_size = (uint32_t)&_sbss - (uint32_t)&_sbss;
	uint8_t *bss = (uint8_t *)&_sbss;

	for(uint32_t i = 0; i < bss_size; i++)
		bss[i] = 0;

	main();
}
