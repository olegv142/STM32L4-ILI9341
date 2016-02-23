#pragma once

static inline void pin_init_out(PinName& p)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	assert_param(p.m_port != 0);

	GPIO_InitStructure.Pin = p.m_pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(p.m_port, &GPIO_InitStructure);
}

static inline void pin_init_af(PinName& p, uint8_t af)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	assert_param(p.m_port != 0);

	GPIO_InitStructure.Pin = p.m_pin;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = af;

	HAL_GPIO_Init(p.m_port, &GPIO_InitStructure);
}

static inline void spi_handle_init(SPI_HandleTypeDef* hspi, uint8_t bits, uint8_t mode, uint32_t prescaler)
{
	assert_param(hspi->Instance);

	hspi->Init.BaudRatePrescaler = prescaler;
	hspi->Init.Direction = SPI_DIRECTION_2LINES;
	hspi->Init.CLKPhase = mode & SPI_CR1_CPHA;
	hspi->Init.CLKPolarity = mode & SPI_CR1_CPOL;
	hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi->Init.CRCPolynomial = 7;
	hspi->Init.DataSize = (bits - 1) << 8;
	hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi->Init.NSS = SPI_NSS_SOFT;
	hspi->Init.TIMode = SPI_TIMODE_DISABLE;
	hspi->Init.Mode = SPI_MODE_MASTER;

	HAL_SPI_Init(hspi);

	__HAL_SPI_ENABLE(hspi);
}
