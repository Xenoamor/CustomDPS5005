#include "ili9163c.h"
#include "main.h"
#include "time.h"
#include "stm32f1xx_ll_gpio.h"
#include "ili9163c_registers.h"
#include "spi.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

//#define SCREEN_WIDTH  (128)
//#define SCREEN_HEIGHT (128)

#define _TFTWIDTH       128//the REAL W resolution of the TFT
#define _TFTHEIGHT      128//the REAL H resolution of the TFT
#define _GRAMWIDTH      128
#define _GRAMHEIGHT     160//160
#define _GRAMSIZE       _GRAMWIDTH * _GRAMHEIGHT//*see note 1
#define __OFFSET    32//*see note 2

const uint8_t pGammaSet[15] = {0x3F, 0x26, 0x23, 0x30, 0x28, 0x10, 0x55, 0xB7, 0x40, 0x19, 0x10, 0x1E, 0x02, 0x01, 0x00};
const uint8_t nGammaSet[15] = {0x09, 0x18, 0x2D, 0x0D, 0x13, 0x15, 0x40, 0x48, 0x53, 0x0C, 0x1D, 0x25, 0x2E, 0x24, 0x29};

static uint8_t rotation;
static uint8_t mad_ctrl_value = 0;

static inline void write_command(uint8_t cmd)
{
    LL_GPIO_ResetOutputPin(GPIO_TFT_A0_GPIO_Port, GPIO_TFT_A0_Pin);
    SPI_Transfer(&cmd, sizeof(cmd));
    //printf("c: %02X\n\r", cmd);
}

static inline void write_data8(uint8_t data)
{
    LL_GPIO_SetOutputPin(GPIO_TFT_A0_GPIO_Port, GPIO_TFT_A0_Pin);
    SPI_Transfer(&data, sizeof(data));
    //printf("d: %02X\n\r", data);
}

static inline void write_data16(uint16_t data)
{
    LL_GPIO_SetOutputPin(GPIO_TFT_A0_GPIO_Port, GPIO_TFT_A0_Pin);
    uint8_t tx_buf[2] = {(uint8_t) (data >> 8), (uint8_t) (data & 0xff)};
    SPI_Transfer((uint8_t *) &tx_buf, sizeof(tx_buf));
    //printf("d: %02X %02X\n\r", tx_buf[1], tx_buf[0]);
}

/*
Colorspace selection:
0: RGB
1: GBR
*/
static void ili9163c_set_color_space(uint8_t cspace)
{
    if (cspace < 1) {
        mad_ctrl_value &= ~(1<<3);
    } else {
        mad_ctrl_value |= 1<<3;
    }
}

void ili9163c_set_rotation(uint8_t rot)
{
    rotation = rot % 4; // can't be higher than 3
    switch (rotation)
    {
        case 0:
            mad_ctrl_value = 0b00001000;
            break;
        case 1:
            mad_ctrl_value = 0b01101000;
            break;
        case 2:
            mad_ctrl_value = 0b11001000;
            break;
        case 3:
            mad_ctrl_value = 0b10101000;
            break;
    }
    ili9163c_set_color_space(0);
    write_command(CMD_MADCTL);
    write_data8(mad_ctrl_value);
}

void ili9163c_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    write_command(CMD_CLMADRS); // Column
    if (rotation == 0 || rotation > 1)
    {
        write_data16(x0);
        write_data16(x1);
    }
    else
    {
        write_data16(x0 + __OFFSET);
        write_data16(x1 + __OFFSET);
    }

    write_command(CMD_PGEADRS); // Page
    if (rotation == 0)
    {
        write_data16(y0 + __OFFSET);
        write_data16(y1 + __OFFSET);
    }
    else
    {
        write_data16(y0);
        write_data16(y1);
    }
    write_command(CMD_RAMWR); // Into RAM
}

void draw_pixel(int16_t x, int16_t y, uint16_t color)
{

    if ((x < 0) || (y < 0)) return;
    ili9163c_set_window(x, y, x + 1, y + 1);
    write_data16(color);
}

void fill_screen(uint16_t color)
{
//    ili9163c_set_window(0, 0, _GRAMWIDTH, _GRAMHEIGHT);
//    for (uint32_t i = 0; i < _GRAMSIZE; ++i)
//    {
//        //write_data16(color);
//    }

    uint32_t i;
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xff; 
    uint8_t fill[] = {hi, lo, hi, lo, hi, lo, hi, lo, hi, lo, hi, lo, hi, lo, hi, lo};
    LL_GPIO_ResetOutputPin(GPIO_TFT_A0_GPIO_Port, GPIO_TFT_A0_Pin);
    ili9163c_set_window(0, 0, _GRAMWIDTH+2, _GRAMHEIGHT); // Note! For some reason filling WxH is results in two vertical lines to the far right...
    LL_GPIO_SetOutputPin(GPIO_TFT_A0_GPIO_Port, GPIO_TFT_A0_Pin);
    for (i = 0;i < ((_GRAMWIDTH+2) * _GRAMHEIGHT)/(sizeof(fill)/2); i++) {
        SPI_Transfer((uint8_t *) &fill, sizeof(fill));
    }    

}

void ili9163c_init(void)
{

    LL_GPIO_SetOutputPin(GPIO_TFT_RESET_GPIO_Port, GPIO_TFT_RESET_Pin);
    Time_Delay_Ms(500);
    LL_GPIO_ResetOutputPin(GPIO_TFT_RESET_GPIO_Port, GPIO_TFT_RESET_Pin);
    Time_Delay_Ms(500);
    LL_GPIO_SetOutputPin(GPIO_TFT_RESET_GPIO_Port, GPIO_TFT_RESET_Pin);
    Time_Delay_Ms(500);

    /*
    7) MY:  1(bottom to top), 0(top to bottom)  Row Address Order
    6) MX:  1(R to L),        0(L to R)         Column Address Order
    5) MV:  1(Exchanged),     0(normal)         Row/Column exchange
    4) ML:  1(bottom to top), 0(top to bottom)  Vertical Refresh Order
    3) RGB: 1(BGR),            0(RGB)               Color Space
    2) MH:  1(R to L),        0(L to R)         Horizontal Refresh Order
    1)
    0)
         MY, MX, MV, ML,RGB, MH, D1, D0
         0 | 0 | 0 | 0 | 1 | 0 | 0 | 0  //normal
         1 | 0 | 0 | 0 | 1 | 0 | 0 | 0  //Y-Mirror
         0 | 1 | 0 | 0 | 1 | 0 | 0 | 0  //X-Mirror
         1 | 1 | 0 | 0 | 1 | 0 | 0 | 0  //X-Y-Mirror
         0 | 0 | 1 | 0 | 1 | 0 | 0 | 0  //X-Y Exchange
         1 | 0 | 1 | 0 | 1 | 0 | 0 | 0  //X-Y Exchange, Y-Mirror
         0 | 1 | 1 | 0 | 1 | 0 | 0 | 0  //XY exchange
         1 | 1 | 1 | 0 | 1 | 0 | 0 | 0
    */

    write_command(CMD_SWRESET); // software reset
    Time_Delay_Ms(500);

    write_command(CMD_SLPOUT); // exit sleep
    Time_Delay_Ms(5);

    write_command(CMD_PIXFMT); // Set Color Format 16bit
    write_data8(0x05);

    write_command(CMD_GAMMASET); // default gamma curve 3
    write_data8(0x04); // 0x04
    Time_Delay_Ms(5);

    write_command(CMD_GAMRSEL); // Enable Gamma adj
    write_data8(0x01);
    Time_Delay_Ms(5);

    write_command(CMD_NORML);
    Time_Delay_Ms(5);

    write_command(CMD_DFUNCTR);
    write_data8(0b11111111);
    write_data8(0b00000110);
    Time_Delay_Ms(5);

    write_command(CMD_PGAMMAC); // Positive Gamma Correction Setting
    for (uint32_t i = 0; i < 15; ++i)
        write_data8(pGammaSet[i]);
    Time_Delay_Ms(5);

    write_command(CMD_NGAMMAC); // Negative Gamma Correction Setting
    for (uint32_t i = 0; i < 15; ++i)
        write_data8(nGammaSet[i]);
    Time_Delay_Ms(5);

    write_command(CMD_FRMCTR1); // Frame Rate Control (In normal mode/Full colors)
    write_data8(0x08); // 0x0C//0x08
    write_data8(0x02); // 0x14//0x08
    Time_Delay_Ms(5);

    write_command(CMD_DINVCTR); // display inversion
    write_data8(0x07);
    Time_Delay_Ms(5);

    write_command(CMD_PWCTR1); // Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
    write_data8(0x0A); // 4.30 - 0x0A
    write_data8(0x02); // 0x05
    Time_Delay_Ms(5);

    write_command(CMD_PWCTR2); // Set BT[2:0] for AVDD & VCL & VGH & VGL
    write_data8(0x02);
    Time_Delay_Ms(5);

    write_command(CMD_VCOMCTR1); // Set VMH[6:0] & VML[6:0] for VOMH & VCOML
    write_data8(0x50); // 0x50
    write_data8(99); // 0x5b
    Time_Delay_Ms(5);

    write_command(CMD_VCOMOFFS);
    write_data8(0); // 0x40
    Time_Delay_Ms(5);

    write_command(CMD_CLMADRS); // Set Column Address
    write_data16(0x00);
    write_data16(_GRAMWIDTH);
    Time_Delay_Ms(5);

    write_command(CMD_PGEADRS); // Set Page Address
    write_data16(0x00);
    write_data16(_GRAMHEIGHT);
    Time_Delay_Ms(5);
    // set scroll area (thanks Masuda)
    write_command(CMD_VSCLLDEF);
    write_data16(__OFFSET);
    write_data16(_GRAMHEIGHT - __OFFSET);
    write_data16(0);
    Time_Delay_Ms(5);

    //ili9163c_set_color_space(0);
    //ili9163c_set_rotation(0);

    write_command(CMD_DISPON); // display ON
    Time_Delay_Ms(5);

    write_command(CMD_RAMWR); // Memory Write
    Time_Delay_Ms(5);

    ili9163c_set_rotation(3);

    printf("Device Inited\n\r");

    fill_screen(0x0000);

    printf("Device Filled\n\r");

    while (1)
    {
        //fill_screen(0x0000);
        Time_Delay_Ms(1);
        //printf("ping\n\r");
    }
}