#include "ProcessCommander.h"

void main(void) {
    runProcessCommander();

    /*int i, fPid, ret;
    for(i=1;i<11;i++)
    {
        fPid = fork();
        if(!fPid)
        {
            printf("Sou o filho número %i\n",i);
            exit(i+10);
        }
        printf("Pai: lancei o filho %i com o PID %i\n",i,fPid);
    }

    while((fPid=wait(&ret))!= -1)
    {
        ret=WEXITSTATUS(ret);
        printf("O filho com PID %i devolveu-me %i\n",fPid,ret);
    }*/
}
