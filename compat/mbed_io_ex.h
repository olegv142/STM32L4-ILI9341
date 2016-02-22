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

