/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 MacAddress.cpp
 Copyright (c) 2016 Automatic Orchestra

 See the COPYRIGHT file at the top-level directory of this distribution and at:
 https://github.com/automatic-orchestra/library-for-arduino/blob/master/COPYRIGHT.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 This file is part of Automatic Orchestra. It is subject to the license terms
 in the LICENSE file found in the top-level directory of this distribution and at:
 https://github.com/automatic-orchestra/library-for-arduino/blob/master/LICENSE.

 No part of Automatic Orchestra, including this file, may be copied, modified,
 propagated, or distributed except according to the terms contained in the
 LICENSE file.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include "MacAddress.h"

String MacAddress::get() {
    uint8_t buffer[BUFFER_LENGTH];

    read(0xe, buffer, 0);
    read(0xf, buffer, 3);

    String s = "";
    for (uint8_t i = 0; i < 6; ++i) {
        if (i != 0) s += ":";
        s += String((*(buffer + i) & 0xF0) >> 4, HEX);
        s += String(*(buffer + i) & 0x0F, HEX);
    }

    return s;
}

void MacAddress::read(uint8_t word, uint8_t *buffer, uint8_t offset) {
    FTFL_FCCOB0 = 0x41; // Selects the READONCE command
    FTFL_FCCOB1 = word; // read the given word of read once area

    // launch command and wait until complete
    FTFL_FSTAT = FTFL_FSTAT_CCIF;
    while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF));

    *(buffer + offset) = FTFL_FCCOB5; // collect only the top three bytes,
    *(buffer + offset + 1) = FTFL_FCCOB6; // in the right orientation (big endian).
    *(buffer + offset + 2) = FTFL_FCCOB7; // Skip FTFL_FCCOB4 as it's always 0.
}