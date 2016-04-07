/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Orchestra.h
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

#ifndef ORCHESTRA_H
#define ORCHESTRA_H

#include <Arduino.h>


class Orchestra
{
public:
  Orchestra();
  ~Orchestra();
  String getMacAddress();
  int getChannel();
  bool isKlockMeister();
protected:
  void setupDeviceParameters(String pMacAddress);
  String mMacAddress;
  int mMidiChannel;
  bool mKlockMeister;
};


#endif //ORCHESTRA_H