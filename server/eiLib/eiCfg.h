
#ifndef EICFG_H_
#define EICFG_H_


typedef enum _iotype
{
  Analogue,
  Digital,

}IOTYPE;

typedef struct _ioPoint
{
    char ID[20];
    char pin[10];
    char type[20];
}IOPOINT;



class eiCfg
{
  IOPOINT points[100];
  
public:
	eiCfg(){}
	void Init(){}
	void Run(){}
};


#endif