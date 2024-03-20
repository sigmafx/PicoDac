#include "waveWriter.h"

#include "hardware/structs/syscfg.h"
#include "pico/stdlib.h"
#include "PicoDac.pio.h"

#include <hardware/dma.h>
#include <hardware/adc.h>
#include <math.h>
#include <tusb.h>

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

WaveWriter::WaveTable wave;

int main() {
    stdio_init_all();
    //while (!tud_cdc_connected()) { sleep_ms(100); }
  
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    waveWriter.Initialise();

    for (uint16_t idx = 0; idx < 256; idx++)
    {
        double angle = (2.0 * M_PI * idx) / 256.0;
        double series = 0.0;
        for(double term = 1.0; term < 2.0; term += 2.0)
        {
            series += sin(term * angle) / term;
        }
        wave[idx] = 127 + 64 * series;
    }

    waveWriter.SetFrequency(261.63);
    waveWriter.SetWaveTable(wave);
    waveWriter.Start();

    while(true)
    {
        double terms = 2.0 + (adc_read() / 50.0);

        for (uint16_t idx = 0; idx < 256; idx++)
        {
            double angle = (2.0 * M_PI * idx) / 256.0;
            double series = 0.0;
            for(double term = 1.0; term < terms; term += 2.0)
            {
                series += sin(term * angle) / term;
            }
            wave[idx] = 127 + 96 * series;
        }

        waveWriter.SetWaveTable(wave);
    }

    return 0;
}
