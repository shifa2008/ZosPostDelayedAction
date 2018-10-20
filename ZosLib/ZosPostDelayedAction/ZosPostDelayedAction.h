/*
 * ZosPostDelayedAction.h
 *
 *  Created on: 2018年7月6日
 *      Author: user
 */

#ifndef ZOSPOSTDELAYEDACTION_H_
#define ZOSPOSTDELAYEDACTION_H_
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <setjmp.h>

typedef struct{
	long Timer;  //Timer-1为完成  小于-1 为阻塞
	void *This;
	long RunLine;
	long InterruptSignal; //信号触发，当信号触发时 立即按触发时间执行 0为空禁用全局唯一
	unsigned char Started; //本线程调用标示
	unsigned long PriorityEvel;
	unsigned long PriorityEvelDef;
	//jmp_buf env;
	//char RunCallFlag;
}ZosPostDelayedActionVar_t;
typedef struct ZosPostDelayedAction_n{
	ZosPostDelayedActionVar_t Var[1];
	int  (*Action)(int argc,char *argv[]);
}ZosPostDelayedAction_t;
typedef struct{
	long Num;
	ZosPostDelayedAction_t *ZosPostBuf;
}ZosPostClass_t;


#define SIGN __LINE__
#define TaskStart(Task) switch(((ZosPostDelayedAction_t*)Task)->Var->RunLine){case 0:               //协程开始
#define TaskSleep(Task)  ((ZosPostDelayedAction_t*)Task)->Var->RunLine=SIGN;return SIGN;case SIGN:  //挂起线程等待下一次唤醒
#define TaskStop(Task)  }                                                                           //协程结束
#define TaskEnd(Task) ((ZosPostDelayedAction_t*)Task)->Var->RunLine=0;return;                                                                           //协程结束

#define  GetThis(Argv)    (void *)Argv[0]
#define  GetSystem(Argv)  ((ZosPostDelayedAction_t *)Argv[1])
#define  GetUserThis(Argv) (void *)Argv[2]
#if 0
#define  TaskSleep(Task)    do { if(setjmp(((ZosPostDelayedAction_t*)Task)->Var->env)==0) return 0;} while(0);                      // if(((ZosPostDelayedAction_t*)Task)->Var->Started>0)longjmp(((ZosPostDelayedAction_t*)Task)->Var->env,1); ((ZosPostDelayedAction_t*)Task)->Var->Started=1;do { if(setjmp(((ZosPostDelayedAction_t*)Task)->Var->env)==0){((ZosPostDelayedAction_t*)Task)->Var->Started=0;}} while(0);
#define TaskStop(Task)
#endif
//#define  TaskSleep(Task) do{TaskSleepNoReturn(Task); return ;}while(0);
extern int ZosPostSetInterruptSignalAction(ZosPostClass_t *This,long InterruptSignal,int  (*Action)(int argc,char *argv[]),void *arg);//设置一个信号断点
extern int ZosPostInterruptSignalAction(ZosPostClass_t *This,long timer,long IsrMark); //触发一个信号断点
extern void  ZosPostDelayedActionClock(ZosPostClass_t *This);  //系统时钟
extern int ZosPostDelayedAction(ZosPostClass_t *This,long timer,int  (*Action)(int argc,char *argv[]),void *arg); //启动延时一个任务
extern int ZosPostDelAction(ZosPostClass_t *This,int  (*Action)(int argc,char *argv[]),void *arg);   //删除一个任务
extern void  ZosPostDelayedActionLoop(ZosPostClass_t *This);  //循环
extern ZosPostClass_t *CreateZosPostDelayedActionClassUser(ZosPostClass_t *This,ZosPostDelayedAction_t *uThis,long Num);//手动创建任务最大支持数
extern ZosPostClass_t *CreateZosPostDelayedActionClassMalloc(long Num);  //创建任务最大支持数
extern long ZosPostSetPriorityEvel(ZosPostClass_t *This,int  (*Action)(int argc,char *argv[]),unsigned long PriorityEvelNum);//任务周期性重要度(可以理解为优先等级)
void ZosysInit(char *argv[],void **This,void **User,ZosPostDelayedAction_t **System);
#endif /* ZOSPOSTDELAYEDACTION_H_ */
