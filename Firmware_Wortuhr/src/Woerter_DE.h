/***************************************************************************
 * Header für die Bitmuster der darzustellenden Wörter auf der LED-Matrix
 *
 * Sprache: deutsch
 *
 *
 **************************************************************************/

/* Vermeidung Doppeldefinitionen */
#pragma once

#define DE_ESIST Matrix[0] |= 0b011011100000       //0
#define DE_FUENF Matrix[1] |= 0b001111000000       //1
#define DE_ZEHN Matrix[1] |= 0b000000001111        //1
#define DE_ZWANZIG Matrix[2] |= 0b000011111110     //2
#define DE_VIERTEL Matrix[3] |= 0b000011111110     //3
#define DE_DREIVIERTEL Matrix[3] |= 0b111111111110 //3
#define DE_VOR Matrix[4] |= 0b001110000000         //4
#define DE_NACH Matrix[4] |= 0b000000011110        //4
#define DE_HALB Matrix[5] |= 0b111100000000        //5
#define DE_H_SIEBEN Matrix[5] |= 0b000001111110    //5
#define DE_H_FUENF Matrix[6] |= 0b001111000000     //6
#define DE_H_EIN Matrix[6] |= 0b000000001110       //6
#define DE_H_EINS Matrix[6] |= 0b000000001111      //6
#define DE_H_ZWEI Matrix[7] |= 0b111100000000      //7
#define DE_H_ACHT Matrix[7] |= 0b000000111100      //7
#define DE_H_DREI Matrix[8] |= 0b001111000000      //8
#define DE_H_VIER Matrix[8] |= 0b000000011110      //8
#define DE_H_SECHS Matrix[9] |= 0b011111000000     //9
#define DE_H_ELF Matrix[9] |= 0b000000000111       //9
#define DE_H_ZWOELF Matrix[10] |= 0b111110000000   //10
#define DE_H_ZEHN Matrix[10] |= 0b000000111100     //10
#define DE_H_NEUN Matrix[11] |= 0b011110000000     //11
#define DE_UHR Matrix[11] |= 0b000000011100        //11
