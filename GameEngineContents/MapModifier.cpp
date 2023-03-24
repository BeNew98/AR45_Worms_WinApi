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
        return;
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
		//PS_SOLID: �Ǽ�
		HPEN MyPen = CreatePen(PS_SOLID, 1, Magenta);
		HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);

		HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Magenta);                     //�ܻ��� ���� ���� �귯���� ����
		HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);                 //������ DC�� ��ü ����
	
		Ellipse(MapDc,
			CircleRenderPos.ix() - _Radius,
			CircleRenderPos.iy() - _Radius,
			CircleRenderPos.ix() + _Radius,
			CircleRenderPos.iy() + _Radius);

		
		SelectObject(MapDc, OldBrush);                                          //�ٽ� ���� �귯�� ����
		
		DeleteObject(MyBrush);                                                  //������ �귯�� ����

		SelectObject(MapDc, OldPen);
		DeleteObject(MyPen);
	}

	//ColMap�� �׸�
	{
		HPEN MyPen = CreatePen(PS_SOLID, 1, Magenta);
		HPEN OldPen = (HPEN)SelectObject(ColMapDc, MyPen);

		HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Magenta);                     //�ܻ��� ���� ���� �귯���� ����
		HBRUSH OldBrush = (HBRUSH)SelectObject(ColMapDc, MyBrush);              //������ DC�� ��ü ����

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

    //�浹�ʰ� �´��� �κ��� �ȼ��� ���� �ִ� �Լ�
	DrawPixel(_Pos, _Radius);
}

void MapModifier::CreateMapModifier()
{
	MainModifier = GetLevel()->CreateActor<MapModifier>(WormsRenderOrder::Map);
	float4 MousePos = GetLevel()->GetMousePosToCamera();
	MainModifier->SetPos(MousePos);
}


void MapModifier::DrawPixel(float4 _Pos, int _Radius)
{
	HDC MapDc = Map::MainMap->GetMapDC();

	std::string ColMapName = Map::MainMap->GetColMapName();
	GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);

	int lineThick = 2;
    float Angle = 0.0f;

	float4 CenterPos = _Pos;						                                        //üũ ������: ���� ����

	for (; Angle < 370; ++Angle)
	{
		float4 CheckPos = { 0.0f, -_Radius };		                                        //�������� ��������ŭ �ö� ��, ������ ���ϱ� �� ���� ������ �ʱ�ȭ
		CheckPos.RotaitonZDeg(Angle);				                                        //ȸ����ȯ ����
		CheckPos += CenterPos;						                                        //ȸ����Ų �� ��ġ ��ȭ


		//�浹���� �Ķ��� �κа� ���̴� �κ��� �� �ѷ��� ������ ��ĥ
        DWORD color = ColImage->GetPixelColor(CheckPos, Magenta);

        //������ 262 ~279��->Magenta�� Ȯ��
        int R = GetRValue(color);
        int G = GetGValue(color);
        int B = GetBValue(color);

        int a = 10;

		if (Blue == ColImage->GetPixelColor(CheckPos, Magenta))
		{
			//Map�� �׸�
			{
				//���� ���� �� ����
				//PS_SOLID: �Ǽ�
				HPEN MyPen = CreatePen(PS_SOLID, lineThick, LineColor);
				HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);

				HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(LineColor);                       //�ܻ��� ���� ���� �귯���� ����
				HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);                     //������ DC�� ��ü ����

				Ellipse(MapDc,
					CheckPos.ix() - lineThick,
					CheckPos.iy() - lineThick,
					CheckPos.ix() + lineThick,
					CheckPos.iy() + lineThick);

				
				SelectObject(MapDc, OldBrush);                                              //�ٽ� ���� �귯�� ����
				DeleteObject(MyBrush);                                                      //������ �귯�� ����

				SelectObject(MapDc, OldPen);
				DeleteObject(MyPen);
			}		
		}
	}
}


//�浹���� Magenta �κп� ���м��� ���� ���, ����� ���� �Լ�
//void MapModifier::CleanLineColor()
//{
//    HDC MapDc = Map::MainMap->GetMapDC();
//
//    std::string MapName = Map::MainMap->GetMapName();
//    GameEngineImage* Image = GameEngineResources::GetInst().ImageFind(MapName);
//
//    std::string ColMapName = Map::MainMap->GetColMapName();
//    GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);
//
//    float4 CheckPos = float4::Zero;
//
//    for (; CheckPos.y < Scale.y; ++CheckPos.y)
//    {
//        for (; CheckPos.x < Scale.x; ++CheckPos.x)
//        {
//            if (LineColor == Image->GetPixelColor(CheckPos, Magenta))
//            {
//                if (Blue != ColImage->GetPixelColor(CheckPos, Magenta))
//                {
//                    //SetPixel(MapDc, CheckPos.x, CheckPos.y, Magenta);
//                    //Map�� �׸�
//                    {
//                        int lineThick = 2;
//                        //���� ���� �� ����
//                        //PS_SOLID: �Ǽ�
//                        HPEN MyPen = CreatePen(PS_SOLID, 1, Magenta);
//                        HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);
//
//                        HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Magenta);                       //�ܻ��� ���� ���� �귯���� ����
//                        HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);                     //������ DC�� ��ü ����
//
//                        Ellipse(MapDc,
//                            CheckPos.ix() - lineThick,
//                            CheckPos.iy() - lineThick,
//                            CheckPos.ix() + lineThick,
//                            CheckPos.iy() + lineThick);
//
//
//                        SelectObject(MapDc, OldBrush);                                              //�ٽ� ���� �귯�� ����
//                        DeleteObject(MyBrush);                                                      //������ �귯�� ����
//
//                        SelectObject(MapDc, OldPen);
//                        DeleteObject(MyPen);
//                    }
//                }
//            }
//        }
//    }
//
//}