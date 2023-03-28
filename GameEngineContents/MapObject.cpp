#include "MapObject.h"
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "GlobalValue.h"
#include "Player.h"
#include "Map.h"
#include "MapModifier.h"
#include "ContentsEnums.h"


#include <GameEnginePlatform/GameEngineWindow.h>

MapObject::MapObject() 
{
   
}

MapObject::~MapObject() 
{
}

void MapObject::Start()
{
    //Render
    MapObjRender = CreateRender(WormsRenderOrder::MapObject);
    MapObjRender->SetImage("mcrate1.bmp");
    MapObjRender->SetScale(ObjScale);
    MapObjRender->SetPosition(GetPos());

    //Animation
    MapObjRender->CreateAnimation({ .AnimationName = "Medikit_Idle", .ImageName = "mcrate1.bmp", .Start = 0, .End = 11});
    MapObjRender->CreateAnimation({ .AnimationName = "Medikit_IdleFlying", .ImageName = "mcrate2.bmp", .Start = 9, .End = 9 });
    MapObjRender->CreateAnimation({ .AnimationName = "Medikit_RightFlying", .ImageName = "mcrate2.bmp", .Start = 0, .End = 8 });
    MapObjRender->CreateAnimation({ .AnimationName = "Medikit_LeftFlying", .ImageName = "mcrate2.bmp", .Start = 10, .End = 18 });        //11
   
    
    MapObjRender->ChangeAnimation("Medikit_IdleFlying");

    //Collision
    MapObjCol = CreateCollision(static_cast<int>(WormsCollisionOrder::MapObject));
    MapObjCol->SetScale({20.0f, 27.0f});
    MapObjCol->SetPosition(GetPos());
    MapObjCol->SetDebugRenderType(CT_CirCle);

    //ColImg Setting
    std::string ColMapName = Map::MainMap->GetColMapName();
    ColImage = GameEngineResources::GetInst().ImageFind(ColMapName);
}

void MapObject::Update(float _DeltaTime)
{
    if (GameEngineInput::IsDown("DebugCollision"))
    {
        GetLevel()->DebugRenderSwitch();
    }

    if (true == IsFlying)                                       //ó�� ���߿��� ������ ��, ���ϻ� �ִϸ��̼� ����(1ȸ)
    {
        //���߿� �ִ� ������ �ٶ��� ������ �޾ƾ� ��
        //wind�� ���⿡ �°� �ִϸ��̼� ��ü�ؾ� ��
        IsFlying = false;
        MapObjRender->ChangeAnimation("Medikit_IdleFlying");
    }

    if (false == IsChangedAnim && true == IsGround())            //���� ������ �⺻ �̹����� ��ü
    {
        IsChangedAnim = true;
        MapObjRender->ChangeAnimation("Medikit_Idle");
    }


    RecoveryCheck();












    //�̵� ����
    GravityApplied(_DeltaTime);                                     //�߷� ����
    float4 NextPos = GetPos() + (MoveDir * _DeltaTime);
    NextPos = PullUp(NextPos, _DeltaTime);                          //�߷¿� ���� �ϰ��� ��ġ���� �ٽ� ������ ����ø�

    SetMove(MoveDir * _DeltaTime);                                  //�̵� ����
}


void MapObject::RecoveryCheck()
{
    std::vector<GameEngineCollision*> CollisionPlayer;

    if (true == MapObjCol->Collision({ .TargetGroup = static_cast<int>(WormsCollisionOrder::Player), .TargetColType = CollisionType::CT_CirCle, .ThisColType = CollisionType::CT_CirCle }, CollisionPlayer))
    {
        for (int i = 0; i < CollisionPlayer.size(); i++)
        {
            int PlayerHp = dynamic_cast<Player*>(CollisionPlayer[i]->GetActor())->GetPlayerHP();

            if (100 <= PlayerHp + Recovery)//PlayerHP�� Max�� �� �� �־�� ��
            {
                dynamic_cast<Player*>(CollisionPlayer[i]->GetActor())->SetHP(100);
            }
            else
            {
                dynamic_cast<Player*>(CollisionPlayer[i]->GetActor())->SetHP(PlayerHp + Recovery);
            }


        }
        Death();
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
    if (Blue != ColImage->GetPixelColor(_NextPos, Blue))
    {
        return _NextPos;
    }

    while (true)
    {
        MoveDir.y -= 1;

        _NextPos = GetPos() + MoveDir * _DeltaTime;

        if (RGB(0, 0, 255) == ColImage->GetPixelColor(_NextPos, RGB(0, 0, 255)))
        {
            continue;
        }

        return _NextPos;
    }
}

bool MapObject::IsGround()
{
    float4 GroundCheckPos = GetPos() + float4::Down;                                    //�˻� ��� ��ġ: ������ ��ġ���� �� ĭ �Ʒ� ����

    if (Blue == ColImage->GetPixelColor(GroundCheckPos, RGB(0, 0, 0)))
    {
        return true;
    }
    else
    {
        return false;
    }
}



void MapObject::SetCurPlayer() 
{
    Player* CurPlayer = GlobalValue::gValue.GetPlayer();

    if (nullptr == CurPlayer)
    {
        MsgAssert("CurPlayer�� nullptr�Դϴ�.");
    }

}