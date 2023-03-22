#include "MapModifier.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCollision.h>
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

	ModifierCollision = CreateCollision(static_cast<int>(WormsCollisionOrder::MapModifier));
	ModifierCollision->SetDebugRenderType(CT_CirCle);
}

void MapModifier::Update(float _DeltaTime)
{
	
}


void MapModifier::CreateHole(float4 _Pos, int _Radius)
{

	ModifierCollision->SetPosition(_Pos);

	if (this == nullptr)
	{
		MsgAssert("ModifierActor�� nullptr �Դϴ�.");
	}

	if (0 >= _Radius)
	{
		MsgAssert("�������� 0���� �۰ų� ���� �� �����ϴ�.");
		return;
	}

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

	DrawPixel(_Pos, _Radius);
}

void MapModifier::CreateMapModifier()
{
	MainModifier = GetLevel()->CreateActor<MapModifier>(WormsRenderOrder::Map);
	float4 MousePos = GetLevel()->GetMousePosToCamera();
	MainModifier->SetPos(MousePos);															//������ ��ġ�� ���߿� �����ؾ� ��
}


void MapModifier::DrawPixel(float4 _Pos, int _Radius)
{
	HDC MapDc = Map::MainMap->GetMapDC();

	std::string ColMapName = Map::MainMap->GetColMapName();
	GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);

	int lineThick = 2;
	float Angle = 0.0f;

	float4 CenterPos = _Pos;						//üũ ������: ���� ����

	for (; Angle <= 365; ++Angle)
	{
		float4 CheckPos = { 0.0f, -_Radius };		//�������� ��������ŭ �ö� ��, ������ ���ϱ� �� ���� ������ �ʱ�ȭ
		CheckPos.RotaitonZDeg(Angle);				//ȸ����ȯ ����
		CheckPos += CenterPos;						//ȸ����Ų �� ��ġ ��ȭ

		//�浹���� �Ķ��� �κа� ���̴� �κ��� �� �ѷ��� ������ ��ĥ
		if (Blue == ColImage->GetPixelColor(CheckPos, Magenta))
		{
			////Map�� �׸�
			{
				//���� ���� �� ����
				//PS_SOLID: ��
				HPEN MyPen = CreatePen(PS_SOLID, 3, LineColor);
				HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);
				//�ܻ��� ���� ���� �귯���� ����
				HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(LineColor);
				//������ DC�� ��ü ����
				HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);

				Ellipse(MapDc,
					CheckPos.ix() - lineThick,
					CheckPos.iy() - lineThick,
					CheckPos.ix() + lineThick,
					CheckPos.iy() + lineThick);

				//�ٽ� ���� �귯�� ����
				SelectObject(MapDc, OldBrush);
				//������ �귯�� ����
				DeleteObject(MyBrush);

				SelectObject(MapDc, OldPen);
				DeleteObject(MyPen);
			}		
		}
	}
}