/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Playlist.cpp
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

#include "Playlist.h"
#include "MovementNull.h"

#define PL_DEBUG false


// ////////////////////////////////////////
// Constructor / Destructor
// ////////////////////////////////////////

Playlist::Playlist() {
    populateMovementIDs();
    firstMovement = true;
}

Playlist::~Playlist() {
    delete[] mMovementIDs;
}

// ////////////////////////////////////////
// Public Methods
// ////////////////////////////////////////

void Playlist::setParent(AutomaticOrchestra *pParent) {
    mParent = pParent;
}

Movement *Playlist::getMovement(int pMovementID) {

#if PL_DEBUG
    Serial.print("### (PL) playlist created with ");
    Serial.print(getNumberOfMovements());
    Serial.print(" movements: ");
    for (uint8_t i = 0; i < getNumberOfMovements(); i++) {
        Serial.print(mMovementIDs[i]);
        Serial.print(" ");
    }
    Serial.println();
#endif

    Movement *mMovement = 0;
    if (firstMovement) {
        mMovement = new MovementNull(parent(), mMovementIDs[0]);
        firstMovement = false;
    } else {
        mMovement = createMovement(pMovementID);
    }
    return mMovement;
}

uint8_t Playlist::getNumberOfMovements() {
    return 1;
}

// ////////////////////////////////////////
// Protected Methods
// ////////////////////////////////////////

Movement* Playlist::createMovement(int pMovementID) {
    Movement* mMovement = new MovementNull(parent(), nextMovementID(pMovementID));
    return mMovement;
}

void Playlist::populateMovementIDs() {
    mMovementIDs = new int[1];
    mMovementIDs[0] = Movement::MOVEMENT_NULL;
}

AutomaticOrchestra* Playlist::parent() {
    return mParent;
}

uint8_t Playlist::nextMovementID(int pMovementID) {
    int mMovementIndex = 0;

    for(int i = 0; i < getNumberOfMovements(); i++){
        if(mMovementIDs[i] == pMovementID){
            mMovementIndex = i + 1;
        }
    }

    if(mMovementIndex >= getNumberOfMovements()){
        mMovementIndex = 0;
    }

#if PL_DEBUG
    Serial.print("### (PL) next movement id is: ");
    Serial.print(mMovementIDs[mMovementIndex]);
    Serial.print(" - located at index: ");
    Serial.println(mMovementIndex);
#endif

    return mMovementIDs[mMovementIndex];
}

