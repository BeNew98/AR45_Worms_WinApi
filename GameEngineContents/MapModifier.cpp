#include "MapModifier.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "Map.h"
#include "ContentsEnums.h"

MapModifier* MapModifier::MainModifier = nullptr;

MapModifier::MapModifier() 
{
}

MapModifier::~MapModifier() 
{
}

void MapModifier::Start()
{
	MainModifier = this;

	if (nullptr == MainModifier)
	{
		MsgAssert("MainModifier�� nullptr�Դϴ�.");
		return;
	}
}

void MapModifier::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("LandHole"))
	{
		float4 Pos = GetLevel()->GetMousePosToCamera();
		CreateHole(Pos);
		return;
	}
}


void MapModifier::CreateHole(float4 _Pos, int _Radius)
{
	if (0 >= _Radius)
	{
		MsgAssert("�������� 0���� �۰ų� ���� �� �����ϴ�.");
		return;
	}

	std::string CurMapName = Map::MainMap->GetMapName();

	HDC MapDc = Map::MainMap->GetMapDC();
	HDC ColMapDc = Map::MainMap->GetColMapDC();

	float4 CircleRenderPos = _Pos;

	//Map�� �׸�
	{
		//���� ���� �� ����
		//PS_SOLID: ��
		HPEN MyPen = CreatePen(PS_SOLID, 1, Magenta);
		HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);
		//�ܻ��� ���� ���� �귯���� ����
		HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Magenta);
		//������ DC�� ��ü ����
		HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);
	
		Ellipse(MapDc,
			CircleRenderPos.ix() - _Radius,
			CircleRenderPos.iy() - _Radius,
			CircleRenderPos.ix() + _Radius,
			CircleRenderPos.iy() + _Radius);

		//�ٽ� ���� �귯�� ����
		SelectObject(MapDc, OldBrush);
		//������ �귯�� ����
		DeleteObject(MyBrush);

		SelectObject(MapDc, OldPen);
		DeleteObject(MyPen);
	}

	//ColMap�� �׸�
	{
		HPEN MyPen = CreatePen(PS_SOLID, 1, Magenta);
		HPEN OldPen = (HPEN)SelectObject(ColMapDc, MyPen);
		//�ܻ��� ���� ���� �귯���� ����
		HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Magenta);
		//������ DC�� ��ü ����
		HBRUSH OldBrush = (HBRUSH)SelectObject(ColMapDc, MyBrush);

		Ellipse(ColMapDc,
			CircleRenderPos.ix() - _Radius,
			CircleRenderPos.iy() - _Radius,
			CircleRenderPos.ix() + _Radius,
			CircleRenderPos.iy() + _Radius);

		SelectObject(ColMapDc, OldBrush);
		DeleteObject(MyBrush);

		SelectObject(ColMapDc, OldPen);
		DeleteObject(MyPen);
	}


	//�׽�Ʈ �ڵ�-------���� ����
	{

		HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Red);
		HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);

		int r = 5;

		Ellipse(MapDc,
			CircleRenderPos.ix() - r,
			CircleRenderPos.iy() - r,
			CircleRenderPos.ix() + r,
			CircleRenderPos.iy() + r);

		SelectObject(MapDc, OldBrush);
		DeleteObject(MyBrush);
	}
}