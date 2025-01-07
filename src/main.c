#include "logic/TTL7400.h"
#include "logic/TTL7402.h"
#include "logic/TTL7404.h"
#include "logic/TTL7425.h"
#include "logic/TTL7427.h"
#include "logic/TTL7474.h"
#include "logic/TTL7486.h"
#include "logic/TTL7493.h"
#include "logic/TTL74107.h"
#include "logic/TTL7430.h"
#include "logic/TTL7410.h"

#include "PicoPong.pio.h"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <tusb.h>
#include <hardware/dma.h>
#include <hardware/pio.h>

/*
 Links...
 https://www.pong-story.com/LAWN_TENNIS.pdf
 https://www.arcade-museum.com/manuals-videogames/P/PongSchematic.pdf
 https://www.falstad.com/pong/
*/

TTL7486_PINS pins7486_A4D = TTL7486_INIT;
TTL7410_PINS pins7410_D8C = TTL7410_INIT;
TTL74107_PINS pins74107_D9B = TTL74107_INIT | TTL74107_Q;
TTL7404_PINS pins7404_E4E = TTL7404_INIT;
TTL7404_PINS pins7404_E4F = TTL7404_INIT;
TTL7474_PINS pins7474_E7A = TTL7474_INIT;
TTL7474_PINS pins7474_E7B = TTL7474_INIT;
TTL7493_PINS pins7493_E8 = TTL7493_INIT | TTL7493_Q0 | TTL7493_Q1 | TTL7493_Q2 | TTL7493_Q3;
TTL7493_PINS pins7493_E9 = TTL7493_INIT | TTL7493_Q0 | TTL7493_Q1 | TTL7493_Q2 | TTL7493_Q3;
TTL7425_PINS pins7425_F2B = TTL7425_INIT;
TTL74107_PINS pins74107_F3B = TTL74107_INIT;
TTL7402_PINS pins7402_F5C = TTL7402_INIT;
TTL7402_PINS pins7402_F5D = TTL7402_INIT;
TTL74107_PINS pins74107_F6B = TTL74107_INIT;
TTL7430_PINS pins7430_F7 = TTL7430_INIT;
TTL7493_PINS pins7493_F8 = TTL7493_INIT;
TTL7493_PINS pins7493_F9 = TTL7493_INIT;
TTL7402_PINS pins7402_G1B = TTL7402_INIT;
TTL7427_PINS pins7427_G2B = TTL7427_INIT;
TTL7400_PINS pins7400_G3B = TTL7400_INIT;
TTL7410_PINS pins7410_G5A = TTL7410_INIT;
TTL7410_PINS pins7410_G5B = TTL7410_INIT;
TTL7400_PINS pins7400_H5A = TTL7400_INIT;
TTL7400_PINS pins7400_H5B = TTL7400_INIT;
TTL7400_PINS pins7400_H5C = TTL7400_INIT;
TTL7400_PINS pins7400_H5D = TTL7400_INIT;

#define PIN_CLK()           clock
#define PIN_1H()            TTL7493_GET_Q0(pins7493_F8)
#define PIN_2H()            TTL7493_GET_Q1(pins7493_F8)
#define PIN_4H()            TTL7493_GET_Q2(pins7493_F8)
#define PIN_8H()            TTL7493_GET_Q3(pins7493_F8)
#define PIN_16H()           TTL7493_GET_Q0(pins7493_F9)
#define PIN_32H()           TTL7493_GET_Q1(pins7493_F9)
#define PIN_64H()           TTL7493_GET_Q2(pins7493_F9)
#define PIN_128H()          TTL7493_GET_Q3(pins7493_F9)
#define PIN_256H()          TTL74107_GET_Q(pins74107_F6B)
#define _PIN_256H()         TTL74107_GET_QQ(pins74107_F6B)
#define PIN_HRESET()        TTL7474_GET_QQ(pins7474_E7B)
#define _PIN_HRESET()       TTL7474_GET_Q(pins7474_E7B)
#define PIN_1V()            TTL7493_GET_Q0(pins7493_E8)
#define PIN_2V()            TTL7493_GET_Q1(pins7493_E8)
#define PIN_4V()            TTL7493_GET_Q2(pins7493_E8)
#define PIN_8V()            TTL7493_GET_Q3(pins7493_E8)
#define PIN_16V()           TTL7493_GET_Q0(pins7493_E9)
#define PIN_32V()           TTL7493_GET_Q1(pins7493_E9)
#define PIN_64V()           TTL7493_GET_Q2(pins7493_E9)
#define PIN_128V()          TTL7493_GET_Q3(pins7493_E9)
#define PIN_256V()          TTL74107_GET_Q(pins74107_D9B)
#define _PIN_256V()         TTL74107_GET_QQ(pins74107_D9B)
#define PIN_HBLANKING()     TTL7400_GET_Y(pins7400_H5C)
#define _PIN_HBLANKING()    TTL7400_GET_Y(pins7400_H5B)
#define _PIN_HSYNC()        TTL7400_GET_Y(pins7400_H5D)
#define PIN_VBLANK()        TTL7402_GET_Y(pins7402_F5D)
#define _PIN_VBLANK()       TTL7402_GET_Y(pins7402_F5C)
#define PIN_VRESET()        TTL7474_GET_QQ(pins7474_E7A)
#define _PIN_VRESET()       TTL7474_GET_Q(pins7474_E7A)
#define _PIN_VSYNC()        TTL7410_GET_Y(pins7410_G5A)
#define PIN_SYNC()          TTL7404_GET_Y(pins7404_E4E)
#define PIN_VIDEO()         TTL7404_ACT(pins7404_E4F)
#define _PIN_HVID()         0
#define _PIN_VVID()         0
#define PIN_PAD1()          0
#define PIN_PAD2()          0
#define PIN_NET()           TTL7427_GET_Y(pins7427_G2B)

#define BYTE_TO_BINARY_PATTERN "0b00000001'%c%c%c%c%c%c%c%c,\n"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

int m_dmaChan;
PIO m_pio = pio0;
uint m_sm = 0;
uint8_t data[238420]; // = {0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00};

void dma_handler()
{
    dma_channel_acknowledge_irq0(m_dmaChan);
    dma_channel_set_read_addr(m_dmaChan, (void*)data, true);
}

void run()
{
	uint offset = pio_add_program(m_pio, &pio_pico_pong_program);
	pio_pico_pong_program_init(m_pio, m_sm, offset);
	pio_sm_set_clkdiv(m_pio, m_sm, 32); // 4.365135
	pio_sm_set_enabled(m_pio, m_sm, true);

    m_dmaChan = dma_claim_unused_channel(true);
    dma_channel_config dmaConfig = dma_channel_get_default_config(m_dmaChan);
    channel_config_set_transfer_data_size(&dmaConfig, DMA_SIZE_32);
    channel_config_set_read_increment(&dmaConfig, true);
    channel_config_set_write_increment(&dmaConfig, false);
    channel_config_set_dreq(&dmaConfig, pio_get_dreq(m_pio, m_sm, true));
    dma_channel_set_config(m_dmaChan, &dmaConfig, false);
    dma_channel_set_trans_count(m_dmaChan, 238420/4, false);
    dma_channel_set_write_addr(m_dmaChan, &m_pio->txf[m_sm], false);
    dma_channel_set_irq0_enabled(m_dmaChan, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    dma_handler();

    while(true)
    {
        tight_loop_contents();
    }
}

int main()
{
    bool clock = false;
    uint32_t idxClock = 0;
    uint8_t output;

    stdio_init_all();
    //while (!tud_cdc_connected()) { sleep_ms(100); }

    /* Hard wire */
    TTL74107_SET_J(pins74107_D9B); 
    TTL74107_SET_K(pins74107_D9B);

    TTL7474_SET_CLR(pins7474_E7A);
    TTL7474_SET_PRE(pins7474_E7A);

    TTL7474_SET_CLR(pins7474_E7B);
    TTL7474_SET_PRE(pins7474_E7B);

    TTL74107_SET_CLR(pins74107_F3B);

    TTL74107_SET_J(pins74107_F6B);
    TTL74107_SET_K(pins74107_F6B);

    TTL7430_SET_B(pins7430_F7);
    TTL7430_SET_C(pins7430_F7);
    TTL7430_SET_D(pins7430_F7);

    for(idxClock = 0; idxClock < 238420; idxClock++)
    {
        /*-------*/
        /* HSYNC */
        /*-------*/
        /* F8 */
        TTL7493_PUT_CLK(pins7493_F8,PIN_CLK());
        pins7493_F8 = TTL7493_ACT(pins7493_F8);

        /* F9 */
        TTL7493_PUT_CLK(pins7493_F9,TTL7493_GET_Q3(pins7493_F8));
        pins7493_F9 = TTL7493_ACT(pins7493_F9);

        /* F6B */
        TTL74107_PUT_CLK(pins74107_F6B,PIN_128H());
        pins74107_F6B = TTL74107_ACT(pins74107_F6B);

        /* F7 */
        TTL7430_PUT_A(pins7430_F7,PIN_256H());
        TTL7430_PUT_E(pins7430_F7,PIN_4H());
        TTL7430_PUT_F(pins7430_F7,PIN_2H());
        TTL7430_PUT_G(pins7430_F7,PIN_128H());
        TTL7430_PUT_H(pins7430_F7,PIN_64H());
        pins7430_F7 = TTL7430_ACT(pins7430_F7);

        /* E7B */
        TTL7474_PUT_CLK(pins7474_E7B,PIN_CLK());
        TTL7474_PUT_D(pins7474_E7B,TTL7430_GET_Y(pins7430_F7));
        pins7474_E7B = TTL7474_ACT(pins7474_E7B);

        /* _HRESET */
        if(_PIN_HRESET())
        {
            TTL74107_SET_CLR(pins74107_F6B);
            pins74107_F6B = TTL74107_ACT(pins74107_F6B);
        }
        else
        {
            TTL74107_RESET_CLR(pins74107_F6B);
        }

        /* HRESET */
        if(PIN_HRESET())
        {
            TTL7493_SET_CLR1(pins7493_F8);
            TTL7493_SET_CLR2(pins7493_F8);
            TTL7493_SET_CLR1(pins7493_F9);
            TTL7493_SET_CLR2(pins7493_F9);
            pins7493_F8 = TTL7493_ACT(pins7493_F8);
            pins7493_F9 = TTL7493_ACT(pins7493_F9);
        }
        else
        {
            TTL7493_RESET_CLR1(pins7493_F8);
            TTL7493_RESET_CLR2(pins7493_F8);
            TTL7493_RESET_CLR1(pins7493_F9);
            TTL7493_RESET_CLR2(pins7493_F9);
        }

        /*-------*/
        /* VSYNC */
        /*-------*/
        /* E8 */
        TTL7493_PUT_CLK(pins7493_E8,PIN_HRESET());
        pins7493_E8 = TTL7493_ACT(pins7493_E8);

        /* E9 */
        TTL7493_PUT_CLK(pins7493_E9,TTL7493_GET_Q3(pins7493_E8));
        pins7493_E9 = TTL7493_ACT(pins7493_E9);

        /* D9B */
        TTL74107_PUT_CLK(pins74107_D9B,TTL7493_GET_Q3(pins7493_E9));
        pins74107_D9B = TTL74107_ACT(pins74107_D9B);

        /* D8C */
        TTL7410_PUT_A(pins7410_D8C,TTL74107_GET_Q(pins74107_D9B));
        TTL7410_PUT_B(pins7410_D8C,TTL7493_GET_Q2(pins7493_E8));
        TTL7410_PUT_C(pins7410_D8C,TTL7493_GET_Q0(pins7493_E8));
        pins7410_D8C = TTL7410_ACT(pins7410_D8C);

        /* E7A */
        TTL7474_PUT_CLK(pins7474_E7A,PIN_HRESET());
        TTL7474_PUT_D(pins7474_E7A,TTL7410_GET_Y(pins7410_D8C));
        pins7474_E7A = TTL7474_ACT(pins7474_E7A);

        /* _VRESET */
        if(TTL7474_GET_Q(pins7474_E7A))
        {
            TTL74107_SET_CLR(pins74107_D9B);
            pins74107_D9B = TTL74107_ACT(pins74107_D9B);
        }
        else
        {
            TTL74107_RESET_CLR(pins74107_D9B);
        }

        /* VRESET */
        if(TTL7474_GET_QQ(pins7474_E7A))
        {
            TTL7493_SET_CLR1(pins7493_E8);
            TTL7493_SET_CLR2(pins7493_E8);
            TTL7493_SET_CLR1(pins7493_E9);
            TTL7493_SET_CLR2(pins7493_E9);
            pins7493_E8 = TTL7493_ACT(pins7493_E8);
            pins7493_E9 = TTL7493_ACT(pins7493_E9);
        }
        else
        {
            TTL7493_RESET_CLR1(pins7493_E8);
            TTL7493_RESET_CLR2(pins7493_E8);
            TTL7493_RESET_CLR1(pins7493_E9);
            TTL7493_RESET_CLR2(pins7493_E9);
        }

        /*------------------*/
        /* SYNC / Video Sum */
        /*------------------*/
        /* G5B */
        TTL7410_PUT_A(pins7410_G5B, PIN_16H());
        TTL7410_PUT_B(pins7410_G5B, PIN_64H());
        TTL7410_PUT_C(pins7410_G5B, PIN_64H());
        pins7410_G5B = TTL7410_ACT(pins7410_G5B);

        /* H5B / H5C */
        TTL7400_PUT_A(pins7400_H5B, TTL7410_GET_Y(pins7410_G5B));
        TTL7400_PUT_B(pins7400_H5B, TTL7400_GET_Y(pins7400_H5C));
        pins7400_H5B = TTL7400_ACT(pins7400_H5B);

        TTL7400_PUT_A(pins7400_H5C, TTL7400_GET_Y(pins7400_H5B));
        TTL7400_PUT_B(pins7400_H5C, _PIN_HRESET());
        pins7400_H5C = TTL7400_ACT(pins7400_H5C);

        TTL7400_PUT_B(pins7400_H5B, TTL7400_GET_Y(pins7400_H5C));
        pins7400_H5B = TTL7400_ACT(pins7400_H5B);

        TTL7400_PUT_A(pins7400_H5C, TTL7400_GET_Y(pins7400_H5B));
        pins7400_H5C = TTL7400_ACT(pins7400_H5C);

        /* H5D */
        TTL7400_PUT_A(pins7400_H5D, TTL7400_GET_Y(pins7400_H5C));
        TTL7400_PUT_B(pins7400_H5D, PIN_32H());
        pins7400_H5D = TTL7400_ACT(pins7400_H5D);

        /* F5C / F5D */
        TTL7402_PUT_A(pins7402_F5C,PIN_VRESET());
        TTL7402_PUT_B(pins7402_F5C,TTL7402_GET_Y(pins7402_F5D));
        pins7402_F5C = TTL7402_ACT(pins7402_F5C);

        TTL7402_PUT_A(pins7402_F5D,TTL7402_GET_Y(pins7402_F5C));
        TTL7402_PUT_B(pins7402_F5D,PIN_16V());
        pins7402_F5D = TTL7402_ACT(pins7402_F5D);

        TTL7402_PUT_B(pins7402_F5C,TTL7402_GET_Y(pins7402_F5D));
        pins7402_F5C = TTL7402_ACT(pins7402_F5C);
        TTL7402_PUT_A(pins7402_F5D,TTL7402_GET_Y(pins7402_F5C));
        pins7402_F5D = TTL7402_ACT(pins7402_F5D);

        /* H5A */
        TTL7400_PUT_A(pins7400_H5A,PIN_8V());
        TTL7400_PUT_B(pins7400_H5A,PIN_8V());
        pins7400_H5A = TTL7400_ACT(pins7400_H5A);

        /* G5A */
        TTL7410_PUT_A(pins7410_G5A, TTL7402_GET_Y(pins7402_F5D));
        TTL7410_PUT_B(pins7410_G5A, PIN_4V());
        TTL7410_PUT_C(pins7410_G5A, TTL7400_GET_Y(pins7400_H5A));
        pins7410_G5A = TTL7410_ACT(pins7410_G5A);

        /* A4D */
        TTL7486_PUT_A(pins7486_A4D, _PIN_HSYNC());
        TTL7486_PUT_B(pins7486_A4D, _PIN_VSYNC());
        pins7486_A4D = TTL7486_ACT(pins7486_A4D);

        /* E4E */
        TTL7404_PUT_A(pins7404_E4E, TTL7486_GET_Y(pins7486_A4D));
        pins7404_E4E = TTL7404_ACT(pins7404_E4E);

        /*-----*/
        /* NET */
        /*-----*/
        /* F3B */
        TTL74107_PUT_J(pins74107_F3B,PIN_256H());
        TTL74107_PUT_K(pins74107_F3B,_PIN_256H());
        TTL74107_PUT_CLK(pins74107_F3B,PIN_CLK());
        pins74107_F3B = TTL74107_ACT(pins74107_F3B);

        /* G3B */
        TTL7400_PUT_A(pins7400_G3B,PIN_256H());
        TTL7400_PUT_B(pins7400_G3B,TTL74107_GET_QQ(pins74107_F3B));
        pins7400_G3B = TTL7400_ACT(pins7400_G3B);

        /* G2B */
        TTL7427_PUT_A(pins7427_G2B,PIN_4V());
        TTL7427_PUT_B(pins7427_G2B,PIN_VBLANK());
        TTL7427_PUT_C(pins7427_G2B,TTL7400_GET_Y(pins7400_G3B));
        pins7427_G2B = TTL7427_ACT(pins7427_G2B);

        // Sum Sync / Video
        TTL7402_PUT_A(pins7402_G1B, _PIN_HVID());
        TTL7402_PUT_B(pins7402_G1B, _PIN_VVID());
        pins7402_G1B = TTL7402_ACT(pins7402_G1B);

        TTL7425_PUT_A(pins7425_F2B, TTL7402_GET_Y(pins7402_G1B));
        TTL7425_PUT_B(pins7425_F2B, PIN_PAD2());
        TTL7425_PUT_C(pins7425_F2B, PIN_NET());
        TTL7425_PUT_D(pins7425_F2B, PIN_PAD1());
        pins7425_F2B = TTL7425_ACT(pins7425_F2B);

        TTL7404_PUT_A(pins7404_E4F, TTL7425_GET_Y(pins7425_F2B));
        pins7404_E4F = TTL7404_ACT(pins7404_E4F);

        /* 0       : CLK */
        /* 1       : SYNC */
        /* 2       : VIDEO */
        output = PIN_256H() ? (1 << 0) : 0; //clock
        output |= PIN_128H() ? (1 << 1) : 0; // PIN_SYNC()
        output |= TTL7400_GET_Y(pins7400_G3B) ? (1 << 2) : 0; // PIN_VIDEO()
        output |= clock ? (1 << 3) : 0;

        data[idxClock] = output;

        /*-----------*/
        /* Half TICK */
        /*-----------*/
        clock = !clock;
    }

    run();

    return 0;
}
