/***************************************************************************
 * Header für die Bitmuster der darzustellenden Wörter auf der LED-Matrix
 *
 * Sprache: englisch
 *
 * Layout der Buchstabenschablone
 *   01234567890
 * 0 ITLISASTIME
 * 1 ACQUARTERDC
 * 2 TWENTYFIFEX
 * 3 HALFBTENFTO
 * 4 PASTERUNINE
 * 5 ONESIXTHREE
 * 6 FOURFIVETWO
 * 7 EIGHTELEVEN
 * 8 SEVENTWELVE
 * 9 TENSEOCLOCK
 *
 **************************************************************************/

/* Vermeidung Doppeldefinitionen */
#pragma once

#define EN_ITIS Matrix[0] |= 0b1101100000000000
#define EN_TIME Matrix[0] |= 0b0000000111100000
#define EN_A Matrix[1] |= 0b1000000000000000
#define EN_QUATER Matrix[1] |= 0b0011111110000000
#define EN_TWENTY Matrix[2] |= 0b1111110000000000
#define EN_FIVE Matrix[2] |= 0b0000001111000000
#define EN_HALF Matrix[3] |= 0b1111000000000000
#define EN_TEN Matrix[3] |= 0b0000011100000000
#define EN_TO Matrix[3] |= 0b0000000001100000
#define EN_PAST Matrix[4] |= 0b1111000000000000
#define EN_H_NINE Matrix[4] |= 0b0000000111100000
#define EN_H_ONE Matrix[5] |= 0b1110000000000000
#define EN_H_SIX Matrix[5] |= 0b0001110000000000
#define EN_H_THREE Matrix[5] |= 0b0000001111100000
#define EN_H_FOUR Matrix[6] |= 0b1111000000000000
#define EN_H_FIVE Matrix[6] |= 0b0000111100000000
#define EN_H_TWO Matrix[6] |= 0b0000000011100000
#define EN_H_EIGHT Matrix[7] |= 0b1111100000000000
#define EN_H_ELEVEN Matrix[7] |= 0b0000011111100000
#define EN_H_SEVEN Matrix[8] |= 0b1111100000000000
#define EN_H_TWELVE Matrix[8] |= 0b0000011111100000
#define EN_H_TEN Matrix[9] |= 0b1110000000000000
#define EN_OCLOCK Matrix[9] |= 0b0000011111100000
