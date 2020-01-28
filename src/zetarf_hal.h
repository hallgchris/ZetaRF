/*!
 * @brief ZetaRF HAL (Hardware Abstraction Layer) interface.
 * Uses default Arduino SPI & GPIO controls
 *
 * License: see LICENSE file
 */

#pragma once

#include <algorithm>
#include <cstring>
#include <type_traits>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#define ZETARF_SPI_SETTINGS_CHANNEL 0
#define ZETARF_SPI_SETTINGS_FREQ 1000000UL


template<int PinNumber>
struct PinSelector
{
    static constexpr int Pin = PinNumber;
};

struct ChipSelectPinSelector
{
};
struct ShutdownPinSelector
{
};
struct IrqPinSelector
{
};

template<int PinNumber>
struct ChipSelectPin : public ChipSelectPinSelector, public PinSelector<PinNumber>
{
};
template<int PinNumber>
struct ShutdownPin : public ShutdownPinSelector, public PinSelector<PinNumber>
{
};
template<int PinNumber>
struct IrqPin : public IrqPinSelector, public PinSelector<PinNumber>
{
};

namespace ZetaRF
{
    template<int PinNumber>
    using CS = ChipSelectPin<PinNumber>;
    template<int PinNumber>
    using SDN = ShutdownPin<PinNumber>;
    template<int PinNumber>
    using IRQ = IrqPin<PinNumber>;
}


template <class TChipSelectPin, class TShutdownPin, class TIrqPin>
class ZetaRfHal
{
    static constexpr int ChipSelect_pin = TChipSelectPin::Pin;
    static constexpr int ShutdownPin = TShutdownPin::Pin;
    static constexpr int IrqPin = TIrqPin::Pin;

    bool m_inSpiTransaction {false};

public:
    static constexpr bool HasHardwareClearToSend {false};

    ZetaRfHal() {
        // Check pins
        static_assert(std::is_base_of<ChipSelectPinSelector, TChipSelectPin>::value, "First parameter of ZetaRF must be the Chip Select pin. Use ChipSelectPin<10> to use pin 10.");
        static_assert(std::is_base_of<ShutdownPinSelector, TShutdownPin>::value, "Second parameter of ZetaRF must be the Shutdown pin. Use ShutdownPin<9> to use pin 9.");
        static_assert(std::is_base_of<IrqPinSelector, TIrqPin>::value, "Third parameter of ZetaRF must be the IRQ pin. Use IrqPin<8> to use pin 8.");
    }

    bool initialize() {
        wiringPiSetup();
        wiringPiSPISetup(ZETARF_SPI_SETTINGS_CHANNEL, ZETARF_SPI_SETTINGS_FREQ);

        pinMode(ChipSelect_pin, OUTPUT);
        pinMode(IrqPin, INPUT);
        pullUpDnControl(IrqPin, PUD_UP);
        pinMode(ShutdownPin, OUTPUT);

        digitalWrite(ChipSelect_pin, HIGH);

        //putInShutdown();
        return true;
    }

    /*bool isClearToSend() const
    {
        return true; // Not implemented here
    }//*/

    bool isIrqAsserted() const
    {
        return (digitalRead(IrqPin) == LOW);
    }

    void putInShutdown()
    {
        digitalWrite(ShutdownPin, HIGH);
    }
    void releaseFromShutdown()
    {
        digitalWrite(ShutdownPin, LOW);
    }


    void beginSpiTransaction()
    {
        m_inSpiTransaction = true;
        digitalWrite(ChipSelect_pin, LOW);
    }
    void resumeOrBeginSpiTransaction()
    {
        if (m_inSpiTransaction)
            return;
        beginSpiTransaction();
    }
    void restartOrBeginSpiTransaction()
    {
        if (m_inSpiTransaction) {
            endSpiTransaction();
            delayMicroseconds(100);
        }
        beginSpiTransaction();
    }
    void endSpiTransaction()
    {
        delayMicroseconds(1);
        digitalWrite(ChipSelect_pin, HIGH);
        m_inSpiTransaction = false;
    }


    uint8_t spiReadWriteByte(uint8_t value)
    {
        wiringPiSPIDataRW(ZETARF_SPI_SETTINGS_CHANNEL, &value, 1);
        return value;
    }

    void spiWriteByte(uint8_t value)
    {
        spiReadWriteByte(value);
    }
    uint8_t spiReadByte()
    {
        return spiReadWriteByte(0xFF);
    }


    void spiReadWriteData(uint8_t* data, uint8_t count)
    {
        wiringPiSPIDataRW(ZETARF_SPI_SETTINGS_CHANNEL, data, count);
    }

    void spiWriteData(uint8_t const* data, uint8_t count)
    {
        // Make copy of data as wiringPi will overwrite
        uint8_t dataCopy[count];
        memcpy(dataCopy, data, sizeof(dataCopy));
        wiringPiSPIDataRW(ZETARF_SPI_SETTINGS_CHANNEL, dataCopy, count);
    }
    void spiReadData(uint8_t* data, uint8_t count)
    {
        std::fill_n(data, count, 0xFF);
        wiringPiSPIDataRW(ZETARF_SPI_SETTINGS_CHANNEL, data, count);
    }

};


// HAL with GPIO 1 as CTS output
struct ClearToSendPinSelector
{
};
template<int PinNumber>
struct ClearToSendPin : public ClearToSendPinSelector, public PinSelector<PinNumber>
{
};

template <class TChipSelectPin, class TShutdownPin, class TIrqPin, class TClearToSendPin>
class ZetaRfHal_Gpio1AsClearToSend : ZetaRfHal<TChipSelectPin, TShutdownPin, TIrqPin>
{
    static constexpr int ClearToSend_pin = TClearToSendPin::Pin;

public:
    static constexpr bool HasHardwareClearToSend = true;

    ZetaRfHal_Gpio1AsClearToSend() {
        // Check pins
        static_assert(std::is_base_of<ClearToSendPinSelector, TClearToSendPin>::value, "Fourth parameter of ZetaRF must be the Clear To Send pin. Use ClearToSendPin<11> to use pin 11.");
    }

    bool initialize() {
        pinMode(ClearToSend_pin, INPUT);
        pullUpDnControl(ClearToSend_pin, PUD_UP);
        return ZetaRfHal<TChipSelectPin, TShutdownPin, TIrqPin>::initialize();;
    }

    bool isClearToSend() const
    {
        return (digitalRead(ClearToSend_pin) != LOW);
    }
};

