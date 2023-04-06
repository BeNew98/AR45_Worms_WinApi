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


void MapModifier::SetModifierColScale(float4 _Scale)
{
    ModifierCollision->SetScale(_Scale);
}

void MapModifier::SetModifierColPosition(float4 _Pos)
{
     ModifierCollision->SetPosition(_Pos);
}

void MapModifier::CreateHole(float4 _Pos, int _Diameter, bool _DrawLine)
{
    int Radius = _Diameter / 2;

	if (this == nullptr)
	{
		MsgAssert("ModifierActor�� nullptr �Դϴ�.");
        return;
	}

	if (0 >= Radius)
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
			CircleRenderPos.ix() - Radius,
			CircleRenderPos.iy() - Radius,
			CircleRenderPos.ix() + Radius,
			CircleRenderPos.iy() + Radius);

		
		SelectObject(MapDc, OldBrush);                                          //�ٽ� ���� �귯�� ����
		
		DeleteObject(MyBrush);                                                  //������ �귯�� ����

		SelectObject(MapDc, OldPen);
		DeleteObject(MyPen);
	}

    if (true == _DrawLine)
    {
        //�浹�ʰ� �´��� �κ��� �ȼ��� ���� �ִ� �Լ�
	    DrawPixel(_Pos, Radius);
    }

	//ColMap�� �׸�
	{
		HPEN MyPen = CreatePen(PS_SOLID, 1, Magenta);
		HPEN OldPen = (HPEN)SelectObject(ColMapDc, MyPen);

		HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Magenta);                     //�ܻ��� ���� ���� �귯���� ����
		HBRUSH OldBrush = (HBRUSH)SelectObject(ColMapDc, MyBrush);              //������ DC�� ��ü ����

		Ellipse(ColMapDc,
			CircleRenderPos.ix() - Radius,
			CircleRenderPos.iy() - Radius,
			CircleRenderPos.ix() + Radius,
			CircleRenderPos.iy() + Radius);

		SelectObject(ColMapDc, OldBrush);
		DeleteObject(MyBrush);

		SelectObject(ColMapDc, OldPen);
		DeleteObject(MyPen);
	}
}

void MapModifier::CreateRect(float4 _Pos, int _Horz, int _Vert, bool _DrawLine)
{

    int HalfHorz = _Horz / 2;
    int HalfVert = _Vert / 2;

    if (this == nullptr)
    {
        MsgAssert("ModifierActor�� nullptr �Դϴ�.");
        return;
    }

    if (0 >= HalfHorz)
    {
        MsgAssert("�簢�� ���� ���̰� 0���� �۰ų� ���� �� �����ϴ�.");
        return;
    }

    if (0 >= HalfVert)
    {
        MsgAssert("�簢�� ���� ���̰� 0���� �۰ų� ���� �� �����ϴ�.");
        return;
    }

    HDC MapDc = Map::MainMap->GetMapDC();
    HDC ColMapDc = Map::MainMap->GetColMapDC();

    float4 RectRenderPos = _Pos;

    //Map�� �׸�
    {
        //���� ���� �� ����
        //PS_SOLID: �Ǽ�              || �β�  || ����
        HPEN MyPen = CreatePen(PS_SOLID, 1, Magenta);
        HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);

        HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Magenta);                     //�ܻ��� ���� ���� �귯���� ����
        HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);                 //������ DC�� ��ü ����

        Rectangle(MapDc,
            RectRenderPos.ix() - HalfHorz,                                         //Left
            RectRenderPos.iy() - HalfVert,                                         //Top
            RectRenderPos.ix() + HalfHorz,
            RectRenderPos.iy() + HalfVert);

        SelectObject(MapDc, OldPen);
        DeleteObject(MyPen);

        SelectObject(MapDc, OldBrush);                                          //�ٽ� ���� �귯�� ����
        DeleteObject(MyBrush);                                                  //������ �귯�� ����

    }

    if (true == _DrawLine)
    {
        DrawPixelRect(_Pos, HalfHorz, HalfVert);
    }

    //ColMap�� �׸�
    {
        HPEN MyPen = CreatePen(PS_SOLID, 1, Magenta);
        HPEN OldPen = (HPEN)SelectObject(ColMapDc, MyPen);

        HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Magenta);                     //�ܻ��� ���� ���� �귯���� ����
        HBRUSH OldBrush = (HBRUSH)SelectObject(ColMapDc, MyBrush);              //������ DC�� ��ü ����

        Rectangle(ColMapDc,
            RectRenderPos.ix() - HalfHorz,
            RectRenderPos.iy() - HalfVert,
            RectRenderPos.ix() + HalfHorz,
            RectRenderPos.iy() + HalfVert);

        SelectObject(ColMapDc, OldBrush);
        DeleteObject(MyBrush);

        SelectObject(ColMapDc, OldPen);
        DeleteObject(MyPen);
    }
}

void MapModifier::DrawPixel(float4 _Pos, int _Radius)
{
	HDC MapDc = Map::MainMap->GetMapDC();

	std::string ColMapName = Map::MainMap->GetColMapName();
	GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);

    if (nullptr == ColImage)
    {
        MsgAssert("�浹�˻縦 ���� ColImage�� nullptr�Դϴ�.");
        return;
    }

	int lineThick = 2;
    float Radius = static_cast<float>(_Radius);
    float Angle = 0.0f;

	float4 CenterPos = _Pos;						                                        //üũ ������: ���� ����

    //360���� �����ؾ� ��
	for (; Angle < 360.0f; ++Angle)
	{
		float4 CheckPos = { 0.0f, -Radius };		                                        //�������� ��������ŭ �ö� ��, ������ ���ϱ� �� ���� ������ �ʱ�ȭ
		CheckPos.RotaitonZDeg(Angle);				                                        //ȸ����ȯ ����
		CheckPos += CenterPos;						                                        //ȸ����Ų �� ��ġ ��ȭ

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

void MapModifier::DrawPixelRect(float4 _Pos, int _Horz, int _Vert)
{
    HDC MapDc = Map::MainMap->GetMapDC();

    std::string ColMapName = Map::MainMap->GetColMapName();
    GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);

    if (nullptr == ColImage)
    {
        MsgAssert("�浹�˻縦 ���� ColImage�� nullptr�Դϴ�.");
        return;
    }

    int lineThick = 2;
    float Horizon = static_cast<float>(_Horz);
    float Vertical = static_cast<float>(_Vert);

    float4 Pos = { _Pos.x - Horizon - 1.0f, _Pos.y - Vertical - 1.0f };                       //�簢�� ������ܿ��� ���μ��� 1pix �Űܰ� ��


    float4 ChecUp       = {Pos.x, Pos.y };                                                    //�簢�� ���� ���
    float4 CheckDown    = { Pos.x, Pos.y + Vertical*2 + 2.0f};                                //�簢�� ���� �ϴ�

    float4 ChecLeft     = { Pos.x, Pos.y };                                                   //�簢�� ���� ���
    float4 CheckRight   = { Pos.x + Horizon*2 + 2.0f, Pos.y };                                //�簢�� ���� ���

    float4 Boundary = { Pos.x + Horizon * 2 + 2.0f, Pos.y + Vertical * 2 + 2.0f };

    for (; ChecUp.x < Boundary.x;)
    {
         if (Blue == ColImage->GetPixelColor(ChecUp, Magenta))
        {
            //���� ���� �� ����
            //PS_SOLID: �Ǽ�
            HPEN MyPen = CreatePen(PS_SOLID, lineThick, LineColor);
            HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);

            HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(LineColor);                       //�ܻ��� ���� ���� �귯���� ����
            HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);                     //������ DC�� ��ü ����

            Rectangle(MapDc,
                ChecUp.ix() - lineThick,
                ChecUp.iy() - lineThick,
                ChecUp.ix() + lineThick,
                ChecUp.iy() + lineThick);


            SelectObject(MapDc, OldBrush);                                              //�ٽ� ���� �귯�� ����
            DeleteObject(MyBrush);                                                      //������ �귯�� ����

            SelectObject(MapDc, OldPen);
            DeleteObject(MyPen);
        }

        if (Blue == ColImage->GetPixelColor(CheckDown, Magenta))
        {
            //���� ���� �� ����
            //PS_SOLID: �Ǽ�
            HPEN MyPen = CreatePen(PS_SOLID, lineThick, LineColor);
            HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);

            HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(LineColor);                       //�ܻ��� ���� ���� �귯���� ����
            HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);                     //������ DC�� ��ü ����

            Rectangle(MapDc,
                CheckDown.ix() - lineThick,
                CheckDown.iy() - lineThick,
                CheckDown.ix() + lineThick,
                CheckDown.iy() + lineThick);


            SelectObject(MapDc, OldBrush);                                              //�ٽ� ���� �귯�� ����
            DeleteObject(MyBrush);                                                      //������ �귯�� ����

            SelectObject(MapDc, OldPen);
            DeleteObject(MyPen);
        }

        ++ChecUp.x;
        ++CheckDown.x;
    }

    for (; ChecLeft.y < Boundary.y;)
    {

        if (Blue == ColImage->GetPixelColor(ChecLeft, Magenta))
        {
            //���� ���� �� ����
            //PS_SOLID: �Ǽ�
            HPEN MyPen = CreatePen(PS_SOLID, lineThick, LineColor);
            HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);

            HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(LineColor);                       //�ܻ��� ���� ���� �귯���� ����
            HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);                     //������ DC�� ��ü ����

            Rectangle(MapDc,
                ChecLeft.ix() - lineThick,
                ChecLeft.iy() - lineThick,
                ChecLeft.ix() + lineThick,
                ChecLeft.iy() + lineThick);


            SelectObject(MapDc, OldBrush);                                              //�ٽ� ���� �귯�� ����
            DeleteObject(MyBrush);                                                      //������ �귯�� ����

            SelectObject(MapDc, OldPen);
            DeleteObject(MyPen);
        }

        if (Blue == ColImage->GetPixelColor(CheckRight, Magenta))
        {
            //���� ���� �� ����
            //PS_SOLID: �Ǽ�
            HPEN MyPen = CreatePen(PS_SOLID, lineThick, LineColor);
            HPEN OldPen = (HPEN)SelectObject(MapDc, MyPen);

            HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(LineColor);                       //�ܻ��� ���� ���� �귯���� ����
            HBRUSH OldBrush = (HBRUSH)SelectObject(MapDc, MyBrush);                     //������ DC�� ��ü ����

            Rectangle(MapDc,
                CheckRight.ix() - lineThick,
                CheckRight.iy() - lineThick,
                CheckRight.ix() + lineThick,
                CheckRight.iy() + lineThick);


            SelectObject(MapDc, OldBrush);                                              //�ٽ� ���� �귯�� ����
            DeleteObject(MyBrush);                                                      //������ �귯�� ����

            SelectObject(MapDc, OldPen);
            DeleteObject(MyPen);
        }

        ++ChecLeft.y;
        ++CheckRight.y;
    }
     
}
