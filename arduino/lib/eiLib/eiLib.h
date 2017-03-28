#ifndef _EILIB_H
#define _EILIB_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
//#ifdef defined(ARDUINO)
//#include "WProgram.h"
//#endif
#endif

#include "eiCfg.h"

#include "eiDisplay.h"

namespace stdint{
	class x
	{

	};
}


#include <inttypes.h>

#include <stdint.h>

//#include <string>

#ifndef eiArduino_h
#define eiArduino_h

#ifndef ARDUINO
#include<iostream>
using namespace std;
#include <stdio.h>
#endif



class Einfo
{
	//eiCom  com;
    eiDisplay display;
public:
	Einfo(){}
        void Init();
        void Run();
        void Dump(){ fprintf(stdout, "Einfo dump");}// cout <<"Einfo dump" <<endl;}
};

#endif
#endif // _EILIB_H
