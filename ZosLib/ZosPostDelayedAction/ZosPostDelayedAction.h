/*
 * ZosPostDelayedAction.h
 *
 *  Created on: 2018��7��6��
 *      Author: user
 */

#ifndef ZOSPOSTDELAYEDACTION_H_
#define ZOSPOSTDELAYEDACTION_H_
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <setjmp.h>

typedef struct{
	long Timer;  //Timer-1Ϊ���  С��-1 Ϊ����
	void *This;
	long RunLine;
	long InterruptSignal; //�źŴ��������źŴ���ʱ ����������ʱ��ִ�� 0Ϊ�ս���ȫ��Ψһ
	unsigned char Started; //���̵߳��ñ�ʾ
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
#define TaskStart(Task) switch(((ZosPostDelayedAction_t*)Task)->Var->RunLine){case 0:               //Э�̿�ʼ
#define TaskSleep(Task)  ((ZosPostDelayedAction_t*)Task)->Var->RunLine=SIGN;return SIGN;case SIGN:  //�����̵߳ȴ���һ�λ���
#define TaskStop(Task)  }                                                                           //Э�̽���
#define TaskEnd(Task) ((ZosPostDelayedAction_t*)Task)->Var->RunLine=0;return;                                                                           //Э�̽���

#define  GetThis(Argv)    (void *)Argv[0]
#define  GetSystem(Argv)  ((ZosPostDelayedAction_t *)Argv[1])
#define  GetUserThis(Argv) (void *)Argv[2]
#if 0
#define  TaskSleep(Task)    do { if(setjmp(((ZosPostDelayedAction_t*)Task)->Var->env)==0) return 0;} while(0);                      // if(((ZosPostDelayedAction_t*)Task)->Var->Started>0)longjmp(((ZosPostDelayedAction_t*)Task)->Var->env,1); ((ZosPostDelayedAction_t*)Task)->Var->Started=1;do { if(setjmp(((ZosPostDelayedAction_t*)Task)->Var->env)==0){((ZosPostDelayedAction_t*)Task)->Var->Started=0;}} while(0);
#define TaskStop(Task)
#endif
//#define  TaskSleep(Task) do{TaskSleepNoReturn(Task); return ;}while(0);
extern int ZosPostSetInterruptSignalAction(ZosPostClass_t *This,long InterruptSignal,int  (*Action)(int argc,char *argv[]),void *arg);//����һ���źŶϵ�
extern int ZosPostInterruptSignalAction(ZosPostClass_t *This,long timer,long IsrMark); //����һ���źŶϵ�
extern void  ZosPostDelayedActionClock(ZosPostClass_t *This);  //ϵͳʱ��
extern int ZosPostDelayedAction(ZosPostClass_t *This,long timer,int  (*Action)(int argc,char *argv[]),void *arg); //������ʱһ������
extern int ZosPostDelAction(ZosPostClass_t *This,int  (*Action)(int argc,char *argv[]),void *arg);   //ɾ��һ������
extern void  ZosPostDelayedActionLoop(ZosPostClass_t *This);  //ѭ��
extern ZosPostClass_t *CreateZosPostDelayedActionClassUser(ZosPostClass_t *This,ZosPostDelayedAction_t *uThis,long Num);//�ֶ������������֧����
extern ZosPostClass_t *CreateZosPostDelayedActionClassMalloc(long Num);  //�����������֧����
extern long ZosPostSetPriorityEvel(ZosPostClass_t *This,int  (*Action)(int argc,char *argv[]),unsigned long PriorityEvelNum);//������������Ҫ��(�������Ϊ���ȵȼ�)
void ZosysInit(char *argv[],void **This,void **User,ZosPostDelayedAction_t **System);
#endif /* ZOSPOSTDELAYEDACTION_H_ */
