/***************************************************************************
 * Header für die Bitmuster der darzustellenden Wörter auf der LED-Matrix
 *
 * Sprache: deutsch
 *
 *
 **************************************************************************/


/* Vermeidung Doppeldefinitionen */
#pragma once

#define DE_ESIST        Matrix[0]   |= 0b1100111000000000
#define DE_FUENF        Matrix[1]   |= 0b0011110000000000
#define DE_ZEHN         Matrix[1]   |= 0b0000000011110000
#define DE_ZWANZIG      Matrix[2]   |= 0b0000111111100000
#define DE_VIERTEL      Matrix[3]   |= 0b0000111111100000
#define DE_DREIVIERTEL  Matrix[3]   |= 0b1111111111100000
#define DE_VOR          Matrix[4]   |= 0b0011100000000000
#define DE_NACH         Matrix[4]   |= 0b0000000111100000
#define DE_HALB         Matrix[5]   |= 0b1111000000000000
#define DE_H_SIEBEN     Matrix[5]   |= 0b0000001111110000
#define DE_H_FUENF      Matrix[6]   |= 0b0011110000000000
#define DE_H_EIN        Matrix[6]   |= 0b0000000011100000
#define DE_H_EINS       Matrix[6]   |= 0b0000000011110000
#define DE_H_ZWEI       Matrix[7]   |= 0b1111000000000000
#define DE_H_ACHT       Matrix[7]   |= 0b0000001111000000
#define DE_H_DREI       Matrix[8]   |= 0b0011110000000000
#define DE_H_VIER       Matrix[8]   |= 0b0000000111100000
#define DE_H_SECHS      Matrix[9]   |= 0b0111110000000000
#define DE_H_ELF        Matrix[9]   |= 0b0000000011100000
#define DE_H_ZWOELF     Matrix[10]  |= 0b1111100000000000
#define DE_H_ZEHN       Matrix[10]  |= 0b0000001111000000
#define DE_H_NEUN       Matrix[11]  |= 0b0001111000000000
#define DE_UHR          Matrix[11]  |= 0b0000000001110000