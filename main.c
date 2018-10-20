/*
 * main.c
 *
 *  Created on: 2018年7月6日
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
//本例程 为测试专用 演示系统运行。与系统无关
//延时模拟
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
	ZosPostSetInterruptSignalAction(Argc,TASK_DEMO_E,GetSystem(argv)->Action,GetUserThis(argv));//放下一个断信号
	ZosPostDelayedAction(Argc,i,GetSystem(argv)->Action,GetUserThis(argv));                     //启动一个延时信号
	TaskSleep(System);                                                                          //放下一个断点暂时退出本函数
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
	//ZosPostDelayedAction(Argc,i,Action,NULL); //让系统锁定在这里执行
	//TaskEnd();
    }
	TaskStop(System);

}
//按键扫描模拟r
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

	    	ZosPostInterruptSignalAction(ZosThis,10,TASK_DEMO_E);//触发一次 TASK_DEMO_E信号
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
	 printf("   纯C演示 模拟线程   \r\n");
	 printf("  演示如何模拟延时输出\r\n");
	 printf("r.如按下会演示立即输出\r\n");
	 printf("k.如按下会按键立即输出\r\n");
	 ButtonThis=CreateButtonClassMallocNew();
	 ButtonThis->ButtonDownEvent=ButtonDownEvent;//按下事件回调
	 ButtonThis->ButtonUpEvent=ButtonUpEvent;    //抬起事件回调
	 ButtonThis->ButtonGet=ButtonGet;            //输入模拟读取
	 ButtonThis->ButtonSetMode(ButtonThis,TriggerChickOne_e);
	 ZosThis=CreateZosPostDelayedActionClassMalloc(200); // 可能 以使用动态内存分配
	// ZosThis=CreateZosPostDelayedActionClassUser(ZosThis,uZosBuff,10); //可以手动指定最大协同数量
	 ZosPostDelayedAction(ZosThis,1,ActionTask,NULL);
	 ZosPostDelayedAction(ZosThis,10,ButtonActionTask,ButtonThis);//模拟检测按键

	// ZosPostSetPriorityEvel(ZosThis,ActionTask,66536);//放慢执行ActtionTask
	 while(1)
	 {
	 ZosPostDelayedActionLoop(ZosThis);
	 ZosPostDelayedActionClock(ZosThis);
	 }

}


