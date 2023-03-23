#include "MouseObject.h"
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnums.h"
MouseObject::MouseObject() 
{
}

MouseObject::~MouseObject() 
{
}

void MouseObject::Start() 
{
//	MouseRender = CreateRender();
	MouseCollision = CreateCollision(WormsCollisionOrder::WeaPonInterFace);

	/*if (nullptr == MouseRender)
	{
		MsgAssert("�������� ��������� �ʾҽ��ϴ�.");
	}*/

	if (nullptr == MouseCollision)
	{
		MsgAssert("�ݸ����� ��������� �ʾҽ��ϴ�.");
	}
}

void MouseObject::Update(float _DeltaTime) 
{
	SetPos(GetLevel()->GetMousePos());
}