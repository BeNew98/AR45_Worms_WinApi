#include "Medikit.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "GlobalValue.h"
#include "Map.h"
#include "Player.h"
#include "ContentsEnums.h"

Medikit::Medikit() 
{
}

Medikit::~Medikit() 
{
}


void Medikit::Start()
{
    //Render
    MapObjRender = CreateRender(WormsRenderOrder::MapObject);
    MapObjRender->SetImage("mcrate1.bmp");
    MapObjRender->SetScale(ObjScale);
    MapObjRender->SetPosition(GetPos());

    //Animation
    MapObjRender->CreateAnimation({ .AnimationName = "Medikit_Idle", .ImageName = "mcrate1.bmp", .Start = 0, .End = 11 });
    MapObjRender->CreateAnimation({ .AnimationName = "Medikit_Flying", .ImageName = "mcrate2.bmp", .Start = 9, .End = 9 });
    MapObjRender->CreateAnimation({ .AnimationName = "Right_Medikit_Flying", .ImageName = "mcrate2.bmp", .Start = 0, .End = 8 });
    MapObjRender->CreateAnimation({ .AnimationName = "Left_Medikit_Flying", .ImageName = "mcrate2.bmp", .Start = 10, .End = 18 });


    MapObjRender->ChangeAnimation("Right_Medikit_Flying");

    //Collision
    MapObjCol = CreateCollision(static_cast<int>(WormsCollisionOrder::Medikit));
    MapObjCol->SetScale({ 20.0f, 27.0f });
    MapObjCol->SetPosition(GetPos());
    MapObjCol->SetDebugRenderType(CT_CirCle);

    SetColImage();

}

void Medikit::Update(float _DeltaTime)
{
    if (true == IsWindApplied() && false == IsChangedAnim)             //ó�� ���߿��� ������ ��, ���ϻ� �ִϸ��̼� ����(1ȸ)
    {
        //���߿� �ִ� ������ �ٶ��� ������ �޾ƾ� ��
        MoveDir.x += static_cast<float>(GlobalValue::gValue.GetWindPhase());

        DirCheck("Medikit_Flying");                                   //wind�� ���⿡ �°� �ִϸ��̼� ��ü�ؾ� ��
    }

    if (false == IsWindApplied() && false == IsChangedAnim)           //������ ���� �Ÿ��̻� ������ �ִٸ� �⺻ ���� �ִϸ��̼����� ����(���̻� �ٶ��� ������ ���� ����)
    {
        MapObjRender->ChangeAnimation("Medikit_Flying");
    }

    if (false == IsChangedAnim && true == IsGroundCheck())            //���� ������ �⺻ �̹����� ��ü
    {
        MoveDir = float4::Zero;
        IsChangedAnim = true;
        MapObjRender->ChangeAnimation("Medikit_Idle");
    }

    if (true == IsUnderWaterCheck())                                   //�ٴٿ� ������ ���� ����
    {
        Death();
    }

    RecoveryCheck();


    //�̵� ���� �Լ�
    GravityApplied(_DeltaTime);                                     //�߷� ����
    float4 NextPos = GetPos() + (MoveDir * _DeltaTime);
    NextPos = PullUp(NextPos, _DeltaTime);                          //�߷¿� ���� �ϰ��� ��ġ���� �ٽ� ������ ����ø�

    SetMove(MoveDir * _DeltaTime);                                  //�̵� ����
}



void Medikit::DirCheck(const std::string_view& _AnimationName)
{
    std::string PrevDirString = DirString;

    int WindDir = GlobalValue::gValue.GetWindPhase();                           //WindPhase(-10 ~ 10)

    if (0 > WindDir)
    {
        DirString = "Left_";
    }

    if (0 < WindDir)
    {
        DirString = "Right_";
    }

    if (0 == WindDir)
    {
        MapObjRender->ChangeAnimation(_AnimationName.data());
        return;
    }

    if (PrevDirString != DirString)
    {
        MapObjRender->ChangeAnimation(DirString + _AnimationName.data());
    }
}


void Medikit::RecoveryCheck()
{
    std::vector<GameEngineCollision*> CollisionPlayer;

    if (true == MapObjCol->Collision({ .TargetGroup = static_cast<int>(WormsCollisionOrder::Player), .TargetColType = CollisionType::CT_CirCle, .ThisColType = CollisionType::CT_CirCle }, CollisionPlayer))
    {
        for (int i = 0; i < CollisionPlayer.size(); i++)
        {
            dynamic_cast<Player*>(CollisionPlayer[i]->GetActor())->UsingHealkit(Recovery);
        }
        Death();
    }
}

bool Medikit::IsWindApplied()
{
    float4 CheckPos = GetPos();                                                  //�˻� ��� ��ġ: ������ ��ġ���� ������Ʈ �����ϸ�ŭ ������ ����
    CheckPos.y += (ObjScale.y * 1.5f);

    if (Blue == ColImage->GetPixelColor(CheckPos, RGB(0, 0, 0)))
    {
        return false;
    }
    else
    {
        return true;
    }
}