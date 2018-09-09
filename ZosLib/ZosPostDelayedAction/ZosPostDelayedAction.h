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
extern int ZosPostSetInterruptSignalAction(ZosPostClass_t *This,long InterruptSignal,void  (*Action)(void *Argc,void *p,void *System),void *arg);//����һ���źŶϵ�
extern int ZosPostInterruptSignalAction(ZosPostClass_t *This,long timer,long IsrMark); //����һ���źŶϵ�
extern void  ZosPostDelayedActionClock(ZosPostClass_t *This);  //ϵͳʱ��
extern int ZosPostDelayedAction(ZosPostClass_t *This,long timer,void  (*Action)(void *Argc,void *p,void *System),void *arg); //������ʱһ������
extern int ZosPostDelAction(ZosPostClass_t *This,void  (*Action)(void *Argc,void *p,void *System),void *arg);   //ɾ��һ������
extern void  ZosPostDelayedActionLoop(ZosPostClass_t *This);  //ѭ��
extern ZosPostClass_t *CreateZosPostDelayedActionClassUser(ZosPostClass_t *This,ZosPostDelayedAction_t *uThis,long Num);//�ֶ������������֧����
extern ZosPostClass_t *CreateZosPostDelayedActionClassMalloc(long Num);  //�����������֧����
#endif /* ZOSPOSTDELAYEDACTION_H_ */
