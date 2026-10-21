# Irrigation Logger

A small data logger for an Arduino Pro Mini with three inputs, an SD card, a DS3231 real-time clock and a buzzer. Moritz built it to log the water meters of three irrigation zones in a potato field trial, as told in his blog post [The Power of small Micro-Electronics Projects](https://moritznelle.com/the-power-of-small-micro-electronics-projects/).

## What it logs
Each time one of the three inputs is pulled to GND, the logger writes a line to the SD card with the date and time from the DS3231 and the number of the input (1, 2 or 3). It prints the same line over serial and plays a short beep. Each input has its own pitch.

The code calls the inputs buttons. It reacts to the moment an input goes from HIGH to LOW. A second press on the same input within 100 ms of the last logged one is ignored (`bunce` in `src/main.cpp`).

## Hardware and pins

| Part | Pin | Notes |
|---|---|---|
| Input 1 | D2 | Internal pull-up, active when connected to GND |
| Input 2 | D3 | Internal pull-up, active when connected to GND |
| Input 3 | D4 | Internal pull-up, active when connected to GND |
| Buzzer | D9 | Driven with `tone()` |
| SD card module | D10 (CS) | Uses the hardware SPI pins D11 (MOSI), D12 (MISO) and D13 (SCK) |
| DS3231 RTC | A4 (SDA), A5 (SCL) | Default I2C pins |

Because the inputs use the internal pull-up resistors, each button or contact goes between its pin and GND.

The PlatformIO board is `pro16MHzatmega328`, the 16 MHz ATmega328 version of the Pro Mini.

## Build and flash with PlatformIO
PlatformIO installs the libraries listed in `platformio.ini` (SD, RTClib, Adafruit BusIO) on the first build.

```
pio run                 # build
pio run -t upload       # build and flash
pio device monitor      # serial output at 9600 baud
```

The Pro Mini has no USB port, so flashing needs a USB-to-serial adapter.

This code reads the time from the DS3231 but never sets it. Set the clock once before you use the logger, for example with the `ds3231` example sketch that comes with RTClib.

## File format
The logger writes to `test.txt` in the root of the SD card. Despite its name, this is the data file. The logger creates it at startup if it is missing and always adds new lines at the end, so data from earlier sessions stays in the file.

Each line holds a timestamp and the input number, separated by a comma. For example:

```
2024-9-25 8:5:7,1
2024-9-25 8:5:41,1
2024-9-25 8:6:2,3
```

- The date is `year-month-day` and the time is `hour:minute:second` in 24-hour format.
- Numbers have no leading zeros, so `8:5:7` means 08:05:07. A comment in the code says otherwise, but the code does not pad them.
- There is no header line. Lines end with `\r\n`.

## Sounds

| Sound | Meaning |
|---|---|
| Three rising beeps (2200, 2500 and 2800 Hz) | Startup finished |
| One short beep at 2000 Hz | Input 1 logged |
| One short beep at 2200 Hz | Input 2 logged |
| One short beep at 2500 Hz | Input 3 logged |
| Two alternating tones, five times (about 3 seconds) | Error |

At startup:
- If the SD card fails to start, the logger prints `SD card initialization failed!` and plays no sound at all. Every input then plays the error sound.
- If the DS3231 is not found, it prints `DS3231 initialization failed!` and plays the error sound instead of the startup beeps. Fix the clock before logging.
- If `test.txt` cannot be created, it plays the error sound and then the startup beeps.

While logging, the error sound means the file could not be opened and that entry was not saved.
