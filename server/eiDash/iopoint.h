#ifndef IOPOINT_H
#define IOPOINT_H


typedef enum _iotype
{
    ANALOG,
    DIGITAL
}IOTYPE;



class iopoint
{
    IOTYPE iotype;


public:
    iopoint();
};

class digitalInput:iopoint
{
    digitalInput(){}
};

typedef enum _boundaryActionLevel
{
    balDEBUG,
    balWARNING,
    balERROR,
    balCRITICALERROR
}BACTIONLEVEL;

typedef enum     _boundaryCrossingDir
{
        bcdInc,
        bcdDec
}BCROSSDIR;

typedef enum _boundaryAction
{
    baLogDebug,
    baShowWidget,
    baModule
}BACTION;

class inputBoundary
{
public:
    float boudary;
    BCROSSDIR dir;
    BACTIONLEVEL level;
 //   string actionId;
    inputBoundary(){}
};



class analogInput: iopoint
{
    float currentValue;
public:
    analogInput(){}
};

class arduinoIOpoint: iopoint
{

public:
    arduinoIOpoint(){}
};






#endif // IOPOINT_H
