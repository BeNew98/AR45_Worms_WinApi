#include "WeaponFirePunch.h"
#include "ContentsEnums.h"
#include "Map.h"
#include <GameEngineCore/GameEngineResources.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include "Player.h"

WeaponFirePunch::WeaponFirePunch()
{
}

WeaponFirePunch::~WeaponFirePunch()
{
}

void WeaponFirePunch::Start()
{


    MoveSpeed = 200;							// ����ӷ�
    //Dmg = 0.0f;								// ������

    float4 Dir = float4::Up;						// ���� ���� ����
    BombScale = 15;				// ���� ����

    WeaponNumber = static_cast<int>(WeaponNum::FirePunch);							// ���� �̸��� �ش��ϴ� Number
    WeaponName = "FirePunch";							// ���� �̸�

    MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp");		//�浹��

    //Player* CurPlayer = nullptr;
    Timer = 5.0f;								// Ÿ�̸�
}

void WeaponFirePunch::Update(float _DeltaTime)
{
    if (false == isFire)
    {
        SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�
    }
    if (false == isCharging && true == PressShoot())
    {
        CurPlayer->ChangePlayerAnimation("FirePunchCharging");
        isCharging = true;
    }
    if (false == isFire && true == GameEngineInput::IsUp("Shoot"))
    {
        CurPlayer->ChangePlayerAnimation("FirePunchFire");
        isFire = true;
        StartPos = CurPlayer->GetPos();
    }

    if (true == isFire && Timer > 0)
    {
        Timer -= _DeltaTime;
        StartPos += Dir * _DeltaTime * 1000.0f;////??????????????????????????????????????????????????????????????????????????????????????????
        CurPlayer->SetPos(StartPos);
    }
    if (Timer < 0 && false == isAfterAnimation)
    {
        CurPlayer->ChangePlayerAnimation("FirePunchAfter");
        isAfterAnimation = true;
    }

    if (Timer < 0 && true == isAfterAnimation && true == CurPlayer->IsPlayerAnimationEnd())
    {
        isWeaponDone = true;
    }
}
