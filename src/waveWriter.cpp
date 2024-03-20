#include "waveWriter.h"

#include "pico/stdlib.h"
#include <hardware/dma.h>
#include <cstring>

WaveWriter waveWriter(pio0, 0);

extern "C" {
void dma_handler()
{
    waveWriter.Continue();
}
}

//-------
//-------
// PUBLIC
//-------
//-------
WaveWriter::WaveWriter(PIO pio, uint sm) :
    m_hz(200.0),
    m_waveCurrent(&m_wave1),
    m_waveNext(nullptr),
    m_pio(pio),
    m_sm(sm),
    m_active(false),
    m_dmaChan(-1)
{
}

void WaveWriter::Initialise()
{
    uint16_t prog_instr = pio_encode_out(pio_pins, 8);
    struct pio_program prog = {
            .instructions = &prog_instr,
            .length = 1,
            .origin = -1
    };
    uint offset = pio_add_program(m_pio, &prog);
    for(uint8_t gpio = 0; gpio < 8; gpio++) pio_gpio_init(m_pio, gpio);
    pio_sm_set_consecutive_pindirs(m_pio, m_sm, 0, 8, GPIO_OUT);

	// Config
    pio_sm_config pioConfig = pio_get_default_sm_config();
    sm_config_set_wrap(&pioConfig, offset, offset);
    sm_config_set_out_pins(&pioConfig, 0, 8);
    sm_config_set_out_shift(&pioConfig, true, true, 32);
    pio_sm_init(m_pio, m_sm, offset, &pioConfig);
    pio_sm_set_enabled(m_pio, m_sm, true);

    m_dmaChan = dma_claim_unused_channel(true);
    dma_channel_config dmaConfig = dma_channel_get_default_config(m_dmaChan);
    channel_config_set_transfer_data_size(&dmaConfig, DMA_SIZE_32);
    channel_config_set_read_increment(&dmaConfig, true);
    channel_config_set_write_increment(&dmaConfig, false);
    channel_config_set_dreq(&dmaConfig, pio_get_dreq(m_pio, m_sm, true));
    dma_channel_set_config(m_dmaChan, &dmaConfig, false);
    dma_channel_set_trans_count(m_dmaChan, 256/4, false);
    dma_channel_set_write_addr(m_dmaChan, &m_pio->txf[m_sm], false);
    dma_channel_set_irq0_enabled(m_dmaChan, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);
}

void WaveWriter::Start()
{
    m_active = true;
    Continue();
}

void WaveWriter::Stop()
{
    m_active = false;
}

void WaveWriter::SetFrequency(float hz)
{
    m_hz = hz;
}

bool WaveWriter::SetWaveTable(WaveTable waveTable)
{
    if(m_waveNext == nullptr)
    {
        m_waveNext = (m_waveCurrent == &m_wave1 ? &m_wave2 : &m_wave1);
        memcpy(m_waveNext, waveTable, sizeof(WaveTable));
        return true;
    }

    return false;
}

//--------
// PRIVATE
//--------
void WaveWriter::Continue()
{
    if(m_active)
    {
        if(m_waveNext != nullptr)
        {
            m_waveCurrent = m_waveNext;
            m_waveNext = nullptr;
        }

        dma_channel_acknowledge_irq0(m_dmaChan);
        pio_sm_set_clkdiv(m_pio, m_sm, FreqToDiv(m_hz));
        dma_channel_set_read_addr(m_dmaChan, m_waveCurrent, true);
    }
}

float WaveWriter::FreqToDiv(const float hz)
{
    const float scale = 125000000.0 / 256.0;
    return scale / hz;
}
