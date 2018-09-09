#include "Button.h"
static long ButtonGetDownTimer(struct Button_n *This)
{
    if(NULL==This)return 0;
    return This->Var->ButtonDownTime;
}
static void Loop(struct Button_n *This)
{
	 if(NULL==This)return ;
    if(This->ButtonGet!=NULL)
	 This->Var->ButtonChick=This->ButtonGet(This);
	 switch(This->Var->ButtonMode)
	 {
			 case 0x00:
				 if(This->Var->ButtonChick>0&&This->Var->ButtonDownTime==0)
				 {
					if(This->ButtonDownEvent!=NULL)
					This->ButtonDownEvent(This);
					 This->Var->ButtonDownTime++;
				 }
				 if(This->Var->ButtonChick==0&&This->Var->ButtonDownTime>0)
				 {
					 if(This->ButtonUpEvent!=NULL)
					This->ButtonUpEvent(This);
					 This->Var->ButtonDownTime=0;
				 }
			 break;
			 case 0x01:
				 if(This->Var->ButtonChick>0)
				{
					if(This->ButtonDownEvent!=NULL)
					This->ButtonDownEvent(This);
					This->Var->ButtonDownTime++;
				 }
				 if(This->Var->ButtonChick==0)
				{
					if(This->ButtonUpEvent!=NULL)
					This->ButtonUpEvent(This);
					This->Var->ButtonDownTime=0;
				}
	         break;
			 default:break;
	 }
}
static void ButtonTimer1ms(struct Button_n *This)
{
	if(NULL==This)return ;
    if(This->Var->ButtonChick>0)
	 {
	    This->Var->ButtonDownTime++;
	 }
}
ButtonClass_t *CreateButtonClass(ButtonClass_t *This)
{
	 if(NULL==This)return NULL;
     This->Loop=Loop;
	 This->ButtonGetDownTimer=ButtonGetDownTimer;
	 This->ButtonTimer1ms=ButtonTimer1ms;
     return This;
}
void ButtonSetMode(struct Button_n *This,ButtonMode_e Mode)
{
	if(NULL==This)return ;
	This->Var->ButtonMode=Mode;
}
ButtonClass_t *CreateButtonClassMallocNew()
{
	 ButtonClass_t *This=malloc(sizeof(ButtonClass_t));
     if(This==NULL)return NULL;
     This->Loop=Loop;
	 This->ButtonGetDownTimer=ButtonGetDownTimer;
	 This->ButtonSetMode=ButtonSetMode;
	 This->ButtonTimer1ms=ButtonTimer1ms;
     return This;
}



