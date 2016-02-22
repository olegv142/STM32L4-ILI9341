#pragma once

#include "platform.h"

class PinName {
public:
	PinName() : m_port(0), m_pin(0), m_obj(0) {}
	PinName(GPIO_TypeDef* port, unsigned pin, void* obj = 0)
		: m_port(port), m_pin(pin), m_obj(obj)
		{
			assert_param(m_port != 0);
		}

	GPIO_TypeDef* m_port;
	unsigned      m_pin;
	void*         m_obj;

	static PinName not_connected;
};

#define NC PinName::not_connected

class DigitalOut {
public:
	DigitalOut(GPIO_TypeDef* port, unsigned pin)
		: m_port(port), m_pin(pin)
		{
			assert_param(m_port != 0);
		}
	DigitalOut(PinName const& N)
		: m_port(N.m_port), m_pin(N.m_pin)
		{
			assert_param(m_port != 0);
		}
	void operator =(int bit) {
		if (bit) m_port->BSRR = m_pin;
		else     m_port->BRR  = m_pin;
	}
private:
	GPIO_TypeDef* m_port;
	unsigned      m_pin;
};

class SPI {
public:
	SPI(SPI_HandleTypeDef* hspi)
		: m_hspi(hspi), m_soft_bits(0), m_hard_bits(0)
	{
		assert_param(m_hspi != 0);
	}
	SPI(PinName const& mosi, PinName const& miso, PinName const& sclk, PinName const& ssel = NC)
		: m_hspi((SPI_HandleTypeDef*)mosi.m_obj), m_soft_bits(0), m_hard_bits(0)
	{
		assert_param(mosi.m_obj == miso.m_obj);
		assert_param(mosi.m_obj == sclk.m_obj);
		assert_param(m_hspi != 0);
	}
	void enable()
	{
		__HAL_SPI_ENABLE(m_hspi);
	}
	void format(int bits, int mode = 0)
	{
		m_soft_bits = bits;
		m_hard_bits = (m_hspi->Init.DataSize >> 8) + 1;
		assert_param(m_soft_bits >= m_hard_bits);
		// The mode must be configured outside of this adaptor
	}
	void frequency(int hz = 0)
	{
		// Must be configured outside of this adaptor
	}
	int write(int value)
	{
		int rx, wide = m_soft_bits > 8, swap = wide && m_hard_bits <= 8;
		assert_param(m_hard_bits);
		if (wide) {
			if (swap)
				*(__IO uint16_t*)&m_hspi->Instance->DR = (uint16_t)__REV16(value);
			else
				*(__IO uint16_t*)&m_hspi->Instance->DR = (uint16_t)value;
		} else {
			*(__IO uint8_t*)&m_hspi->Instance->DR = (uint8_t)value;
		}
		while (m_hspi->Instance->SR & SPI_FLAG_BSY);
		if (wide) {
			if (swap)
				rx = __REV16(*(__IO uint16_t*)&m_hspi->Instance->DR);
			else
				rx = *(__IO uint16_t*)&m_hspi->Instance->DR;
		} else {
			rx = *(__IO uint8_t*)&m_hspi->Instance->DR;
		}
		return rx;
	}
	void tx_buff(uint16_t const* buff, unsigned len)
	{
		int swap = m_hard_bits <= 8;
		assert_param(m_hard_bits);
		while (len)
		{
			if (m_hspi->Instance->SR & SPI_FLAG_TXE)
			{
				if (swap)
					*(__IO uint16_t*)&m_hspi->Instance->DR = (uint16_t)__REV16(*buff);
				else
					*(__IO uint16_t*)&m_hspi->Instance->DR = (uint16_t)*buff;
				++buff;
				--len;
			}
		}
		while (m_hspi->Instance->SR & SPI_FLAG_BSY);
		__HAL_SPI_CLEAR_OVRFLAG(m_hspi);
	}

private:
	SPI_HandleTypeDef* m_hspi;
	int m_soft_bits;
	int m_hard_bits;
};
