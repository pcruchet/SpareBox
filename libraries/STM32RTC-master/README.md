# STM32RTC
A RTC library for STM32.

# API

This library is based on the Arduino RTCZero library.
The library allows to take control of the internal RTC of the STM32 boards.

The following functions are not supported:

* **`void standbyMode()`**: use the STM32 Low Power library instead.
* **`uint8_t getAlarmMonth()`**: month not supported by STM32 RTC architecture.
* **`uint8_t getAlarmYear()`**: year not supported by STM32 RTC architecture.
* **`void setAlarmMonth(uint8_t month)`**: month not supported by STM32 RTC architecture.
* **`void setAlarmYear(uint8_t year)`**: year not supported by STM32 RTC architecture.
* **`void setAlarmDate(uint8_t day, uint8_t month, uint8_t year)`**: month and year not supported by STM32 RTC architecture.

The following functions have been added to support specific STM32 RTC features:

_RTC hours mode (12 or 24)_
* **`void begin(RTC_Hour_Format format)`**

_RTC clock source_
* **`void setClockSource(RTC_Source_Clock source)`** : this function must be called before `begin()`.

_RTC Asynchronous and Synchronous prescaler_
* **`void getPrediv(int8_t *predivA, int16_t *predivS)`** : get user (a)synchronous prescaler values if set else computed ones for the current clock source.
* **`void setPrediv(int8_t predivA, int16_t predivS)`** : set user (a)synchronous prescaler values.  This function must be called before `begin()`. Use -1 to reset value and use computed ones.

_SubSeconds management_
* **`uint32_t getSubSeconds(void)`**
* **`void setSubSeconds(uint32_t subSeconds)`**

_Hour format (AM or PM)_
* **`uint8_t getHours(RTC_AM_PM *period)`**
* **`void setHours(uint8_t hours, RTC_AM_PM period)`**
* **`void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint32_t subSeconds, RTC_AM_PM period)`**
* **`void setAlarmHours(uint8_t hours, RTC_AM_PM period)`**
* **`uint8_t getAlarmHours(RTC_AM_PM *period)`**
* **`void setAlarmTime(uint8_t hours, uint8_t minutes, uint8_t seconds, RTC_AM_PM period)`**

_Week day configuration_
* **`uint8_t getWeekDay(void)`**
* **`void setWeekDay(uint8_t weekDay)`**
* **`void setDate(uint8_t weekDay, uint8_t day, uint8_t month, uint8_t year)`**

Refer to the Arduino RTC documentation for the other functions  
http://arduino.cc/en/Reference/RTC

## Source

Source files available at:  
https://github.com/stm32duino/STM32RTC
