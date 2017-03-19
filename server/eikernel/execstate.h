#ifndef EXECSTATE_H
#define EXECSTATE_H
#include <memory.h>

#ifndef null
#define null  0
#endif





typedef enum _packMLAllstates
{
    unholding,
    held,
    holding,
    idle,
    starting,
    executing, // execute
    completing,
    complete,
    resetting,
    unsuspending,
    suspended,
    suspending,
    stopped,
    stopping,
    clearing,
    aborted,
    aborting
}PACKMLALLSTATES;

typedef enum _packMLstates
{
    pmlstate_held,
    pmlstate_idle,
    pmlstate_execute,
    pmlstate_complete,
    pmlstate_suspended,
    pmlstate_stopped,
    pmlstate_aborted,
}PACKMLSTATES;

typedef enum _packMLCommands
{
    pmlcmd_unhold,
    pmlcmd_hold,
    pmlcmd_start,
    pmlcmd_suspend,
    pmlcmd_unsuspend,
    pmlcmd_reset,
    pmlcmd_stop,
    pmlcmd_clear,
    pmlcmd_abort
}PACKMLCOMMANDS;

typedef struct _graphEdge
{
    int id;
    int vertexFrom, vertexTo;
}GRAPHEDGE;



class statemachine
{
    static const int MAXVERTICES = 128;
    static const int MAXEDGES = 128;
    int vertices[MAXVERTICES+1];
    GRAPHEDGE edges[MAXEDGES+1];
    char * statedesc;
    char * transitiondesc;
public:
    statemachine();
    void init(int * states, int numStates, int * transitions, int numTransitions, char * stateDesc = null, char * transitionsDesc = null);
    int transition();
    void dump();
    void registerListener();
    void unregisterListener();


};


/*
typedef enum _packMLTransitionstates
{
    undefined,
    unholding,
    held,
    holding,
    idle,
    starting,
    executing, // execute
    completing,
    complete,
    resetting,
    unsuspending,
    suspended,
    suspending,
    stopped,
    stopping,
    clearing,
    aborted,
    aborting



};
*/

class execstate
{
public:
    execstate();
};

#endif // EXECSTATE_H
