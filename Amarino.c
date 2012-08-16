/*
  MeetAndroid.cpp - Arduino Library for Amarino
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
*/

// Includes
//#if defined(ARDUINO) && ARDUINO >= 100
//#include "Arduino.h"
////#else
////#include "WProgram.h"
////#include "WConstants.h"
//#endif

//#include "HardwareSerial.h"
#include "inc/hw_types.h"
#include "drivers/display96x16x1.h"
#include "Amarino.h"
#include "Test2\Serial.h"
#include <stdlib.h>
//
//extern "C" {
//#include <stdlib.h>
//}

#define true 1
#define false 0

/**************** VARIABLE DEFINATION ****************/
//
//
/*********************************************************/
char abord;
char ack;
char delimiter;
char startFlag; // used to communicate with Android (leads each msg to Android)

static uint8_t bufferCount;
static uint8_t buffer[100];//ByteBufferLenght];

static int numberOfValues;

static unsigned char customErrorFunc;


static uint16_t waitTime;
typedef void (*H_voidFuncPtr)(uint8_t, uint8_t);
static H_voidFuncPtr intFunc[FunctionBufferLenght];
static H_voidFuncPtr errorFunc;

/**************** FUNCTION DECLARATION ****************/
//
//
/*********************************************************/

/**************** delayMicroseconds ****************/
//Name;       delayMicroseconds
//Parametter: value
//Return:     none
//Description:  
/*********************************************************/
void delayMicroseconds(int val) //CHUA CHINH XAC
{
  unsigned int j,i;
  for(i=0;i<val;i++)
     for(j=0;j<1000;j++);
}

/**************** FUNCTION Amarino ****************/
//Name;       MeetAndroid
//Parametter: none
//Return:     none
//Description:  init
/*********************************************************/

////////////////////MeetAndroid_init////////////////////
void MeetAndroid_init()
{
	waitTime = 30;
	startFlag = 18;
	ack = 19;
	abord = 27;
	delimiter = 59; //';'
	numberOfValues = 0;	
	for(int a = 0;a < FunctionBufferLenght;a++){
		intFunc[a] = errorFunc;
	}
}

////////////////////MeetAndroid_processCommand////////////////////
void MeetAndroid_processCommand(){
	if(buffer[0]-FunctionBufferOffset < FunctionBufferLenght){
		void (*H_FuncPtr)(uint8_t, uint8_t) = intFunc[buffer[0]-FunctionBufferOffset];
		if (H_FuncPtr != 0) {
			H_FuncPtr(buffer[0], MeetAndroid_getArrayLength());
		}
		else {
			//MeetAndroid_send("Flag not registered: ");
			MeetAndroid_send(buffer[0]);
		}
	}
	else {
		if (customErrorFunc)
			errorFunc(buffer[0], MeetAndroid_getArrayLength());
		else {
			//MeetAndroid_send("Flag out of bounds: ");
			MeetAndroid_send(buffer[0]);
		}
	}
}

////////////////////MeetAndroid_registerFunction////////////////////
void MeetAndroid_registerFunction(void(*userfunction)(uint8_t, uint8_t),uint8_t command){
	intFunc[command-FunctionBufferOffset] = userfunction;
}

////////////////////MeetAndroid_unregisterFunction//////////////////////
void MeetAndroid_unregisterFunction(uint8_t command){
	intFunc[command-FunctionBufferOffset] = errorFunc;
}

////////////////////MeetAndroid_receive////////////////////
unsigned char MeetAndroid_receive(){
	uint8_t lastByte;
	unsigned char timeout = false;
	while(!timeout)
	{
		while(serial_available() > 0)
		{
			lastByte = serial_read();
			
			if(lastByte == abord){
                                                                Display96x16x1Clear();
                               Display96x16x1StringDraw("abord",0,0);
				MeetAndroid_flush();
			}
			else if(lastByte == ack){
                                   Display96x16x1Clear();
                               Display96x16x1StringDraw("ack",0,0);
				MeetAndroid_processCommand();
				MeetAndroid_flush();

			}
			else if(bufferCount < ByteBufferLenght){
                               Display96x16x1Clear();
                               Display96x16x1StringDraw("getbyte",0,0);
				buffer[bufferCount] = lastByte;
				bufferCount++;
			}
			else return false;
		}
		
		if((serial_available() <= 0) && !timeout){
			if(waitTime > 0) delayMicroseconds(waitTime);
			if(serial_available() <= 0) timeout = true;
		}
	}
	return timeout;
}

////////////////////MeetAndroid_getArrayLength////////////////////
int MeetAndroid_getArrayLength()
{
	if (bufferCount == 1) return 0; // only a flag and ack was sent, not data attached
	numberOfValues = 1;
	// find the amount of values we got
	for (int a=1; a<bufferCount;a++){
		if (buffer[a]==delimiter) numberOfValues++;
	}
	return numberOfValues;
}

/**************** FUNCTION SERIAL ****************/
//Name;       serial_print
//Parametter: value
//Return:     none
//Description:  
/*********************************************************/
////////////////////MeetAndroid_send////////////////////
void MeetAndroid_send(char c ){
  
  unsigned char *tmp = 0;
//	serial_print((char*)startFlag);
//	serial_print((char*)c);
//	serial_print((char*)ack);
  tmp[0]=startFlag;
  serial_print(tmp);
  tmp[0] = c;
  serial_print(tmp);
  tmp[0] = ack;
  serial_print(tmp);
  
}

////////////////////MeetAndroid_flush////////////////////
void MeetAndroid_flush(){
	for(uint8_t a=0; a < ByteBufferLenght; a++){
		buffer[a] = 0;
	}
	bufferCount = 0;
	numberOfValues = 0;
}

/**************** FUNCTION Amarino get values ****************/
//Name;       get<type>
//Parametter: none
//Return:     buffer *b
//Description:  many function to get various type of value
/*********************************************************/

////////////////////MeetAndroid_getBuffer////////////////////
void MeetAndroid_getBuffer(uint8_t buf[]){

	for(int a = 0;a < bufferCount;a++){
		buf[a] = buffer[a];
	}
}

////////////////////MeetAndroid_getString////////////////////
void MeetAndroid_getString(char string[]){

	for(int a = 1;a < bufferCount;a++){
		string[a-1] = buffer[a];
	}
	string[bufferCount-1] = '\0';
}

////////////////////MeetAndroid_getInt////////////////////
int MeetAndroid_getInt()
{
	//uint8_t b[bufferCount];
        uint8_t b[100];
	for(int a = 1;a < bufferCount;a++){
		b[a-1] = buffer[a];
	}

	b[bufferCount-1] = '\0';
	return atoi((char*)b);
}

////////////////////MeetAndroid_getLong////////////////////
long MeetAndroid_getLong()
{
//	uint8_t b[bufferCount];
        uint8_t b[100];
	for(int a = 1;a < bufferCount;a++){
		b[a-1] = buffer[a];
	}

	b[bufferCount-1] = '\0';
	return atol((char*)b);
}

////////////////////MeetAndroid_getFloat////////////////////
//float MeetAndroid_getFloat()
//{
//	return (float)getDouble();
//}

////////////////////MeetAndroid_getFloatValues////////////////////
//void MeetAndroid_getFloatValues(float values[])
//{
//	int t = 0; // counter for each char based array
//	int pos = 0;
//
//	int start = 1; // start of first value
//	for (int end=1; end<bufferCount;end++){
//		// find end of value
//		if (buffer[end]==delimiter) {
//			// now we know start and end of a value
//			char b[(end-start)+1]; // create container for one value plus '\0'
//			t = 0;
//			for(int i = start;i < end;i++){
//				b[t++] = (char)buffer[i];
//			}
//			b[t] = '\0';
//			values[pos++] = atof(b);
//			start = end+1;
//		}
//	}
//	// get the last value
//	char b[(bufferCount-start)+1]; // create container for one value plus '\0'
//	t = 0;
//	for(int i = start;i < bufferCount;i++){
//		b[t++] = (char)buffer[i];
//	}
//	b[t] = '\0';
//	values[pos] = atof(b);
//}

// not tested yet
////////////////////MeetAndroid_getDoubleValues////////////////////
//void MeetAndroid_getDoubleValues(float values[])
//{
//	getFloatValues(values);
//}

// not tested yet
////////////////////MeetAndroid_getIntValues////////////////////
//void MeetAndroid_getIntValues(int values[])
//{
//	int t = 0; // counter for each char based array
//	int pos = 0;
//
//	int start = 1; // start of first value
//	for (int end=1; end<bufferCount;end++){
//		// find end of value
//		if (buffer[end]==delimiter) {
//			// now we know start and end of a value
//			char b[(end-start)+1]; // create container for one value plus '\0'
//			t = 0;
//			for(int i = start;i < end;i++){
//				b[t++] = (char)buffer[i];
//			}
//			b[t] = '\0';
//			values[pos++] = atoi(b);
//			start = end+1;
//		}
//	}
//	// get the last value
//	char b[(bufferCount-start)+1]; // create container for one value plus '\0'
//	t = 0;
//	for(int i = start;i < bufferCount;i++){
//		b[t++] = (char)buffer[i];
//	}
//	b[t] = '\0';
//	values[pos] = atoi(b);
//}

////////////////////MeetAndroid_getDouble////////////////////
double MeetAndroid_getDouble()
{
	//char b[bufferCount];
        char b[100];
	for(int a = 1;a < bufferCount;a++){
		b[a-1] = (char)buffer[a];
	}

	b[bufferCount-1] = '\0';
	return atof(b);	
}
