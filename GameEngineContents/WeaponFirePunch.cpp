#include "WeaponFirePunch.h"
#include "ContentsEnums.h"
#include <GameEngineCore/GameEngineResources.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include "Player.h"
#include "SmokeSparkEffect.h"
#include "MapModifier.h"
#include "SmokeSparkEffect.h"
#include <GameEngineCore/GameEngineLevel.h>

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

    MaxKnockBackPower = 13 * 3; // ���� �� ���ɼ� ����
    MinKnockBackPower = 13 * 3;

    WeaponNumber = static_cast<int>(WeaponNum::FirePunch);							// ���� �̸��� �ش��ϴ� Number
    WeaponName = "FirePunch";							// ���� �̸�

    MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp");		//�浹��
    PunchCollision = CreateCollision(WormsCollisionOrder::Weapon);
    PunchCollision->SetPosition({ 0,-25 });
    PunchCollision->SetScale({ 20,20 });

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
            if (true == GameEngineInput::IsUp("Shoot")) // �����̽��ٸ� �´ٸ�
            {
                CurPlayer->ChangePlayerAnimation("FirePunchFire");
                isFire = true;
                StartPos = CurPlayer->GetPos();
            }
        }
        else
        {
            SetPos(CurPlayer->GetPos());
            if (Timer > 0) // �������� ��ġ
            {
                Timer -= _DeltaTime;
                EffectTimer -= _DeltaTime;
                if ((StartPos + Dir * _DeltaTime * 300.0f).y > 0)
                {
                    StartPos += Dir * _DeltaTime * 300.0f;
                }
                if (EffectTimer < 0) // - �����ʿ� ����ũ ��� ����Ʈ�� ���� �ʿ���
                {
                    SmokeSparkEffect* Smoke = GetLevel()->CreateActor<SmokeSparkEffect>();
                    Smoke->SetPos(StartPos);
                    Smoke->CreateSmokeSpark(1, 0, BombScale);
                    EffectTimer = 0.05f;
                }

                CurPlayer->SetPos(StartPos); // �÷��̾� �̵�
                //GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();								  // 1. Bomb �ݸ��� ��������
                //BombCollision->SetPosition(StartPos + float4{0,-60});											                                       // 2. Bomb �ݸ��� �̵�
                //BombCollision->SetScale({ 20,20 });
                AttackPlayer(PunchCollision,false);																	  // 3. Bomb�ݸ��� Player Check

                MapModifier::MainModifier->CreateHole(PunchCollision->GetActorPlusPos(), BombScale);                                         //4. createHole
            }
            else // ��ġ�� �����ٸ�
            {
                if (false == isAfterAnimation) // �������� �ִϸ��̼�
                {
                    CurPlayer->ChangePlayerAnimation("FirePunchAfter");
                    isAfterAnimation = true;
                }
                else if (true == CurPlayer->IsPlayerAnimationEnd()) // �������� �ִϸ��̼��� �����ٸ�
                {
                    isWeaponDone = true; // ��������
                }
            }
        }
    }
}
