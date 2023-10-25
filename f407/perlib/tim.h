#ifndef TIM_H_SENTRY
#define TIM_H_SENTRY

#include <stm32f4xx.h>

typedef uint32_t tim_psc_t;
typedef uint32_t tim_arr_t;

typedef enum {
	tim_num_6,
	tim_num_7
} tim_num_t;

enum {
	tim6_irq_prior = 0,
	tim7_irq_prior = 0
};

typedef enum {
	tim_irq_off,
	tim_irq_on
} tim_irq_state_t;

typedef struct {
	tim_num_t tim;
	tim_psc_t psc;
	tim_arr_t arr;
	tim_irq_state_t irq_state;
} tim_t;

void tim_init(const tim_t *tim_set);
void tim_set_freq(const tim_t *tim_set, uint32_t freq);
void tim_enable(const tim_t *tim_set);
void tim_disable(const tim_t *tim_set);
void tim_enable_irq(tim_t *tim_set);
void tim_disable_irq(tim_t *tim_set);
uint32_t tim_get_ticks(const tim_t *tim_set);
void tim_set_ticks(const tim_t *tim_set, const uint32_t ticks);
void tim_hold_init();
void tim_delay_ms(const uint32_t ms);

#endif
