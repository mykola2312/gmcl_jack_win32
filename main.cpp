#include <Windows.h>
#include "interface.h"

#define BALL_NAME "luaL_Ball"
#define BALL_TYPE 195

typedef struct {
	int mass;
	int speed;
} ball_t;

int refId;

int TestFunc(CLuaState* L)
{
	L->PushString("Hello World!");
	return 1;
}

int CallBack(CLuaState* L)
{
	L->CheckType(1,FUNCTION);
	L->CheckType(2,NUMBER);
	int n = L->GetNumber(2);

	L->Push(1);
	L->Call(0,n);
	L->Pop(n);
	return 0;
}

int CreateBall(CLuaState* L)
{
	L->CheckType(1,NUMBER);
	L->CheckType(2,NUMBER);
	ball_t* pBall = new ball_t;

	pBall->mass = L->GetNumber(1);
	pBall->speed = L->GetNumber(2);

	userdata_t* pData = L->NewUserData();
	pData->data = pBall;
	pData->type = BALL_TYPE;

	L->GetMetaFromRegistry(BALL_NAME);
	L->SetMetaTable(-2);

	return 1;
}

ball_t* CheckBall(CLuaState* L,int idx)
{
	userdata_t* pData = L->GetUserData(idx);
	if(pData->type == BALL_TYPE)
		return (ball_t*)pData->data;
	else
		return NULL;
}

int ball_tostring(CLuaState* L)
{
	ball_t* pBall = CheckBall(L,1);
	if(!pBall)
		L->PushString("pBall = NULL");
	else
		L->PushFString("[Ball with mass %d and speed %d]",pBall->mass,pBall->speed);
	return 1;
}

int ball_eq(CLuaState *L)
{
	ball_t* pBall1 = CheckBall(L,1),*pBall2 = CheckBall(L,2);
	L->PushBool(((pBall1->mass == pBall2->mass) && (pBall1->speed == pBall2->speed)));
	return 1;
}

int ball_add(CLuaState *L)
{
	ball_t* pBall1 = CheckBall(L,1),*pBall2 = CheckBall(L,2);

	L->PushCFunction(CreateBall);
	L->PushNumber(pBall1->mass + pBall2->mass);
	L->PushNumber(pBall1->speed + pBall2->speed);
	L->Call(2,1);

	return 1;
}

int ball_gc(CLuaState* L)
{
	ball_t* pBall = CheckBall(L,1);
	delete pBall;
	return 0;
}

int ball_increment(CLuaState* L)
{
	ball_t* pBall = CheckBall(L,1);
	pBall->mass++;
	pBall->speed++;
	return 0;
}

GMOD_MODULE_OPEN()
{
	L->CreateMetaTable(BALL_NAME);

	L->PushCFunction(ball_tostring);
	L->SetField(-2,"__tostring");

	L->PushCFunction(ball_eq);
	L->SetField(-2,"__eq");

	L->PushCFunction(ball_gc);
	L->SetField(-2,"__gc");

	L->PushCFunction(ball_add);
	L->SetField(-2,"__add");

	L->PushCFunction(ball_increment);
	L->SetField(-2,"Increment");

	L->Push(-1);
	L->SetField(-2,"__index");

	L->CreateTable();
	
	L->PushCFunction(TestFunc);
	L->SetField(-2,"TestFunc");

	L->PushCFunction(CallBack);
	L->SetField(-2,"CallBack");

	L->PushCFunction(CreateBall);
	L->SetField(-2,"CreateBall");

	L->SetGlobal("jack");

	L->PushString("Hello World!");
	refId = L->ReferenceCreate();
	L->Print("Reference ID: %d",refId);

	L->GetGlobal("print");
	L->ReferencePush(refId);
	L->Call(1,0);
	return 0;
}

GMOD_MODULE_CLOSE()
{
	L->ReferenceFree(refId);

	L->PushNil();
	L->SetGlobal("jack");
	return 0;
}