#include "spi.h"

static void init_gpio(spi_t *spi_setup);
static SPI_TypeDef *switch_spi(spi_t *spi_setup);
nvic_irqn_t switch_irqn(const spi_t *spi_setup);
nvic_irq_prior_t switch_irq_prior(const spi_t *spi_setup);
rcc_periph_id_t switch_spi_id(const spi_t *spi_setup);
spi_bus_div_t switch_bus_div(const spi_t *spi_setup);
rcc_clk_t switch_clk(const spi_t *spi_setup);
#if 0
void set_speed(const spi_t *spi_setup);
#endif

void spi_init(spi_t *spi_setup)
{
	SPI_TypeDef *spi = switch_spi(spi_setup);
	rcc_periph_id_t spi_id = switch_spi_id(spi_setup);
#if 0
	nvic_irqn_t spi_irqn = switch_irqn(spi_setup);
	nvic_irq_prior_t spi_irq_prior = switch_irq_prior(spi_setup);
#endif
	init_gpio(spi_setup);
	rcc_enable(spi_id);

	spi->CR1 |=	((spi_setup->mode) 		<< SPI_CR1_MSTR_Pos) |
				((spi_setup->cpol) 		<< SPI_CR1_CPOL_Pos) |
				((spi_setup->cpha) 		<< SPI_CR1_CPHA_Pos) |
				((spi_setup->spd_div) 	<< SPI_CR1_BR_Pos);


	spi->CR2 &= ~0xE7; //bad practice
	spi->CR2 |= ((spi_setup->nss) << SPI_CR2_SSOE_Pos);

	spi->CR1 |= ((spi_setup->pwr) << SPI_CR1_SPE_Pos);
}

void spi_send_char(spi_t *spi_setup, const char c)
{
	SPI_TypeDef *spi = switch_spi(spi_setup);
	while(!(spi->SR & SPI_SR_TXE)) {}
	spi->DR = c;
}


void spi_recv_char(spi_t *spi_setup, char *c)
{
	SPI_TypeDef *spi = switch_spi(spi_setup);
	while(!(spi->SR & SPI_SR_RXNE)) {}
	*c = spi->DR;
}

void spi_swap_char(spi_t *spi_setup, const char send_c, char *recv_c)
{
	spi_send_char(spi_setup, send_c);
	spi_recv_char(spi_setup, recv_c);
}

void spi_send_buff(spi_t *spi_setup, const void *buff, const uint32_t byte_num)
{
	uint8_t *buff_ptr = (uint8_t *)buff;
	uint8_t c;
	int i;
	for(i = 0; i < byte_num; i++) {
		c = buff_ptr[i];
		spi_send_char(spi_setup, c);
	}
}

void spi_send_str(spi_t *spi_setup, const char *str)
{
	const char *str_ptr = str;
	while(*str_ptr) {
		spi_send_char(spi_setup, *str_ptr);
		str_ptr++;
	}
}

#if 0
/*static*/ void set_speed(spi_t *spi_setup)
{
}
#endif

/*static*/ void init_gpio(spi_t *spi_setup)
{
	spi_num_t num = spi_setup->num;

	gpio_t miso, mosi, sck;

	miso.mode	= gpio_mode_alt;
	miso.pupd	= gpio_pupd_no;

	mosi.mode	= gpio_mode_alt;
	mosi.pupd	= gpio_pupd_no;

	sck.mode	= gpio_mode_alt;
	sck.pupd	= gpio_pupd_no;

	switch(num) {
	case spi_1:	miso.mask 	= spi1_miso_pin;
				miso.port	= spi1_gpio_port;
				miso.alt	= gpio_alt_spi1;
				mosi.mask 	= spi1_mosi_pin;
				mosi.port	= spi1_gpio_port;
				mosi.alt	= gpio_alt_spi1;
				sck.mask 	= spi1_sck_pin;
				sck.port	= spi1_gpio_port;
				sck.alt		= gpio_alt_spi1;
				break;
	case spi_2:	miso.mask 	= spi2_miso_pin;
				miso.port	= spi2_gpio_port;
				miso.alt	= gpio_alt_spi2;
				mosi.mask 	= spi2_mosi_pin;
				mosi.port	= spi2_gpio_port;
				mosi.alt	= gpio_alt_spi2;
				sck.mask 	= spi2_sck_pin;
				sck.port	= spi2_gpio_port;
				sck.alt		= gpio_alt_spi2;
				break;
	default:	return;
	}


	gpio_init(&miso);
	gpio_init(&mosi);
	gpio_init(&sck);
}

/*static*/ SPI_TypeDef *switch_spi(spi_t *spi_setup)
{
	spi_num_t num = spi_setup->num;
	switch(num) {
	case spi_1:	return SPI1;
	case spi_2: return SPI2;
	default:	return 0;
	}
}

nvic_irqn_t switch_irqn(const spi_t *spi_setup)
{
	spi_num_t num = spi_setup->num;
	switch(num) {
	case spi_1:	return spi1_irqn;
	case spi_2: return spi2_irqn;
	default:	return none_irqn;
	}
}

nvic_irq_prior_t switch_irq_prior(const spi_t *spi_setup)
{
	spi_num_t num = spi_setup->num;
	switch(num) {
	case spi_1:	return spi1_irq_prior;	
	case spi_2: return spi2_irq_prior;
	default:	return none_irq_prior;
	}
}

rcc_periph_id_t switch_spi_id(const spi_t *spi_setup)
{
	spi_num_t num = spi_setup->num;
	switch(num) {
	case spi_1:	return spi1_id;
	case spi_2: return spi2_id;
	default:	return none_id;
	}
}

rcc_clk_t switch_clk(const spi_t *spi_setup)
{
	spi_num_t num = spi_setup->num;
	rcc_clk_t clk;

	switch(num) {
	case spi_1:	clk = rcc_get_apb2_clk();
				break;

	case spi_2: clk = rcc_get_apb1_clk();
				break;

	default:	return 0;
	}

	return clk;
}
