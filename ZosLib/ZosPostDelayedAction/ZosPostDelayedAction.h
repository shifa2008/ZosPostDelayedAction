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
	jmp_buf env;
	//char RunCallFlag;
}ZosPostDelayedActionVar_t;
typedef struct ZosPostDelayedAction_n{
	ZosPostDelayedActionVar_t Var[1];
	void  (*Action)(void *Argc,void *p,void *System);
}ZosPostDelayedAction_t;
typedef struct{
	long Num;
	ZosPostDelayedAction_t *ZosPostBuf;
}ZosPostClass_t;
#define  TaskSleep(Task)    do { if(setjmp(((ZosPostDelayedAction_t*)Task)->Var->env)==0) return ;} while(0);                      // if(((ZosPostDelayedAction_t*)Task)->Var->Started>0)longjmp(((ZosPostDelayedAction_t*)Task)->Var->env,1); ((ZosPostDelayedAction_t*)Task)->Var->Started=1;do { if(setjmp(((ZosPostDelayedAction_t*)Task)->Var->env)==0){((ZosPostDelayedAction_t*)Task)->Var->Started=0;}} while(0);
extern int ZosPostSetInterruptSignalAction(ZosPostClass_t *This,long InterruptSignal,void  (*Action)(void *Argc,void *p,void *System),void *arg);//设置一个信号断点
extern int ZosPostInterruptSignalAction(ZosPostClass_t *This,long timer,long IsrMark); //触发一个信号断点
extern void  ZosPostDelayedActionClock(ZosPostClass_t *This);  //系统时钟
extern int ZosPostDelayedAction(ZosPostClass_t *This,long timer,void  (*Action)(void *Argc,void *p,void *System),void *arg); //启动延时一个任务
extern int ZosPostDelAction(ZosPostClass_t *This,void  (*Action)(void *Argc,void *p,void *System),void *arg);   //删除一个任务
extern void  ZosPostDelayedActionLoop(ZosPostClass_t *This);  //循环
extern ZosPostClass_t *CreateZosPostDelayedActionClassUser(ZosPostClass_t *This,ZosPostDelayedAction_t *uThis,long Num);//手动创建任务最大支持数
extern ZosPostClass_t *CreateZosPostDelayedActionClassMalloc(long Num);  //创建任务最大支持数
#endif /* ZOSPOSTDELAYEDACTION_H_ */
