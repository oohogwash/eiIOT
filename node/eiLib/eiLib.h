#ifndef _EILIB_H
#define _EILIB_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
//#ifdef defined(ARDUINO)
//#include "WProgram.h"
//#endif
#endif
#ifndef eiArduino_h
#define eiArduino_h

#ifndef ARDUINO
#include<iostream>
using namespace std;
#include <stdio.h>
#endif


#include "eiCfg.h"
#include "eiCom.h"
#include "eiDisplay.h"

#include <inttypes.h>

#include <stdint.h>


namespace eiMsg
{


} // eiMsg

#endif

#endif // _EILIB_H
