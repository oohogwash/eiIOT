#include "module.h"
#include "modulemanager.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>


int compModuleIndexEntries(const void * a, const void * b)
{
    MODULEINDEXENTRY  * ma = (MODULEINDEXENTRY *)a;
    MODULEINDEXENTRY  * mb = (MODULEINDEXENTRY *)b;
    return strcmp(ma->pID, mb->pID);
}




moduleManager::moduleManager()
{
  strcpy(moduleGroupDesc[0], "Base");
}
void moduleManager::dump()
{
    printf("module manager\nNum modules %d\n", numModules);
    for(int idx =0; idx < numModules;idx++){
        modules[idx]->dump();
    }
}

MODULEINDEXENTRY * moduleManager::findModuleIndex( const char * id)
{
    MODULEINDEXENTRY srch;
    srch.pID = id;
    MODULEINDEXENTRY *result = (MODULEINDEXENTRY * ) bsearch(&srch, moduleIndex, numModules, sizeof(MODULEINDEXENTRY),  compModuleIndexEntries);
    return result;
}

module * moduleManager::findModule(const char * id)
{

   /* MODULEINDEXENTRY srch;
    srch.pID = id;
    MODULEINDEXENTRY *result = (MODULEINDEXENTRY * ) bsearch(&srch, moduleIndex, numModules, sizeof(MODULEINDEXENTRY),  compModuleIndexEntries);
    */
    MODULEINDEXENTRY * result = findModuleIndex(id);
    if(result != 0)
    {
        return result->pModule;
    }
    return 0;
}

int moduleManager::add(module * m, bool manage)
{
    if(findModule(m->id) == 0)
    {
    if(numModules< MAXMODULES)
    {
        modules[numModules] = m;
        moduleIndex[numModules].pID = m->id;
        moduleIndex[numModules].pModule = m;
        moduleIndex[numModules].managed = manage;
        moduleIndex[numModules].index = numModules;
        if(++numModules >1)
        {
            qsort(moduleIndex, numModules, sizeof(MODULEINDEXENTRY ), compModuleIndexEntries);
        }
        return numModules;
    }
    }
    return -1;

}


int moduleManager::remove(module * m)
{
    MODULEINDEXENTRY * result = findModuleIndex(m->id);

    if(result != 0)
    {
        if(result->managed)
        {
            delete result->pModule;
        }
        //remove module
        for(int idx=result->index; idx < numModules; idx++)
        {
            modules[idx] = modules[idx+1];
        }
        numModules--;
        // reindex
        qsort(moduleIndex, numModules, sizeof(MODULEINDEXENTRY ), compModuleIndexEntries);
        return numModules;
    }
    return -1;

}

void moduleManager::init()
{

}
