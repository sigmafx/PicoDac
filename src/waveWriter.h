#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/sync.h>
#include <pico/critical_section.h>

class WaveWriter;
extern WaveWriter waveWriter;

class WaveWriter
{
    public:
        typedef uint8_t WaveTable[256];
        typedef uint8_t (*WaveTablePtr)[256];

        WaveWriter(PIO pio, uint sm);
        void Initialise();
        void Start();
        void Stop();
        void Continue();
        void SetFrequency(float hz);
        bool SetWaveTable(WaveTable waveTable);

    private:
        WaveTable m_wave1;
        WaveTable m_wave2;
        WaveTablePtr m_waveCurrent;
        WaveTablePtr m_waveNext;
        float m_hz;
        PIO m_pio;
        uint m_sm;
        int m_dmaChan;
        bool m_active;
        critical_section_t m_criticalSection;

        float FreqToDiv(float hz);
};