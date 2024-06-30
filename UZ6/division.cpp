#include <Arduino.h>


inline void division(uint8_t dividend, uint8_t divisor) {
	char outputBuffer[128];
	uint8_t quotient, rest;

	/* Divisionsalgorithmus:
	
	Eingabe:
		- Divident
		- Divisor
	Ausgabe:
		- Quotient
		- Rest
	
	quotient = 0;
	while (dividend >= divisor) {
		dividend -= divisor;
		quotient += 1;
	}
	rest = dividend;
	*/

	__asm__ (
    "clr %[quotient]\n"              // Initialize quotient to 0
    "mov r16, %[dividend]\n"         // Move dividend to r16 (	dividend = r16;)
    
    "loop_start:\n"
    "cp r16, %[divisor]\n"           // Compare r16 (dividend) with divisor
    "brlt loop_end\n"                // Branch if less than (dividend < divisor)
    
    "sub r16, %[divisor]\n"          // Subtract divisor from dividend
    "inc %[quotient]\n"              // Increment quotient
    "rjmp loop_start\n"              // Jump back to start of loop
    
    "loop_end:\n"
    "mov %[rest], r16\n"             // Move remaining value (rest) to output
    
    /* output-operand-list */
    : [quotient] "=d" (quotient), [rest] "=d" (rest)
    /* input-operand-list */
    : [dividend] "d" (dividend), [divisor] "d" (divisor)
    /* clobber-list */
    : "r16"
);

	snprintf(outputBuffer, 128, "%d/%d = %d Rest %d", dividend, divisor, quotient, rest);
	Serial.println(outputBuffer);
}


void setup() {
	Serial.begin(115200);
}

void loop() {

	division(33, 5);
	division(60, 5);
	division(4, 5);
	division(5, 5);
	division(0, 5);

	delay(2000);
}
