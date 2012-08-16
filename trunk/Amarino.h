/*
MeetAndroid.h - Arduino Library for Amarino
Copyright (c) 2009 Bonifaz Kaufmann.  All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Acknowledgements:
This library is based on the SerialHandler library from Madrang of The
Warrent Team. The original library has been modified to address the 
specific demand to communicate with the Amarino toolkit.

Following changes were made:
- ByteBufferLenght has been increased from 16 to 64
- FunctionBufferLength has been decreased from 128 to 75
- A startFlag and delimiter char has been added.
- ack and abord chars has been made private
- convenient convertion functions added
- constructor simplified
- default error handling introduced
- some more optimizations
- send functions added
- names of most functions changed

last modified by Bonifaz Kaufmann 08 Dec 2011
*/

#ifndef MeetAndroid_h
#define MeetAndroid_h

#include <inttypes.h>
//#include "Print.h"


/******************************************************************************
* Definitions
******************************************************************************/

#define ByteBufferLenght 64
#define FunctionBufferLenght 75 // 48-122 (in ascii: 0 - z)
#define FunctionBufferOffset 48  // offset to calc the position in the function buffer ('0' should be stored in intFunc[0])
#define _MEET_ANDROID_VERSION 4 // software version of this library
#define true 1
#define false 0

/****************FUNCTION PROTOTYPE ****************/
//
//
/*********************************************************/
void   MeetAndroid_processCommand(void);
void   MeetAndroid_init(void);
int    MeetAndroid_getArrayLength();

void   MeetAndroid_flush(void);
void MeetAndroid_init();
unsigned char  MeetAndroid_receive(void);
void  MeetAndroid_registerFunction(void(*)(uint8_t, uint8_t),uint8_t);
void   MeetAndroid_unregisterFunction(uint8_t);
//	int MeetAndroid_bufferLength(){return bufferCount;} // buffer withouth ACK
//	int MeetAndroid_stringLength(){return bufferCount;} // string without flag but '/0' at the end
//void  MeetAndroid_getBuffer(uint8_t[]);
//
//void MeetAndroid_getString(char[]);
int MeetAndroid_getInt();
//long MeetAndroid_getLong();
//float MeetAndroid_getFloat();
//double MeetAndroid_getDouble();
//void MeetAndroid_getIntValues(int[]);
//void MeetAndroid_getFloatValues(float[]);
//void MeetAndroid_getDoubleValues(float[]); // in Arduino double and float are the same

void MeetAndroid_send(char);
//    void MeetAndroid_send(const char[]);
//    void MeetAndroid_send(uint8_t);
//    void MeetAndroid_send(int);
//    void MeetAndroid_send(unsigned int);
//    void MeetAndroid_send(long);
//    void MeetAndroid_send(unsigned long);
//    void MeetAndroid_send(long, int);
//    void MeetAndroid_send(double);
//    void MeetAndroid_sendln(void);



static int library_version() { 
  return _MEET_ANDROID_VERSION;} 
//};

// Arduino 0012 workaround
#undef int
#undef char
#undef long
#undef byte
#undef float
#undef abs
#undef round 

#endif
