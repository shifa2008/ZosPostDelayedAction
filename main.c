/*
 * main.c
 *
 *  Created on: 2018��9��9��
 *      Author: user
 */


/*
 * main.c
 *
 *  Created on: 2018��7��6��
 *      Author: user
 */
#include <windows.h>
#include<conio.h>
#include "Button.h"
#include "ZosPostDelayedAction.h"

ButtonClass_t *ButtonThis=NULL;
void *ZosThis=NULL;
//��ʱģ��
unsigned int i=0;
void  ActionTask(void *Argc,void *User,void *System)
{

	TaskStart(System);


    while(1)
    {
    i=100000;

	//��� ʱ�䳤��-2 Ϊ�����źű�־
	ZosPostSetInterruptSignalAction(Argc,-2,ActionTask,User);
	ZosPostDelayedAction(Argc,i,ActionTask,User);
	TaskSleep(System);
	printf("1hello%d\r\n",i);

	//i=i*2;
	ZosPostSetInterruptSignalAction(Argc,-2,ActionTask,User);
	ZosPostDelayedAction(Argc,i,ActionTask,User);
	TaskSleep(System);
	printf("2hello%d\r\n",i);

	//i=i*2;
	ZosPostSetInterruptSignalAction(Argc,-2,ActionTask,User);
	ZosPostDelayedAction(Argc,i,ActionTask,User);
	TaskSleep(System);
	printf("3hello%d\r\n",i);
	ZosPostDelayedAction(Argc,5000,ActionTask,User);
	ZosPostSetInterruptSignalAction(Argc,-2,ActionTask,User);
	TaskSleep(System);
	//i=i*2;
	//ZosPostDelayedAction(Argc,i,Action,NULL); //��ϵͳ����������ִ��
	//TaskEnd();
    }

}
//����ɨ��ģ��r
void  ButtonActionTask(void *Argc,void *User,void *System)
{
	ButtonClass_t *Button=User;
	unsigned int k=0;
	TaskStart(System);
    while(1)
    {
    //	k++;
   //  printf("\r\nk=%d",k);
     Button->Loop(Button);
     ZosPostDelayedAction(Argc,10,ButtonActionTask,User);
     TaskSleep(System);

    }

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

	    	ZosPostInterruptSignalAction(ZosThis,10,-2);
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
//Э��Ӧ�þ���
int main()
{

	 printf("����R ����ActionTask ����\r\n");
	 printf("����K ����ButtonActionTask ����\r\n");

	 ButtonThis=CreateButtonClassMallocNew();
	 ButtonThis->ButtonDownEvent=ButtonDownEvent;//�����¼��ص�
	 ButtonThis->ButtonUpEvent=ButtonUpEvent;    //̧���¼��ص�
	 ButtonThis->ButtonGet=ButtonGet;            //����ģ���ȡ
	 ButtonThis->ButtonSetMode(ButtonThis,TriggerChickOne_e);
	 ZosThis=CreateZosPostDelayedActionClassMalloc(200); // ���� ��ʹ�ö�̬�ڴ����
	// ZosThis=CreateZosPostDelayedActionClassUser(ZosThis,uZosBuff,10); //�����ֶ�ָ�����Эͬ����
	 ZosPostDelayedAction(ZosThis,1,ActionTask,NULL);
	 ZosPostDelayedAction(ZosThis,10,ButtonActionTask,ButtonThis);//ģ���ⰴ��
	 while(1)
	 {

    	 ZosPostDelayedActionLoop(ZosThis);
	     ZosPostDelayedActionClock(ZosThis);//ʵ��Ӧ�� ���Ӧ���ڶ�ʱ����

	 }

}


