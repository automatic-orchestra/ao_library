/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Playlist.h
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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <Arduino.h>
#include "AutomaticOrchestra.h"
#include "Movement.h"


//TODO add control message to go to next movement
class Playlist {
public:
    Playlist();
    ~Playlist();
    void setParent(AutomaticOrchestra* pParent);
    Movement* getMovement(int pMovementID);
    virtual uint8_t getNumberOfMovements();
protected:
    virtual Movement* createMovement(int pMovementID);
    virtual void populateMovementIDs();
    AutomaticOrchestra* parent();
    uint8_t nextMovementID(int pMovementID);
    int* mMovementIDs;
private:
    AutomaticOrchestra* mParent;
    bool firstMovement;
};

#endif //PLAYLIST_H
