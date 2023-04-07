#include "MapDecoration.h"
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "Map.h"
#include "MapModifier.h"
#include "ContentsEnums.h"


MapDecoration::MapDecoration()
{
}

MapDecoration::~MapDecoration()
{
}

void MapDecoration::Start()
{
    Objects.push_back("MapObject1.bmp");
    Objects.push_back("MapObject2.bmp");
    Objects.push_back("MapObject3.bmp");
    ColObjects.push_back("MapObject1_Col.bmp");
    ColObjects.push_back("MapObject2_Col.bmp");
    ColObjects.push_back("MapObject3_Col.bmp");


    RandIdx = GameEngineRandom::MainRandom.RandomInt(0, 2);
    DecoScale = GameEngineResources::GetInst().ImageFind(Objects[RandIdx])->GetImageScale();
}

void MapDecoration::Update(float _DeltaTime)
{

}

void MapDecoration::MergeMap()
{
    if (float4::Zero == DecoScale)
    {
        MsgAssert("DecoObject Scale�� Zero�Դϴ�.");
        return;
    }

    float4 CheckPos = float4::Zero;                //������Ʈ �̹������� �ȼ� üũ�� ��ġ

    float4 RealPos = GetPos();                     //���� �ʿ� �����Ǵ� ��ġ
    RealPos -= DecoScale.half();

    HDC ColMapDc = Map::MainMap->GetColMapDC();
    HDC MapDc = Map::MainMap->GetMapDC();

    GameEngineImage* ObjImage = GameEngineResources::GetInst().ImageFind(Objects[RandIdx]);
    GameEngineImage* ColObjImage = GameEngineResources::GetInst().ImageFind(ColObjects[RandIdx]);

    std::string ColMapName = Map::MainMap->GetColMapName();
    GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);

    for (; CheckPos.y < DecoScale.y; ++CheckPos.y)
    {
        for (CheckPos.x = 0.0f; CheckPos.x < DecoScale.x; ++CheckPos.x)
        {
            //Obj�� �浹�̹����� �Ķ����̸� �ʿ����� �ش� ��ġ�� ���� ����
            if (Blue == ColObjImage->GetPixelColor(CheckPos, Magenta))
            {
                //�浹�ʿ��� �ش� ������ �Ķ����� �ƴ� ���� �ʿ� ������Ʈ�� �׸�
                if (Blue != ColImage->GetPixelColor(RealPos, Magenta))
                {
                    DWORD Color = ObjImage->GetPixelColor(CheckPos, Magenta);
                    SetPixel(MapDc, RealPos.ix(), RealPos.iy(), Color);                   //���� �ʿ� �׸�
                    SetPixel(ColMapDc, RealPos.ix(), RealPos.iy(), Blue);                 //�浹 �ʿ� �׸�
                }
            }
            ++RealPos.x;
        }

        RealPos.x -= DecoScale.x;
        ++RealPos.y;
    }

    Death();           //�� �ռ��� ������ ������Ʈ�� ���͸� ����
}

void MapDecoration::MakeRandomPosSets()
{
    RandPosSets.clear();

    float4 StartGridPos = { 200.0f, 0.0f };

    float4 Scale = GameEngineResources::GetInst().ImageFind(Objects[RandIdx])->GetImageScale();
    float4 Boundary = { 3640.0f, 1120.0f };                                                         //���� �� ũ�⿡�� ����, ���� ������ ����

    std::string ColMapName = Map::MainMap->GetColMapName();
    GameEngineImage* ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);

    int B_Count = 0;

    for (; StartGridPos.y < Boundary.y; StartGridPos.y += Scale.y)
    {
        for (StartGridPos.x = 200.0f; StartGridPos.x < Boundary.x; StartGridPos.x += Scale.x)
        {
            float4 Center = StartGridPos + Scale.half();
            float4 CenterDown = { Center.x, Center.y + Scale.hy() };
            float4 LeftDown = { CenterDown.x - Scale.hx(), CenterDown.y };
            float4 RightDown = { CenterDown.x + Scale.hx(), CenterDown.y };

            if (Blue == ColImage->GetPixelColor(CenterDown, Magenta))
            {
                ++B_Count;
            }

            if (Blue == ColImage->GetPixelColor(LeftDown, Magenta))
            {
                ++B_Count;
            }

            if (Blue == ColImage->GetPixelColor(RightDown, Magenta))
            {
                ++B_Count;
            }


            if (3 == B_Count && Blue != ColImage->GetPixelColor(Center, Magenta))
            {
                RandPosSets.push_back(Center);
            }

            B_Count = 0;
        }
    }

}

float4 MapDecoration::GetRandomPos()
{
    if (PrevIdx != RandIdx)
    {
        MakeRandomPosSets();
        PrevIdx = RandIdx;
    }

    int Size = static_cast<int>(RandPosSets.size());
    int RandNum = GameEngineRandom::MainRandom.RandomInt(0, Size - 1);

    return RandPosSets[RandNum];
}