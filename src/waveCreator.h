#include <pico/stdlib.h>

class WaveCreator
{
public:
    typedef uint8_t WaveTable[256];
    typedef uint8_t (*WaveTablePtr)[256];

    virtual void Create(WaveTable waveTable, uint8_t terms) = 0;
}