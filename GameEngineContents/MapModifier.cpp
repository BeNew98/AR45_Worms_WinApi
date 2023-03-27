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
    //�浹�ʰ� �´��� �κ��� �ȼ��� ���� �ִ� �Լ�
	DrawPixel(_Pos, _Radius);

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

}

void MapModifier::CreateRect(float4 _Pos, int _Horz, int _Vert)
{
    ModifierCollision->SetPosition(_Pos);

    if (this == nullptr)
    {
        MsgAssert("ModifierActor�� nullptr �Դϴ�.");
        return;
    }

    if (0 >= _Horz)
    {
        MsgAssert("�簢�� ���� ���̰� 0���� �۰ų� ���� �� �����ϴ�.");
        return;
    }

    if (0 >= _Vert)
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
            RectRenderPos.ix() - _Horz,                                         //Left
            RectRenderPos.iy() - _Vert,                                         //Top
            RectRenderPos.ix() + _Horz,
            RectRenderPos.iy() + _Vert);

        SelectObject(MapDc, OldPen);
        DeleteObject(MyPen);

        SelectObject(MapDc, OldBrush);                                          //�ٽ� ���� �귯�� ����
        DeleteObject(MyBrush);                                                  //������ �귯�� ����

    }
    DrawPixelRect(_Pos, _Horz, _Vert);

    //ColMap�� �׸�
    {
        HPEN MyPen = CreatePen(PS_SOLID, 1, Magenta);
        HPEN OldPen = (HPEN)SelectObject(ColMapDc, MyPen);

        HBRUSH MyBrush = (HBRUSH)CreateSolidBrush(Magenta);                     //�ܻ��� ���� ���� �귯���� ����
        HBRUSH OldBrush = (HBRUSH)SelectObject(ColMapDc, MyBrush);              //������ DC�� ��ü ����

        Rectangle(ColMapDc,
            RectRenderPos.ix() - _Horz,
            RectRenderPos.iy() - _Vert,
            RectRenderPos.ix() + _Horz,
            RectRenderPos.iy() + _Vert);

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

	int lineThick = 2;
    float Radius = static_cast<float>(_Radius);
    float Angle = 0.0f;

	float4 CenterPos = _Pos;						                                        //üũ ������: ���� ����

    //360���� �����ؾ� ��
	for (; Angle < 90.0f; ++Angle)
	{
		float4 CheckPos = { 0.0f, -Radius };		                                        //�������� ��������ŭ �ö� ��, ������ ���ϱ� �� ���� ������ �ʱ�ȭ
		CheckPos.RotaitonZDeg(Angle);				                                        //ȸ����ȯ ����
		CheckPos += CenterPos;						                                        //ȸ����Ų �� ��ġ ��ȭ


        float4 CheckPos1 = {Radius,  0.0f };		                                        //90���� ����                                      
        CheckPos1.RotaitonZDeg(Angle);				
        CheckPos1 += CenterPos;

        float4 CheckPos2 = { 0.0f, Radius };                                               //180������ ����
        CheckPos2.RotaitonZDeg(Angle);
        CheckPos2 += CenterPos;

        float4 CheckPos3 = { -Radius, 0.0f };                                               //270������ ����
        CheckPos3.RotaitonZDeg(Angle);
        CheckPos3 += CenterPos;


      //  {
		    //////�浹���� �Ķ��� �κа� ���̴� �κ��� �� �ѷ��� ������ ��ĥ
      //      DWORD color = ColImage->GetPixelColor(CheckPos, Magenta);

      //      //������ 262 ~279��->Magenta�� Ȯ��
      //      int R = GetRValue(color);
      //      int G = GetGValue(color);
      //      int B = GetBValue(color);

      //      int a = 10;
      //  }

      //  {
      //      DWORD color = ColImage->GetPixelColor(CheckPos1, Magenta);

      //      //������ 262 ~279��->Magenta�� Ȯ��
      //      int R = GetRValue(color);
      //      int G = GetGValue(color);
      //      int B = GetBValue(color);

      //      int a = 10;
      //  }

      //  {
      //      DWORD color = ColImage->GetPixelColor(CheckPos2, Magenta);

      //      //������ 262 ~279��->Magenta�� Ȯ��
      //      int R = GetRValue(color);
      //      int G = GetGValue(color);
      //      int B = GetBValue(color);

      //      int a = 10;
      //  }
      //  {
      //      DWORD color = ColImage->GetPixelColor(CheckPos3, Magenta);

      //      //������ 262 ~279��->Magenta�� Ȯ��
      //      int R = GetRValue(color);
      //      int G = GetGValue(color);
      //      int B = GetBValue(color);

      //      int a = 10;
      //  }


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

        if (Blue == ColImage->GetPixelColor(CheckPos1, Magenta))
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
                    CheckPos1.ix() - lineThick,
                    CheckPos1.iy() - lineThick,
                    CheckPos1.ix() + lineThick,
                    CheckPos1.iy() + lineThick);


                SelectObject(MapDc, OldBrush);                                              //�ٽ� ���� �귯�� ����
                DeleteObject(MyBrush);                                                      //������ �귯�� ����

                SelectObject(MapDc, OldPen);
                DeleteObject(MyPen);
            }
        }

        if (Blue == ColImage->GetPixelColor(CheckPos2, Magenta))
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
                    CheckPos2.ix() - lineThick,
                    CheckPos2.iy() - lineThick,
                    CheckPos2.ix() + lineThick,
                    CheckPos2.iy() + lineThick);


                SelectObject(MapDc, OldBrush);                                              //�ٽ� ���� �귯�� ����
                DeleteObject(MyBrush);                                                      //������ �귯�� ����

                SelectObject(MapDc, OldPen);
                DeleteObject(MyPen);
            }
        }


        if (Blue == ColImage->GetPixelColor(CheckPos3, Magenta))
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
                    CheckPos3.ix() - lineThick,
                    CheckPos3.iy() - lineThick,
                    CheckPos3.ix() + lineThick,
                    CheckPos3.iy() + lineThick);


                SelectObject(MapDc, OldBrush);                                              //�ٽ� ���� �귯�� ����
                DeleteObject(MyBrush);                                                      //������ �귯�� ����

                SelectObject(MapDc, OldPen);
                DeleteObject(MyPen);
            }
        }


        /*else
        {
            Rectangle(MapDc,
                CheckPos.ix() - 1,
                CheckPos.iy() - 1,
                CheckPos.ix() + 1,
                CheckPos.iy() + 1);
        }*/
	}
}

void MapModifier::DrawPixelRect(float4 _Pos, int _Horz, int _Vert)
{
    HDC MapDc = Map::MainMap->GetMapDC();

    std::string ColMapName = Map::MainMap->GetColMapName();
    GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);

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
