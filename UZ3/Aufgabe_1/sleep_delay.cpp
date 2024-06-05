#include "sleep_delay.h"
#include <avr/interrupt.h>
#include <avr/wdt.h>

volatile uint32_t sleep_counter = 0;
volatile bool wdt_triggered = false;

// Watchdog Timer interrupt service routine
ISR(WDT_vect) {
    wdt_disable(); // Disable watchdog timer
    wdt_triggered = true; // Set flag to indicate watchdog interrupt
}

void sleep_delay(uint32_t seconds) {
    uint32_t wakeup_count = seconds / 8;
    uint32_t remaining_seconds = seconds % 8;

    // Setup Watchdog Timer
    MCUSR &= ~(1 << WDRF); // Clear reset flag
    WDTCSR |= (1 << WDCE) | (1 << WDE); // Enable configuration changes
    WDTCSR = (1 << WDP3) | (1 << WDP0) | (1 << WDIE); // Set WDT to 8 seconds and enable interrupt mode

    while (wakeup_count > 0) {
        wdt_triggered = false;
        // Enter sleep mode
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sei(); // Enable global interrupts
        sleep_cpu(); // Put the device to sleep

        // Sleep until Watchdog Timer interrupt
        sleep_disable();
        cli(); // Disable global interrupts

        if (wdt_triggered) {
            wakeup_count--;
        }
    }

    // Handle remaining seconds
    if (remaining_seconds > 0) {
        wdt_triggered = false;
        uint8_t wdt_prescaler = (remaining_seconds <= 2) ? (1 << WDP1) : (1 << WDP1) | (1 << WDP2);
        WDTCSR |= (1 << WDCE) | (1 << WDE);
        WDTCSR = wdt_prescaler | (1 << WDIE); // Set WDT to the remaining time and enable interrupt mode

        // Enter sleep mode
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sei(); // Enable global interrupts
        sleep_cpu(); // Put the device to sleep

        // Sleep until Watchdog Timer interrupt
        sleep_disable();
        cli(); // Disable global interrupts
    }

    // Disable Watchdog Timer
    wdt_disable();
}
