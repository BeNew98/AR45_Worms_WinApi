#include "MapObject.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "Map.h"
#include "MapModifier.h"
#include "ContentsEnums.h"
MapObject::MapObject() 
{
}

MapObject::~MapObject() 
{
}

void MapObject::Start()
{
    //TestRender = CreateRender(WormsRenderOrder::MapObject);
    //TestRender->SetImage("MapObject1.bmp");
    //TestRender->SetPosition(GetPos());
    //TestRender->SetScaleToImage();

    ObjScale = GameEngineResources::GetInst().ImageFind("MapObject1.bmp")->GetImageScale();
   

}

void MapObject::Update(float _DeltaTime)
{
    if (true == GameEngineInput::IsDown("DebugMode"))							//����� ���_�浹�� Ȥ�� ���� �� �� ����
    {
        MergeMap();
    }

   
}

void MapObject::MergeMap()
{
    if (float4::Zero == ObjScale)
    {
        MsgAssert("Object Scale�� Zero�Դϴ�.");
        return;
    }

    float4 CheckPos = float4::Zero;

    float4 RealPos = GetPos();
    RealPos -= ObjScale.half();

    HDC ColMapDc = Map::MainMap->GetColMapDC();
    HDC MapDc = Map::MainMap->GetMapDC();

    //ObjectImgaeName ������ ���� ����
    GameEngineImage* ColObjImage = GameEngineResources::GetInst().ImageFind("MapObject1_Col.bmp");
    GameEngineImage* ObjImage = GameEngineResources::GetInst().ImageFind("MapObject1.bmp");

    std::string ColMapName = Map::MainMap->GetColMapName();
    GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);

    for (; CheckPos.y < ObjScale.y; ++CheckPos.y)
    {
        for (CheckPos.x = 0.0f; CheckPos.x < ObjScale.x; ++CheckPos.x)
        {
            //Obj�� �浹�̹����� �Ķ����̸� �ʿ����� �ش� ��ġ�� ���� ����
            if (Blue == ColObjImage->GetPixelColor(CheckPos, Magenta))
            {
                //�浹�ʿ��� �ش� ������ �Ķ����� �ƴ� ���� �ʿ� �׷��� ��
                if (Blue != ColImage->GetPixelColor(RealPos, Magenta))
                {
                    DWORD Color = ObjImage->GetPixelColor(CheckPos, Magenta);
                    SetPixel(MapDc, RealPos.x, RealPos.y, Color);                   //���� �ʿ� �׸�
                    SetPixel(ColMapDc, RealPos.x, RealPos.y, Blue);                 //�浹 �ʿ� �׸�
                }                
            }
            ++RealPos.x;
        }

        RealPos.x -= ObjScale.x;
        ++RealPos.y;
    }
    Death();
}

std::vector<float4> MapObject::GetRandomPos()
{
    std::vector<float4> RandPosSets = std::vector<float4>();

 
    return RandPosSets;
}

float4 MapObject::SetObjPosRand()
{
    return float4::Zero;
}