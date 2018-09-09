#ifndef BUTTON_H
#define BUTTON_H
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
typedef enum {
	TriggerChickOne_e=0,
	TriggerChickAlways_e
}ButtonMode_e;
typedef struct {
      char ButtonChick;
      char ButtonMode;
	  long ButtonDownTime;
}ButtonVar_t;
typedef struct Button_n{
      ButtonVar_t Var[1];
	  char (*ButtonGet)(struct Button_n *This);
	  void (*ButtonUpEvent)(struct Button_n *This);
	  void (*ButtonDownEvent)(struct Button_n *This);
	  void (*Loop)(struct Button_n *This);
	  long (*ButtonGetDownTimer)(struct Button_n *This);
	  void (*ButtonSetMode)(struct Button_n *This,ButtonMode_e Mode);
	  void (*ButtonTimer1ms)(struct Button_n *This);
}ButtonClass_t;

extern ButtonClass_t *CreateButtonClass(ButtonClass_t *This);
extern ButtonClass_t *CreateButtonClassMallocNew();
#endif

