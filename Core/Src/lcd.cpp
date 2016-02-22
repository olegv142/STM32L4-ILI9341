#include "mbed.h"
#include "mbed_io_ex.h"
#include "lcd.h"
#include "SPI_TFT_ILI9341.h"

static PinName s_lcd_cs (LCD_CS_PORT,  LCD_CS_PIN);
static PinName s_lcd_dc (LCD_DC_PORT,  LCD_DC_PIN);
static PinName s_lcd_rst(LCD_RST_PORT, LCD_RST_PIN);

static SPI_HandleTypeDef s_lcd_spi;

static PinName s_lcd_mosi(LCD_SDI_PORT, LCD_SDI_PIN, &s_lcd_spi);
static PinName s_lcd_miso(LCD_SDO_PORT, LCD_SDO_PIN, &s_lcd_spi);
static PinName s_lcd_sclk(LCD_SCK_PORT, LCD_SCK_PIN, &s_lcd_spi);

SPI_TFT_ILI9341* g_pLCD = 0;

static void LED_SPI_Init(void)
{
	/* SPI Config */
	s_lcd_spi.Instance = LED_SPI;
	/* SPI baudrate is set to 40 MHz (PCLK1/SPI_BaudRatePrescaler = 80/2 = 40 MHz) 
	*/
	s_lcd_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	s_lcd_spi.Init.Direction = SPI_DIRECTION_2LINES;
	s_lcd_spi.Init.CLKPhase = SPI_PHASE_1EDGE;
	s_lcd_spi.Init.CLKPolarity = SPI_POLARITY_LOW;
	s_lcd_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	s_lcd_spi.Init.CRCPolynomial = 7;
	s_lcd_spi.Init.DataSize = SPI_DATASIZE_8BIT;
	s_lcd_spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	s_lcd_spi.Init.NSS = SPI_NSS_SOFT;
	s_lcd_spi.Init.TIMode = SPI_TIMODE_DISABLE;
	s_lcd_spi.Init.Mode = SPI_MODE_MASTER;

	HAL_SPI_Init(&s_lcd_spi);
	__HAL_SPI_ENABLE(&s_lcd_spi);
}

void bouncing_ball(SPI_TFT_ILI9341& lcd)
{
	uint16_t color = Red, cdelta = 7103;
	int const R = 31, W = 240, H = 320;
	int x = R, y = R, dx = 1, dy = 1;
	for (;;) {
		lcd.fillcircle(x, y, R, color);
		x += dx;
		y += dy;
		if (x <= R || W - R <= x) {
			dx = -dx;
			color += cdelta;
		}
		if (y <= R || H - R <= y) {
			dy = -dy;
			color += cdelta;
		}
	}
}

#define N 100
static uint16_t s_bmp[N*N*2];

void bmp_test(SPI_TFT_ILI9341& lcd)
{
	int i;
	for (i = 0; i < N; ++i) {
		s_bmp[i*N + i] = Red;
	}
	for (;;) {
		for (i = 0; i <= N; ++i) {
			lcd.Bitmap(20, 50, N, N, (unsigned char*)(s_bmp + i*N));
		}
	}
}

extern "C" {

void lcd_start(void)
{
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();

	pin_init_out(s_lcd_cs);
	pin_init_out(s_lcd_dc);
	pin_init_out(s_lcd_rst);

	pin_init_af(s_lcd_mosi, LCD_SPI_AF);
	pin_init_af(s_lcd_miso, LCD_SPI_AF);
	pin_init_af(s_lcd_sclk, LCD_SPI_AF);

	LED_SPI_Init();

	SPI_TFT_ILI9341 lcd(s_lcd_mosi, s_lcd_miso, s_lcd_sclk, s_lcd_cs, s_lcd_rst, s_lcd_dc);
	bouncing_ball(lcd);
	//bmp_test(lcd);
}

}
