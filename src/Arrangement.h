/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Arrangement.h
 Copyright (c) 2016 Automatic Orchestra

 See the COPYRIGHT file at the top-level directory of this distribution and at:
 https://github.com/automatic-orchestra/ao_library/blob/master/COPYRIGHT.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 This file is part of Automatic Orchestra. It is subject to the license terms
 in the LICENSE file found in the top-level directory of this distribution and at:
 https://github.com/automatic-orchestra/ao_library/blob/master/LICENSE.

 No part of Automatic Orchestra, including this file, may be copied, modified,
 propagated, or distributed except according to the terms contained in the
 LICENSE file.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#ifndef ARRANGEMENT_H
#define ARRANGEMENT_H

#include <Arduino.h>

struct orchestra_member_t {
    String macAddress;
    unsigned int midiChannel;
    bool klockMeister;
};

/**
 * This is a SINGLETON class!!!
 * Code derived from here:
 * http://stackoverflow.com/questions/270947/can-any-one-provide-me-a-sample-of-singleton-in-c/271104#271104
 */
class Arrangement
{
private:
    Arrangement() {};
    Arrangement(Arrangement const&); // Don't Implement.
    void operator=(Arrangement const&); // Don't implement	
    orchestra_member_t* mList;
    const uint8_t DEFAULT_SIZE = 12;
    uint8_t mSize = 0;
public:
    static Arrangement& getInstance();
    void init(uint8_t pSize = 0);
    void init(String macAddresses[], uint8_t pSize);
    orchestra_member_t* getList();
    uint8_t getSize();
};

#endif //ARRANGEMENT_H