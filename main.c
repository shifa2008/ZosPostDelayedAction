/*
 * main.c
 *
 *  Created on: 2018��7��6��
 *      Author: user
 */
#include  "stdio.h"
#include "Button.h"
#include "ZosPostDelayedAction.h"
ButtonClass_t *ButtonThis=NULL;
void *ZosThis=NULL;
enum{
	SINGNAL_NULL_E,
	TASK_DEMO_E,
	BUTTON_SINGNAL_E,
}SINGNAL_e;
//������ Ϊ����ר�� ��ʾϵͳ���С���ϵͳ�޹�
//��ʱģ��
unsigned int i=1;
int  ActionTask(int arg,char *argv[])
{

    void *Argc=(ZosPostClass_t*)argv[0];
    ZosPostDelayedAction_t *System=(ZosPostDelayedAction_t*)argv[1];

	TaskStart(System);

    while(1)
    {
    if(i==0)i=1;
	i=i*2;
	ZosPostSetInterruptSignalAction(Argc,TASK_DEMO_E,GetSystem(argv)->Action,GetUserThis(argv));//����һ�����ź�
	ZosPostDelayedAction(Argc,i,GetSystem(argv)->Action,GetUserThis(argv));                     //����һ����ʱ�ź�
	TaskSleep(System);                                                                          //����һ���ϵ���ʱ�˳�������
	printf("1hello:%d\r\n",i);

	i=i*2;
	ZosPostSetInterruptSignalAction(Argc,TASK_DEMO_E,GetSystem(argv)->Action,GetUserThis(argv));
	ZosPostDelayedAction(Argc,i,GetSystem(argv)->Action,GetUserThis(argv));
	TaskSleep(System);
	printf("2hello:%d\r\n",i);

	i=i*2;
	ZosPostSetInterruptSignalAction(Argc,TASK_DEMO_E,GetSystem(argv)->Action,GetUserThis(argv));
	ZosPostDelayedAction(Argc,i,GetSystem(argv)->Action,GetUserThis(argv));
	TaskSleep(System);

	//i=i*2;
	//ZosPostDelayedAction(Argc,i,Action,NULL); //��ϵͳ����������ִ��
	//TaskEnd();
    }
	TaskStop(System);

}
//����ɨ��ģ��r
int  ButtonActionTask(int arg,char *argv[])
{
	ButtonClass_t *Button=(	ButtonClass_t *)GetUserThis(argv);
	TaskStart(GetSystem(argv));
    while(1)
    {
     Button->Loop(Button);
     ZosPostDelayedAction(GetThis(argv),10,ButtonActionTask,GetUserThis(argv));
     TaskSleep(GetSystem(argv));
    }
	TaskStop(System);

}
char ButtonGet(struct Button_n *This)
{
	char ch;
	if( kbhit() ) {
	    ch = getch();
	    if(ch=='k')
	    {
	    	return 1;
	    }
	    if(ch=='r')
	    {

	    	ZosPostInterruptSignalAction(ZosThis,10,TASK_DEMO_E);//����һ�� TASK_DEMO_E�ź�
	    }
	}

    return 0;
}

void ButtonUpEvent(struct Button_n *This)
{
    printf("ButtonUpEvent\r\n");
}
void ButtonDownEvent(struct Button_n *This)
{
    printf("ButtonDownEvent\r\n");
}

int main()
{
	 printf("   ��C��ʾ ģ���߳�   \r\n");
	 printf("  ��ʾ���ģ����ʱ���\r\n");
	 printf("r.�簴�»���ʾ�������\r\n");
	 printf("k.�簴�»ᰴ���������\r\n");
	 ButtonThis=CreateButtonClassMallocNew();
	 ButtonThis->ButtonDownEvent=ButtonDownEvent;//�����¼��ص�
	 ButtonThis->ButtonUpEvent=ButtonUpEvent;    //̧���¼��ص�
	 ButtonThis->ButtonGet=ButtonGet;            //����ģ���ȡ
	 ButtonThis->ButtonSetMode(ButtonThis,TriggerChickOne_e);
	 ZosThis=CreateZosPostDelayedActionClassMalloc(200); // ���� ��ʹ�ö�̬�ڴ����
	// ZosThis=CreateZosPostDelayedActionClassUser(ZosThis,uZosBuff,10); //�����ֶ�ָ�����Эͬ����
	 ZosPostDelayedAction(ZosThis,1,ActionTask,NULL);
	 ZosPostDelayedAction(ZosThis,10,ButtonActionTask,ButtonThis);//ģ���ⰴ��

	// ZosPostSetPriorityEvel(ZosThis,ActionTask,66536);//����ִ��ActtionTask
	 while(1)
	 {
	 ZosPostDelayedActionLoop(ZosThis);
	 ZosPostDelayedActionClock(ZosThis);
	 }

}


