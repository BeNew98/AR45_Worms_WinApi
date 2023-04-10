#include "WeaponFirePunch.h"
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>

#include "ContentsEnums.h"
#include "Player.h"
#include "SmokeSparkEffect.h"
#include "MapModifier.h"
#include "SmokeSparkEffect.h"
#include "Map.h"

WeaponFirePunch::WeaponFirePunch()
{
}

WeaponFirePunch::~WeaponFirePunch()
{
}

void WeaponFirePunch::Start()
{


    MoveSpeed = 200;							// ����ӷ�

    Dir = float4::Up;						// ���� ���� ����
    BombScale = 26;

    MaxDmg = 45;
    MinDmg = 21;

    MaxKnockBackPower = 300; // dir Y�� ���� �ʿ���
    MinKnockBackPower = 200;

    WeaponNumber = static_cast<int>(WeaponNum::FirePunch);							// ���� �̸��� �ش��ϴ� Number
    WeaponName = "FirePunch";							// ���� �̸�

    std::string Name = Map::MainMap->GetColMapName();
    MapCollision = GameEngineResources::GetInst().ImageFind(Name);
    PunchCollision = CreateCollision(WormsCollisionOrder::Weapon);
    PunchCollision->SetPosition({ 0,50 });
    PunchCollision->SetScale({ 50,50 });

    Timer = 0.45f;								// Ÿ�̸�
}

void WeaponFirePunch::Update(float _DeltaTime)
{
    if (false == isFire && false == isCharging)
    {
        SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�
    }


    if (false == isCharging) // �߰� ���� �ʿ� : isFire�̶� ��ĥ ��� �����غ���
    {
        if (true == PressShoot()) // �����̽��ٸ� �����ٸ�
        {
            CurPlayer->ChangePlayerAnimation("FirePunchCharging");
            isCharging = true;
        }
    }
    else
    {
        if (false == isFire) 
        {
            if (true == UpShoot()) // �����̽��ٸ� �´ٸ�
            {
                CurPlayer->ChangePlayerAnimation("FirePunchFire");
                isFire = true;
                StartPos = CurPlayer->GetPos();
                SetPos(StartPos);
                GameEngineResources::GetInst().SoundPlay("DRAGONPUNCH.wav");
                PunchCollision->SetMove(CurPlayer->GetPlayerDir() * 10);
            }
            else if (true == PressShoot() && GetChargeTime() > 0.6f)
            {
                CurPlayer->ChangePlayerAnimation("FirePunchCharging",7);
            }
        }
        else
        {
            //float4 CurPos = CurPlayer->GetPos();
            if (Timer > 0) // �������� ��ġ
            {
                // ī�޶� �̵�
                float4 CamPos = float4::Zero.Lerp(GetLevel()->GetCameraPos(), StartPos - GameEngineWindow::GetScreenSize().half(), _DeltaTime * 10);
                GetLevel()->SetCameraPos(CamPos);

                Timer -= _DeltaTime;
                EffectTimer -= _DeltaTime;
                HitTimer -= _DeltaTime;
                if ((StartPos + Dir * _DeltaTime * 300.0f).y > 0)
                {
                    StartPos += Dir * _DeltaTime * 300.0f;
                    PunchCollision->SetMove(Dir * _DeltaTime * 300.0f);
                }
                CurPlayer->SetPos(StartPos); // �÷��̾� �̵�
                
                //PunchCollision->SetPosition(StartPos - GetPos() + float4{0,50});

                if (EffectTimer < 0) // - �����ʿ� ����ũ ��� ����Ʈ�� ���� �ʿ���
                {
                    GameEngineRender* StarSmoke = CreateRender(WormsRenderOrder::Weapon);
                    StarSmoke->SetPosition(PunchCollision->GetPosition() + float4{0,-50} + float4{ GameEngineRandom::MainRandom.RandomFloat(-15.0f,15.0f) });
                    StarSmoke->SetScale({ 60, 60 });
                    StarSmoke->CreateAnimation({ .AnimationName = "Smoke", .ImageName = "starEffect.bmp", .Start = 0, .End = 9, .InterTime = 0.1f , .Loop = false });
                    StarSmoke->ChangeAnimation("Smoke");
                    EffectTimer = 0.03f;
                }


                if (true == AttackPlayer(PunchCollision, false) && HitTimer <0)																  // 3. Bomb�ݸ��� Player Check
                {
                    GameEngineRender* StarSmoke = CreateRender(WormsRenderOrder::Weapon);
                    StarSmoke->SetPosition(PunchCollision->GetPosition() + float4{ 0,-50 });
                    StarSmoke->SetScale({ 50, 50 });
                    StarSmoke->CreateAnimation({ .AnimationName = "Hit", .ImageName = "firehit.bmp", .Start = 0, .End = 8, .InterTime = 0.1f , .Loop = false });
                    StarSmoke->ChangeAnimation("Hit");
                    HitTimer = 0.1f;
                    GameEngineResources::GetInst().SoundPlay("FirePunchImpact.wav");
                }

                MapModifier::MainModifier->CreateHole(StartPos + float4(0,-20), static_cast<int>(BombScale));                                         //4. createHole
            }
            else // ��ġ�� �����ٸ�
            {
                if (false == isAfterAnimation) // �������� �ִϸ��̼�
                {
                    CurPlayer->ChangePlayerAnimation("FirePunchAfter");
                    isAfterAnimation = true;
                }
                else
                {
                    CurPlayer->SetMove(float4(0, 120*_DeltaTime));
                    if (true == CurPlayer->IsPlayerAnimationEnd()) // �������� �ִϸ��̼��� �����ٸ�
                    {
                        isWeaponDone = true; // ��������
                    }
                }
            }
        }
    }
}
