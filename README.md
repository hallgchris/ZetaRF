ZetaRF ![Version 1.1.11](https://img.shields.io/badge/Version-1.1.11-blue.svg)
======
**Written by**: *Benjamin Balga.*
**Copyright**: ***2018***, *Benjamin Balga*, released under BSD license.

**Ported by**: *Christopher Hall.*

This is a fork of [GIPdA's ZetaRF library](https://github.com/GIPdA/ZetaRF) for Arduino, ported to run on the Raspberry Pi and with an added python wrapper. Work in progress!

## About

ZetaRF is an Arduino library for ZETA modules from [RF Solutions][2], which implements a Silicon Labs [Si4455][3] Low Current Sub-GHz Transceiver

RF Solutions does not provide ready to use sample code in C/C++ to use their ZETA modules directly, but either PIC ASM code or Arduino library if you interface your module with their CODEC chip. This library enables you to communicate directly with the ZETA module without the CODEC chip.

ZETA modules communicates over high-speed SPI and 2 gpio.

This library is based on code examples from Silicon Labs [Wireless Development Suite][1], largely modified.

**ZetaRF library is optimized for very low latency (a few milliseconds). That costs a bit of reliability, packets may be dropped.** You can expect about 4ms of latency for a 8-byte packet.

--

**Beta release, everything has not been tested yet!**

> This library may not be directly compatible with any board implementing Si4455 chip! Currently it was only tested with the ZETA-868-SO and ZETA-433-SO modules from RF Solutions.

ZETA module datasheet: <https://www.rfsolutions.co.uk/downloads/1456219226DS-ZETA.pdf>



## Data Packets

**NOTE:** At this point in time, the python wrapper only supports fixed size packets. (Both variable and fixed size will work from the C++ API, however)

The Si4455 chip has two options: variable length or fixed length packets.
Both cannot be used together, write

	#define VARIABLE_LENGTH_ON
before including ZetaRF.h to activate variable length packets. *See the warning below about variable length packets.*


### Fixed size packets
The library use fixed size packets by default.

Default packet size is 8 bytes, but you can specify the size you want by passing the value to `begin(channel, packet size)`.

Receive and sending methods use this packet length by default unless you specify the length to receive/send (thus you're not limited to the length specified in begin(), but you must tell the send and read methods what size you expect in that case).

The buffer pointed by data in `readPacket()` must be at least the size of the packet.


### Variable length packets

When variable length is activated, the first byte in the buffer is the packet length (size byte excluded), at sending and receiving.

To receive variable length packets, listening is done with a length of zero (handled automatically by `startListening()` and `startListening(channel)`).

At this time you cannot peek the packet size before read.

> Few tests have shown that variable packet may not be very reliable, extensive testings are needed.

## Usage

### Python API

#### Building, importing and initialising

You will need the `wiringPi`, `cmake` and `cython` library installed on your Pi. Clone this library, and run `cmake .` followed by `make` to build the project. This will create a shared library with a name similar to `zetarf433.cpython-37m-arm-linux-gnueabihf.so`. This can be imported into ordinary `.py` files with a normal import statement:

```python
from zetarf433 import PyZetaRF433
```

At the current point in time, the frequency (433 MHz) is hard-coded. See the section "Pin connections" for how to change this.

A new instance of the zeta library is created and initialised with

```python
from zetarf433 import PyZetaRF433
zeta = PyZetaRF433()
if not zeta.begin():
    print("Zeta begin failed")
```

This will initialise the chip. `begin()` must be called before any other functions. If an issue occurs with the Zeta module, `begin()` may be called to reset it at any time.

The default packet size is 8 bytes, but this can be overridden by alternatively calling `beginWithPacketLengthOf(packet_length)`. This can manually e overridden later also. The maximum packet length is 80 bytes.

#### Sending data

Send a packet using

```python
def send_packet(self, channel: int, data: bytes, packet_length: int) -> bool
```

This will send the first `packet_length` bytes of `data`. If `data` is shorter than `packet_length`, zero bytes are automatically appended.

The `check_for` function can then be used to see when the packet is sent:

```python
from zetarf433 import Status
if zeta.check_for(Status.DataTransmitted):
    print("Data transmitted")
```

#### Receiving data

To begin listening for packets, call

```python
def start_listening_on_channel(self, channel: int) -> bool
```

This will return true if the operation was successful.

You can then wait for a message to arrive with

```python
def check_recieved(self) -> bool
```

which will return true when there is a packet ready to be read. This is done with

```python
def read_packet(self, packet_length: int) -> bytes
```

which will return the packet as a Python `bytes` object.

### C++ API

#### Including the SPI header file 

It has been reported that on the latest Arduino IDE you have to explicitly add the following before you include the ZetaRF.h file, even though 
the SPI.h file is included by the ZetaRF.h file itself. Not doing this will generate many compiler warnings about a 'missing' SPI.h file.

	#include <SPI.h>

#### 868 MHz or 433 MHz operation

By default this library configures the Zeta for 868 MHz operation. If you instead have a 433MHz module, then add this before including the ZetaRF.h file:

	#define ZETARF_FREQUENCY_433MHZ 1

#### Power Supply

Zeta modules **only supports 1.8V to 3.6V operation**, and malfunction issues have been reported with 5V-level signals (e.g. 5V Arduino with 3.3V Zeta). Do not use voltage levels greater than VDD. If you can, power everything with 3.3V or use level shifters (voltage divider or else).

#### Send data

	void sendPacket(const uint8_t *data);

Transmit a packet (channel and packet size set in begin()). FIFO size is 64 bytes (data is overwritten if you overshoot).

To transmit on a different channel, use:

	void sendPacket(uint8_t channel, const uint8_t *data);

##### Check if data was transmitted

	bool checkTransmitted();

Returns true when data was successfully sent.

The chip automatically goes back to listening when the transmit is done.



#### Receive data

##### Switch to receive mode

	void startListening();

Chip must be in listening mode in order to receive data (listening channel set in `begin()`).

To receive on a different channel, use:

	void startListening(uint8_t channel);

##### Check if data is available

	bool checkReceived();

Returns true if data is available.

##### Read data

	uint8_t readPacket(uint8_t *data);

Read a packet from FIFO, returns the number of bytes read or -1 on error.  FIFO has a capacity of 64 bytes.

##### Errors

	bool systemError();

Returns true if a system error occured (auto clears). System error happens when the module is not responding correctly. In that case, reset the module by calling begin() again.

##### Examples

See code examples for more details.
(TODO: add more examples)


## Installation
Copy the library folder to your Arduino library folder.

## Pin connections

ZETA Pin #|ZETA Module|Arduino|Description
----------|-----------|-------|-----------
1         |ANT        |-      |Antenna (small wire for tests works great (86mm long for 868MHz))
2         |GND        |GND    |Power Ground
3         |SDN        |GPIO   |Shutdown (active high)
4         |VDD        |VDD    |Power (1.8V to 3.6V)
5         |nIRQ       |GPIO   |IRQ (active low)
6         |NC         |-      |Not Connected
7         |GPIO1      |-      |Not Used
8         |GPIO2      |-      |Not Used
9         |SCLK       |SCLK   |SPI Clock
10        |SDI        |MOSI   |Zeta SPI In to Arduino SPI Out
11        |SDO        |MISO   |Zeta SPI Out to Arduino SPI In
12        |nSEL       |CS     |Chip Select (active low)

(**Note** - Possible improvement: a GPIO could be used for CTS instead of polling a register)

At this point in time, in the python version, the three GPIO connections are hard-coded. These can be changed by modifying the definition of `ZetaRF433_PresetPins`, at the bottom of `src/ZetaRF.h`. The radio frequency may also be modified here. These pins are default:

| Pin name        | Wiring Pi pin number |
| --------------- | -------------------- |
| `ChipSelectPin` | 6                    |
| `ShutdownPin`   | 9                    |
| `IrqPin`        | 8                    |




## Configuration files
The project contains WDS XML configuration files, used to generate `radio_config_xx.h` files, if you want to change some settings. Be careful, it might brake things if you don't know what you're doing.

Note: FRR are used and enforced by the library (on begin()) to read module state and interrupt registers. Changing the FRR configuration after begin() will break the library.


## Todo List
- More examples
- Frequency selection with enum instead of define
- Configurable auto-return to listening mode
- Sleep/Active status commands
- Add available() method and FIFO capacity getters


## License
See LICENSE file.

[1]: http://www.silabs.com/products/wireless/EZRadio/Pages/Si4455.aspx "Wireless Development Suite"
[2]: https://www.rfsolutions.co.uk/radio-modules-c10/name-c49/zeta-c86 "RF Solutions"
[3]: http://www.silabs.com/products/wireless/EZRadio/Pages/Si4455.aspx "Si4455"

b
