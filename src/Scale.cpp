/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Scale.cpp
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

#include "Scale.h"

char Scale::test_array[] = {1, 2, 3, 4, 5, 6};

int Scale::HALF_TONE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
int Scale::FIFTH[] = {0, 7};
int Scale::MINOR[] = {0, 2, 3, 5, 7, 8, 10};
int Scale::MAJOR[] = {0, 2, 4, 5, 7, 9, 11};
int Scale::MINOR_CHORD[] = {0, 3, 7};
int Scale::MAJOR_CHORD[] = {0, 4, 7};
int Scale::MINOR_CHORD_7[] = {0, 3, 7, 11};
int Scale::MAJOR_CHORD_7[] = {0, 4, 7, 11};
int Scale::MINOR_PENTATONIC[] = {0, 3, 5, 7, 10};
int Scale::MAJOR_PENTATONIC[] = {0, 4, 5, 7, 11};
int Scale::OCTAVE[] = {0};
int Scale::DIMINISHED[] = {0, 3, 6, 9};
int Scale::FIBONACCI[] = {1 - 1, 1 - 1, 2 - 1, 3 - 1, 5 - 1, 8 - 1};
