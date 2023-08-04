#ifndef TIM_H_SENTRY
#define TIM_H_SENTRY

#include <stm32f4xx.h>

typedef uint32_t tim_psc_t;
typedef uint32_t tim_arr_t;

typedef enum {
	tim_6,
	tim_7,
	first_base_tim = tim_6,
	second_base_tim = tim_7
} tim_num_t;

enum {
	tim6_irq_prior = 0,
	tim7_irq_prior = 0
};

typedef enum {
	tim_irq_off,
	tim_irq_on
} tim_irq_state_t;

typedef enum {
	tim_pwr_off = 0,
	tim_pwr_on 	= 1
} tim_power_state_t;

typedef struct {
	tim_num_t tim_num;
	tim_psc_t psc;
	tim_arr_t arr;
	tim_irq_state_t irq_st;
	tim_power_state_t pwr_st;
} tim_t;

void tim_init(const tim_t *tim_setup);
void tim_enable(tim_t *tim_setup);
void tim_disable(tim_t *tim_setup);
void tim_enable_irq(tim_t *tim_setup);
void tim_disable_irq(tim_t *tim_setup);
uint32_t tim_get_ticks(const tim_t *tim_setup);
void tim_set_ticks(const tim_t *tim_setup, const uint32_t ticks);

#endif
