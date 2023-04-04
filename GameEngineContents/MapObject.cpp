#include "MapObject.h"
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineResources.h>
#include "Map.h"





MapObject::MapObject() 
{
   
}

MapObject::~MapObject() 
{
}


void MapObject::SetColImage()
{
    //ColImg Setting
    std::string ColMapName = Map::MainMap->GetColMapName();
    ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);

    if (nullptr == ColImage)
    {
        MsgAssert("ColImage�� nullptr�Դϴ�.");
    }
}

float4 MapObject::GetMapObjPos()
{
    float4 ReturnPos = float4::Zero;
    ReturnPos.x = GameEngineRandom::MainRandom.RandomFloat(50.0f, 3790.0f);                               //��ũ��: { 3840.0f, 1392.0f }, x�� ������ ���� �� ũ�� �翷���� 50 pix �������� ����� ����
    ReturnPos.y = 100.0f;

    return ReturnPos;
}


void MapObject::GravityApplied(float _DeltaTime)
{
    MoveDir += (float4::Down * Gravity * _DeltaTime);
}


float4 MapObject::PullUp(float4 _NextPos, float _DeltaTime)
{
    if (nullptr == ColImage)
    {
        MsgAssert("ColImage�� nullptr�Դϴ�.");
    }

    if (Blue != ColImage->GetPixelColor(_NextPos, Blue))
    {
        return _NextPos;
    }

    while (true)
    {
        MoveDir.y -= 1;

        _NextPos = GetPos() + MoveDir * _DeltaTime;

        if (RGB(0, 0, 255) == ColImage->GetPixelColor(_NextPos, RGB(0, 0, 0)))
        {
            continue;
        }

        return _NextPos;
    }
}

bool MapObject::IsGroundCheck()
{
    float4 GroundCheckPos = GetPos() + float4::Down;                                    //�˻� ��� ��ġ: �̹����� �߾� �ϴܺο��� �� ĭ �Ʒ� ����
    GroundCheckPos.y += ObjScale.hy();

    if (nullptr == ColImage)
    {
        MsgAssert("ColImage�� nullptr�Դϴ�.");
    }

    if (Blue == ColImage->GetPixelColor(GroundCheckPos, RGB(0, 0, 0)))
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool MapObject::IsUnderWaterCheck()
{
    float4 CheckPos = GetPos() + float4::Down;                                          //�˻� ��� ��ġ: �̹����� �߾� �ϴܺο��� �� ĭ �Ʒ� ����
    CheckPos.y += ObjScale.hy();

    if (Map::MainMap->GetWavePos().y <= CheckPos.y)
    {
        return true;
    }
    return false;
}