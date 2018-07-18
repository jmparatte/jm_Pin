/*
	jm_Pin_LED13.ino demonstrates...
	Copyright (c) 2018 Jean-Marc Paratte.  All right reserved.

	*/

#include <jm_Pin.h>

jm_Pin LED13(13, OUTPUT);

bool state = false;
word time0 = 0;
#define time1 1000

void setup()
{
	time0 = (word)millis();
}

void loop()
{
	LED13.toggle();

	while (((word)millis() - time0) < time1);

	time0 += time1;
}
