#include "Drum.h"
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "GlobalValue.h"
#include "MapModifier.h"
#include "PetrolEffect.h"
#include "ContentsEnums.h"
#include "Player.h"
#include "Weapon.h"

bool Drum::EffectEnd = false;
int Drum::Count = 0;

Drum::Drum()
{
}

Drum::~Drum()
{
}

void Drum::Start()
{
    //Render
    MapObjRender = CreateRender(WormsRenderOrder::MapObject);
    MapObjRender->SetImage("oildrum1.bmp");
    MapObjRender->SetScale(ObjScale);
    MapObjRender->SetPosition(GetPos());

    //Animation
    MapObjRender->CreateAnimation({ .AnimationName = "OilDrum_1", .ImageName = "oildrum1.bmp", .Start = 0, .End = 19 });
    MapObjRender->CreateAnimation({ .AnimationName = "OilDrum_2", .ImageName = "oildrum2.bmp", .Start = 0, .End = 19 });
    MapObjRender->CreateAnimation({ .AnimationName = "OilDrum_3", .ImageName = "oildrum3.bmp", .Start = 0, .End = 19 });
    MapObjRender->CreateAnimation({ .AnimationName = "OilDrum_4", .ImageName = "oildrum4.bmp", .Start = 0, .End = 19 });


    MapObjRender->ChangeAnimation("OilDrum_1");

    //Collision
    MapObjCol = CreateCollision(static_cast<int>(WormsCollisionOrder::Drum));
    MapObjCol->SetScale({ 20.0f, 33.0f });
    MapObjCol->SetPosition(GetPos());
    MapObjCol->SetDebugRenderType(CT_CirCle);

    SetColImage();
    ExplosionEffectInit();
}

void Drum::Update(float _DeltaTime)
{
    HitWeaponCheck();
    AnimCheck();

    if (0 >= Gauge && false == IsExplosion)
    {
       MapObjRender->Off();
       Explosion();
    }
   
    if (true == IsExplosion && false == IsPetrolEffectOn)
    { 
        IsPetrolEffectOn = true;
        MapObjCol->Off();
        CreatePetrol(30);
    }


    if (30 == Count)
    {
        Count = 0;
        EffectEnd = false;
        Death();
    }


    if (true == IsUnderWaterCheck())                                   //�ٴٿ� ������ ���� ����
    {
        Death();
    }

    ExplosionAnimationOff();


    //�̵� ����
    GravityApplied(_DeltaTime);                                     //�߷� ����
    float4 NextPos = GetPos() + (MoveDir * _DeltaTime);
    NextPos = PullUp(NextPos, _DeltaTime);                          //�߷¿� ���� �ϰ��� ��ġ���� �ٽ� ������ ����ø�

    SetMove(MoveDir * _DeltaTime);                                  //�̵� ����
}


void Drum::CreatePetrol(int _NumOfPetrol)
{
    if (0 >= _NumOfPetrol)
    {
        MsgAssert("������ oil ������ ���� 0 ���ϰ� �� �� �����ϴ�.");
    }

    NumOfPetrol = _NumOfPetrol;

    for (int i = 0; i < NumOfPetrol; i++)
    {
        float4 Pos = GetPos();

        PetrolEffect* NewPetrol = GetLevel()->CreateActor<PetrolEffect>();
        NewPetrol->SetPos(Pos);
    }
}


void Drum::HitWeaponCheck()
{
    if (nullptr != MapObjCol)
    {
        if (true == MapObjCol->Collision({ .TargetGroup = static_cast<int>(WormsCollisionOrder::Weapon), .TargetColType = CollisionType::CT_CirCle, .ThisColType = CollisionType::CT_CirCle })
            )
        {
            Gauge = 0;
            MapObjCol->Off();
        }
    }
}


void Drum::HitPlayerCheck()
{
    float4 Pos = GetPos();
    MapModifier::MainModifier->SetModifierColScale({ 100, 100 });                                              //Modifier �浹ü�� ũ�⸦ ���� ����
    MapModifier::MainModifier->CreateHole(Pos, BombScale);

    GameEngineCollision* ModifierCollision = MapModifier::MainModifier->GetModifierCollision();
    AttackPlayer(ModifierCollision);
}

void Drum::AttackPlayer(GameEngineCollision* _Col) // �ӽ� ���� �Ϸ�
{                   // ���� CollisionScale���� �ʿ�                                   :_Col->SetScale({ static_cast<float>(BombScale * 2) });
                    // _Col->GetActorPlusPos()�� ��Ȯ�� ���� ��ġ�� �ǰ� ���� �ʿ�

    if (nullptr == _Col)
    {
        MsgAssert("üũ�� �ݸ����� �����ϴ�.");
    }

    // �÷��̾� üũ
    std::vector<GameEngineCollision*> CollisionList;

    int Radius = BombScale / 2;

    if (_Col != nullptr && true == _Col->Collision({ .TargetGroup = static_cast<int>(WormsCollisionOrder::Player), .TargetColType = CollisionType::CT_CirCle, .ThisColType = CollisionType::CT_CirCle }, CollisionList))
    {
        for (int i = 0; i < CollisionList.size(); i++)
        {
            Player* ColPlayer = dynamic_cast<Player*>(CollisionList[i]->GetActor());
            float4 Distance = ColPlayer->GetPos() - _Col->GetActorPlusPos();                                                                    //���� ����

            int proportional_dmg = static_cast<int>(MaxDmg * (1 - Distance.Size() / Radius) + MinDmg * (Distance.Size() / Radius));
            float proportional_power = MaxKnockBackPower * (1 - Distance.Size() / Radius) + MinKnockBackPower * (Distance.Size() / Radius);     //Lerp
            Distance.Normalize();

            //              �Ÿ� ��ʵ�����,    ���󰡴� ����,   �Ÿ� ��� ���󰡴� ���� 
            ColPlayer->Damaged(proportional_dmg, Distance, proportional_power);
            //���⼭ Dmg �� �ִ� ������, KnockBackPower�� �ִ� �˹� �Ŀ��� �̾߱���
        }
    }
}



void Drum::Explosion() //����
{
    if (CheckCollision(MapObjCol) == true)
    {
        return;
    }
    else
    {
        IsExplosion = true;

        ExplosionCircle->SetPosition(MapObjRender->GetPosition());
        ExplosionCircle->On();
        ExplosionCircle->ChangeAnimation("Explosion", 0);

        ExplosionElipse->SetPosition(MapObjRender->GetPosition());
        ExplosionElipse->On();
        ExplosionElipse->ChangeAnimation("ExplosionElipse", 0);

        PootTextAnimation->SetPosition(MapObjRender->GetPosition());
        PootTextAnimation->On();
        PootTextAnimation->ChangeAnimation("Poot", 0);

        HitPlayerCheck();                                                                       //�÷��̾�� ������ ����(���� �ݰ濡 �ִٸ�)
    }
}


bool Drum::CheckCollision(GameEngineCollision* _Col)
{
    if (nullptr == _Col)
    {
        MsgAssert("üũ�� �ݸ����� �����ϴ�.");
    }

    //��üũ
    if (Blue == ColImage->GetPixelColor(_Col->GetActorPlusPos(), Blue) && _Col->GetActorPlusPos().y > 0)               //�ʿ� ������ �����
    {
        return true;
    }

    //�� ������ �������� üũ
    float4 _Pos = _Col->GetPosition();
    if (!(-640 <= _Pos.x && _Pos.x < 4480 && -743 <= _Pos.y && _Pos.y < 1310))
    {
        return true;
    }

    return false;
}


void Drum::AnimCheck()
{

    if (100 >= Gauge && 50 < Gauge)
    {
        MapObjRender->ChangeAnimation("OilDrum_2");
    }
    else if (50 >= Gauge && 0 < Gauge)
    {
        MapObjRender->ChangeAnimation("OilDrum_3");
    }
    else if (0 >= Gauge)
    {
        MapObjRender->ChangeAnimation("OilDrum_4");
    }
}

void Drum::ExplosionEffectInit()
{
    ExplosionCircle = CreateRender("circle50.bmp", WormsRenderOrder::MapObject);
    ExplosionCircle->CreateAnimation({ .AnimationName = "Explosion", .ImageName = "circle50.bmp", .Start = 0, .End = 8, .InterTime = 0.05f , .Loop = false });
    ExplosionCircle->CreateAnimation({ .AnimationName = "Idle", .ImageName = "circle50.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    ExplosionCircle->SetScale({ 100, 100 });

    ExplosionCircle->ChangeAnimation("Idle");
    ExplosionCircle->Off();

    ExplosionElipse = CreateRender("Elipse50.bmp", WormsRenderOrder::MapObject);
    ExplosionElipse->CreateAnimation({ .AnimationName = "ExplosionElipse", .ImageName = "Elipse50.bmp", .Start = 0, .End = 19, .InterTime = 0.03f , .Loop = false });
    ExplosionElipse->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Elipse50.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    ExplosionElipse->SetScale({ 150, 150 });

    ExplosionElipse->ChangeAnimation("Idle");
    ExplosionElipse->Off();

    PootTextAnimation = CreateRender("Poot.bmp", WormsRenderOrder::Weapon);
    PootTextAnimation->CreateAnimation({ .AnimationName = "Poot", .ImageName = "Poot.bmp", .Start = 0, .End = 17, .InterTime = 0.02f , .Loop = false });
    PootTextAnimation->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Poot.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    PootTextAnimation->SetScale({ 70, 70 });

    PootTextAnimation->ChangeAnimation("Idle");
    PootTextAnimation->Off();
}

void Drum::ExplosionAnimationOff()
{
    if (ExplosionCircle->IsAnimationEnd() == true)
    {
        ExplosionCircle->ChangeAnimation("Idle");
        ExplosionCircle->Off();
    }
    if (ExplosionElipse->IsAnimationEnd() == true)
    {
        ExplosionElipse->ChangeAnimation("Idle");
        ExplosionElipse->Off();
    }

    if (PootTextAnimation->IsAnimationEnd() == true)
    {
        PootTextAnimation->ChangeAnimation("Idle");
        PootTextAnimation->Off();
    }
}