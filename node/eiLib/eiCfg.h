
#ifndef EICFG_H_
#define EICFG_H_

namespace eiMsg
{

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



class EiCfg
{
  IOPOINT points[100];
  
public:
    EiCfg(){}
	void Init(){}
	void Run(){}
};

} // eiMsg
#endif
