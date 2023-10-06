#include "gpio.h"
#include "tim.h"
#include "rcc.h"
#include "systick.h"

void init_gpio();
void init_tim();
void init();
void handler();
void move(uint32_t steps);
void switch_dir();

enum {
	freq = 100000,
	max_step = 80,
};
typedef enum {
	c_note = 261,
	cd_note = 277,
	d_note = 293,
	dd_note = 311,
	e_note = 329,
	f_note = 349,
	fd_note = 369,
	g_note = 392,
	gd_note = 415,
	a_note = 440,
	ad_note = 466,
	b_note = 493,

	c2_note = 523,
	c2d_note = 554,
	d2_note = 587,
	d2d_note = 622,
	e2_note = 659,
	f2_note = 698,
	f2d_note = 739,
	g2_note = 784,
	g2d_note = 830,
	a2_note = 880,
	a2d_note = 932,
	b2_note = 987,

	c3_note = 1046,
	c3d_note = 1108,
	d3_note = 1174,
	d3d_note = 1244,
	e3_note = 1318,
	f3_note = 1396,
	f3d_note = 1480,
	g3_note = 1568,
	g3d_note = 1661,
	a3_note = 1720,
	a3d_note = 1864,
	b3_note = 1975,

	c4_note = 2093,
	c4d_note = 2217,
	d4_note = 2349,
	d4d_note = 2489,
	e4_note = 2637,
	f4_note = 2793,
	f4d_note = 2960,
	g4_note = 3136,
	g4d_note = 3332,
	a4_note = 3440,
	a4d_note = 3729,
	b4_note = 3961
} note_t;

gpio_t led1, dir_pin, step_pin;
tim_t tim, timd;

int main()
{
	__enable_irq();
	rcc_init();
	init();
	handler();
	while(1) {
	}
}

void move(uint32_t steps)
{
	while(steps) {
		gpio_tgl(&step_pin);
		systick_delay_ms(1);
		steps--;
	}
}

void switch_dir()
{
	gpio_tgl(&dir_pin);
}

void init()
{
	init_gpio();
	init_tim();
}

void init_gpio()
{
	led1.port = gpio_port_a;
	led1.mask = 6;
	led1.mode = gpio_mode_output;
	led1.otype = gpio_otype_pp;
	led1.ospeed = gpio_ospeed_low;
	led1.pupd = gpio_pupd_pu;
	led1.alt = gpio_alt_system;

	dir_pin.port = gpio_port_a;
	dir_pin.mask = 1;
	dir_pin.mode = gpio_mode_output;
	dir_pin.otype = gpio_otype_pp;
	dir_pin.ospeed = gpio_ospeed_low;
	dir_pin.pupd = gpio_pupd_pu;
	dir_pin.alt = gpio_alt_system;

	step_pin.port = gpio_port_a;
	step_pin.mask = 2;
	step_pin.mode = gpio_mode_output;
	step_pin.otype = gpio_otype_pp;
	step_pin.ospeed = gpio_ospeed_low;
	step_pin.pupd = gpio_pupd_pu;
	step_pin.alt = gpio_alt_system;

	gpio_init(&led1);
	gpio_init(&dir_pin);
	gpio_init(&step_pin);
}

void init_tim()
{
	tim.tim_num 	= tim_6;
	tim.psc 		= 168 / 4;
	tim.arr 		= 10;
	tim.irq_st 		= tim_irq_on;
	tim.pwr_st 		= tim_pwr_on;

	timd.tim_num 		= tim_7;
	timd.psc 			= 168 / 4;
	timd.arr 			= 1000;
	timd.irq_st 		= tim_irq_on;
	timd.pwr_st 		= tim_pwr_on;

	tim_init(&tim);
	tim_init(&timd);
}

void handler()
{
	uint32_t curr_note = 0;
	uint32_t curr_step = 0;
	uint32_t ticks, ticksd;
	uint32_t note_freq;
	note_t note_arr[12];
	note_arr[0] = c_note;
	note_arr[1] = cd_note;
	note_arr[2] = d_note;
	note_arr[3] = dd_note;
	note_arr[4] = e_note;
	note_arr[5] = f_note;
	note_arr[6] = fd_note;
	note_arr[7] = g_note;
	note_arr[8] = gd_note;
	note_arr[9] = a_note;
	note_arr[10] = ad_note;
	note_arr[11] = b_note;
	
	while(1) {
		if(curr_step == max_step) {
			curr_step = 0;
			gpio_tgl(&dir_pin);
		}
		ticksd = tim_get_ticks(&timd);
		if(ticksd >= 1000) {
			curr_note++;
			if(curr_note == sizeof(note_arr))
				curr_note = 0;
			tim_set_ticks(&timd, 0);
		}
		ticks = tim_get_ticks(&tim);
		note_freq = freq / note_arr[curr_note];
		if(ticks >= note_freq) {
			gpio_tgl(&step_pin);
			curr_step++;
			tim_set_ticks(&tim, 0);
		}
	}
}
