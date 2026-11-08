# Lab 7 RTC - Student Starter Guide

This starter keeps the original Keil project structure and working LED/USART setup.
Complete the five TODO sections in `RTC.c` and `main.c`.

## Expected behavior after completion

- The RTC uses the **32.768 kHz LSE** clock.
- The starting date is **03-31-2015**.
- The starting time is **11:59:55 PM**.
- The onboard green LED toggles once per main-loop iteration.
- Tera Term displays the RTC time/date using **9600 baud, 8N1**.
- Because this version uses a software delay rather than an RTC alarm interrupt, some seconds may be skipped in the terminal output.

Example output:

```text
11:59:55, 03-31-2015
11:59:56, 03-31-2015
11:59:58, 03-31-2015
12:00:00, 03-31-2015
```

## TODOs

1. **RTC clock setup** - start the LSE, wait for it to be ready, select LSE for RTC, and enable RTC.
2. **RTC write protection** - implement the unlock and lock key writes.
3. **Set date/time** - call the provided calendar/time helper functions with the required BCD values.
4. **Calendar formatting** - convert BCD fields to normal decimal and format the strings.
5. **Main loop display** - read the RTC, format the combined line, and send it through USART2.

## Files students should edit

- `RTC.c`
- `main.c`

The remaining project files are provided as working support code.
