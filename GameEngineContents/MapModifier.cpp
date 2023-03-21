#include "MapModifier.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
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
	
}


void MapModifier::CreateHole(float4 _Pos, int _Radius)
{
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
}

void MapModifier::CreateMapModifier()
{
	MainModifier = GetLevel()->CreateActor<MapModifier>(WormsRenderOrder::Map);
	float4 MousePos = GetLevel()->GetMousePosToCamera();
	MainModifier->SetPos(MousePos);															//������ ��ġ�� ���߿� �����ؾ� ��
}

void MapModifier::DrawArc(float4 _Pos, int _Radius)
{
	HDC MapDc = Map::MainMap->GetMapDC();
	float4 CircleRenderPos = _Pos;
	float4 ArcStartPos = GetStartArcPos(_Pos, _Radius);
	float4 ArcEndPos = GetEndArcPos(_Pos, _Radius);
	//Map�� �׸�
	{
		//���� ���� �� ����
		//PS_SOLID: ��
		HPEN MyPen = CreatePen(PS_SOLID, 3, Cyan);
		HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);
		//�ܻ��� ���� ���� �귯���� ����
		HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Magenta);
		//������ DC�� ��ü ����
		HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);

		Arc(MapDc,
			CircleRenderPos.ix() - _Radius,
			CircleRenderPos.iy() - _Radius,
			CircleRenderPos.ix() + _Radius,
			CircleRenderPos.iy() + _Radius,
			ArcStartPos.ix(),
			ArcStartPos.iy(),
			ArcEndPos.ix(),
			ArcEndPos.iy()
		);

		//�ٽ� ���� �귯�� ����
		SelectObject(MapDc, OldBrush);
		//������ �귯�� ����
		DeleteObject(MyBrush);

		SelectObject(MapDc, OldPen);
		DeleteObject(MyPen);
	}
}

float4 MapModifier::GetStartArcPos(float4 _Pos, int _Radius)
{
	std::string ColMapName = Map::MainMap->GetColMapName();
	GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);
	float4 NextPos = _Pos;							//CenterPos
	int RadForX = _Radius;							//X ��ǥ�� �˻� ����
	int RadForY = _Radius;							//Y ��ǥ�� �˻� ����
	bool IsBoundary = false;						//�ȼ� �浹�� �Ͼ�� �ݺ����� ���� ������ ���� ��
	
	for (int i = 0; i <= RadForX; i++)
	{
		for (int i = 0; i <= RadForY; i++)
		{
			if (Blue != ColImage->GetPixelColor(NextPos, Magenta))
			{
				++NextPos.y;						//���� �ȼ� �浹���� y���� �������� �� ĭ �Ʒ��� ���� �˻��ϵ��� ��
			}
			else
			{
				IsBoundary = true;
				break;
			}
		}

		if (true == IsBoundary)
		{
			break;
		}
		else
		{
			NextPos.y -= RadForY;					//y���� ó�� �����ߴ� ���� ���� ��ġ�� �ø�
			--RadForY;
			++NextPos.x;
		}
	}

	return NextPos;
}

float4 MapModifier::GetEndArcPos(float4 _Pos, int _Radius)
{
	std::string ColMapName = Map::MainMap->GetColMapName();
	GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);
	float4 NextPos = _Pos;						//CenterPos
	int RadForX = _Radius;
	int RadForY = _Radius;
	bool IsBoundary = false;

	for (int i = 0; i <= RadForX; i++)
	{
		for (int i = 0; i <= RadForY; i++)
		{
			if (Blue != ColImage->GetPixelColor(NextPos, Magenta))
			{
				--NextPos.y;						//���� �ȼ� �浹���� y���� ���ҽ��� �� ĭ ���� ���� �˻��ϵ��� ��
			}
			else
			{
				IsBoundary = true;
				break;
			}
		}

		if (true == IsBoundary)
		{
			break;
		}
		else
		{
			NextPos.y += RadForY;					//y���� ó�� �����ߴ� ���� ���� ��ġ�� ����
			--RadForY;
			++NextPos.x;
		}
	}
	
	return NextPos;
}