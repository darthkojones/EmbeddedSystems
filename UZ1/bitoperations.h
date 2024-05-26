#ifndef BITOPERATIONS_H
#define BITOPERATIONS_H

// Sets the bit at POSITION
#define SB(VALUE, POSITION) ((VALUE) |= (1U << (POSITION)))

// Clears the bit at POSITION
#define CB(VALUE, POSITION) ((VALUE) &= ~(1U << (POSITION)))

// Checks if the bit at POSITION is set
#define IBS(VALUE, POSITION) (((VALUE) & (1U << (POSITION))) != 0)

// Toggles the bit at POSITION
#define TB(VALUE, POSITION) ((VALUE) ^= (1U << (POSITION)))

#endif // BITOPERATIONS_H
