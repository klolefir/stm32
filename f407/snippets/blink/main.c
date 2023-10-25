#include "gpio.h"
#include "tim.h"
#include "rcc.h"

int main(void)
{
	__enable_irq();
	rcc_init();

	gpio_t led;
	led.port = gpio_port_a;
	led.mask = 6;
	led.mode = gpio_mode_output;
	led.otype = gpio_otype_pp;
	led.ospeed = gpio_ospeed_high;
	led.pupd = gpio_pupd_pd;
	led.alt = gpio_alt_system;
	gpio_init(&led);
	tim_hold_init();
	
	while(1) {
		gpio_tgl(&led);
		tim_delay_ms(500);
	}
}
