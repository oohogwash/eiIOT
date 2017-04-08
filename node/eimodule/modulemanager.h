#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H


namespace eiModule
{

class Module;

typedef struct _moduleIndexEntry
{
    Module * pModule;
    const char * pID;
    int index; // the index into index entries and modules tables.
    bool managed; // when removing the entry will be deleted
}MODULEINDEXENTRY;

class ModuleManager
{
    static const int MAXMODULES = 128;
    int numModules=0;

    Module * modules[MAXMODULES];
    MODULEINDEXENTRY moduleIndex[MAXMODULES];

    static const int MAXMODULEPRIORITIES = 10;
    static const int MAXMODULEGROUPS = 10;
    static const int GROUPDESCLEN = 32;
    static const int PRIORITYDESCLEN = 32;

    char moduleGroupDesc[MAXMODULEGROUPS+1][GROUPDESCLEN+1];
    char modulePriorityDesc[MAXMODULEPRIORITIES+1][PRIORITYDESCLEN+1];

    enum executionTrigger
    {
        et_undefined,
        et_scan, //loaded into a list of models executed in scan of inputs loaded, exec, outputs emitted
        et_event,
        et_schedule,
        et_call
    };
    MODULEINDEXENTRY * findModuleIndex( const char * id);
public:
    ModuleManager();
    Module * findModule(const char * id);
    int add(Module * Module, bool manage = false);
    int remove(Module * Module);



    void init();
    void dump();
};

} // eiMsgModule

#endif // MODULEMANAGER_H
