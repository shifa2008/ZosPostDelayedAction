/*
 * main.c
 *
 *  Created on: 2018年9月9日
 *      Author: user
 */


/*
 * main.c
 *
 *  Created on: 2018年7月6日
 *      Author: user
 */
#include <windows.h>
#include<conio.h>
#include "Button.h"
#include "ZosPostDelayedAction.h"

ButtonClass_t *ButtonThis=NULL;
void *ZosThis=NULL;
//延时模拟
unsigned int i=0;
void  ActionTask(void *Argc,void *User,void *System)
{

	TaskStart(System);


    while(1)
    {
    i=100000;

	//如果 时间长度-2 为阻塞信号标志
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
	//ZosPostDelayedAction(Argc,i,Action,NULL); //让系统锁定在这里执行
	//TaskEnd();
    }

}
//按键扫描模拟r
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
//协程应用举例
int main()
{

	 printf("按下R 运行ActionTask 例子\r\n");
	 printf("按下K 运行ButtonActionTask 例子\r\n");

	 ButtonThis=CreateButtonClassMallocNew();
	 ButtonThis->ButtonDownEvent=ButtonDownEvent;//按下事件回调
	 ButtonThis->ButtonUpEvent=ButtonUpEvent;    //抬起事件回调
	 ButtonThis->ButtonGet=ButtonGet;            //输入模拟读取
	 ButtonThis->ButtonSetMode(ButtonThis,TriggerChickOne_e);
	 ZosThis=CreateZosPostDelayedActionClassMalloc(200); // 可能 以使用动态内存分配
	// ZosThis=CreateZosPostDelayedActionClassUser(ZosThis,uZosBuff,10); //可以手动指定最大协同数量
	 ZosPostDelayedAction(ZosThis,1,ActionTask,NULL);
	 ZosPostDelayedAction(ZosThis,10,ButtonActionTask,ButtonThis);//模拟检测按键
	 while(1)
	 {

    	 ZosPostDelayedActionLoop(ZosThis);
	     ZosPostDelayedActionClock(ZosThis);//实际应用 这个应放在定时器中

	 }

}


