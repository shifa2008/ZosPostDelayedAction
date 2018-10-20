应用场景：

               在嵌入式工作中 ，大的项目通常是多人配合完成。每个人代码风格，命名习惯，工作思路 都可能不一样。那代码在整个合过程中或后期维护都 是一件很头痛的事情为此我整理了这个开源库，希望能对有这方面困扰的朋友有一定帮助。

 在项目开发过程中，经常遇到以下情况：

 1、某个方法在操作某个硬件时，需要等待一段时间硬件才响应。

 2、某个方法在操作某个硬件时，不知道硬件什么时候才响应。

 3、在项目整合过程中，发现同事某些无用功能功，因为不了解同事代码逻辑和风格无法去除。

 4、得到同事一大堆结口函数，使用时无从下手。

为解决这些问题下面就来看一下我认为可行的办法：

先来看一段代码：

 ButtonClass_t *ButtonThis=NULL;

 ButtonThis=CreateButtonClassMallocNew();
 ButtonThis->ButtonDownEvent=ButtonDownEvent;//按下事件回调
 ButtonThis->ButtonUpEvent=ButtonUpEvent;    //抬起事件回调
 ButtonThis->ButtonGet=ButtonGet;            //输入模拟读取
 ButtonThis->ButtonSetMode(ButtonThis,TriggerChickOne_e);//按下一次触发一次

用高级语言的人一个 这个是不是很面熟。没错 这特别像C++中的类，但是这个并不是类，而是C的结构体封装。很多人会觉得奇怪，为什么用C这样子来写代码，你不如直接用C++好了，C和C++这个问题我这里不作讨论。

这样子使用有什么好处能解决什么问题：

      这样子书写，可以很好的解决代码整合过程中新功能的增加和删除。

      为什么这么说呢：1、删除无用功能（假如整合过程中ButtonThis 这个按钮没有作用，只需要将ButtonThis出现过的地方删除                                           即可，无需关心其它代码逻辑）

                                   2、增加功能（如果项目中要用到按钮这个功能 ，只需要创建一个按钮指针并给他对应的回调即可）

                                   3、这样子书写 留给用户的所有接口全在这一个ButtonThis结构体指针里，不用整个工程寻找各个接口。

至此3和4 问题基本得到解决。

接下来我们解决1和2这就要用到我标题提到的ZosPost了 ：

同样先来看一段代码

void *ZosThis=NULL;

ButtonClass_t *ButtonThis=NULL;

//按键扫描模拟
int  ButtonActionTask(int arg,char *argv[])
{
    ButtonClass_t *Button=(    ButtonClass_t *)GetUserThis(argv);
    TaskStart(GetSystem(argv));
    while(1)
    {
     Button->Loop(Button);
     ZosPostDelayedAction(GetThis(argv),10,ButtonActionTask,GetUserThis(argv));
     TaskSleep(GetSystem(argv));
    }
    TaskStop(System);

}

//延时模拟
unsigned int i=1;
int  ActionTask(int arg,char *argv[])
{

    void Argc=(ZosPostClass_t)argv[0];
    ZosPostDelayedAction_t System=(ZosPostDelayedAction_t)argv[1];

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

int main()
{
     printf("   纯C演示 模拟线程   \r\n");
     ButtonThis=CreateButtonClassMallocNew();
     ButtonThis->ButtonDownEvent=ButtonDownEvent;//按下事件回调
     ButtonThis->ButtonUpEvent=ButtonUpEvent;    //抬起事件回调
     ButtonThis->ButtonGet=ButtonGet;            //输入模拟读取
     ButtonThis->ButtonSetMode(ButtonThis,TriggerChickOne_e);
     ZosThis=CreateZosPostDelayedActionClassMalloc(200); // 可能 以使用动态内存分配
    // ZosThis=CreateZosPostDelayedActionClassUser(ZosThis,uZosBuff,10); //可以手动指定最大协同数量
     ZosPostDelayedAction(ZosThis,1,ActionTask,NULL);
     ZosPostDelayedAction(ZosThis,10,ButtonActionTask,ButtonThis);//模拟检测按键

    // ZosPostSetPriorityEvel(ZosThis,ActionTask,66536);//放慢执行ActtionTask们（可以理解为优先级）
     while(1)
     {
     ZosPostDelayedActionLoop(ZosThis);
     ZosPostDelayedActionClock(ZosThis);
     }

}

看到这么多代码 是否有点蒙了 ，但仔细看 你会发现这玩意是否特别像操作系统中的线程？

比如： ButtonActionTask这个函数还特别奇怪里面居然有一个while(1)  这不是死循环了么？？呵呵 这个问题以后解释

先说上面这段代码是怎么解决 前面提到的1 和2 的问题的。

前面说到这玩意像一个线程，没错 甚至可以理解为它就是一个线程（其实不是线程只是写法不一样而已）上面这段代码实现了 在检测按键（ButtonActionTask）的同时也执行了ActionTask中的逻辑处理，在ActionTask 中我们用到了 延时等待 和信号等待完美解决了

 1、某个方法在操作某个硬件时，需要等待一段时间硬件才响应。

 2、某个方法在操作某个硬件时，不知道硬件什么时候才响应。
这2个问题