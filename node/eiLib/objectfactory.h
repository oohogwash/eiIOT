#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

namespace eiKernel
{


class ObjectFactory
{
public:
    ObjectFactory();
   static void * getObject(const char * id);
};

}

#endif // OBJECTFACTORY_H
