#include "WeaponShotgun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>

#include "MapModifier.h"
#include "Player.h"
#include "SmokeSparkEffect.h"

WeaponShotgun::WeaponShotgun()
{
}

WeaponShotgun::~WeaponShotgun()
{
}

void WeaponShotgun::Start()
{
	// ���� �⺻ ����
	WeaponName = "Shotgun";
	MoveSpeed = 1000.0f;
	//float Dmg = 0.0f;
	Dir = float4::Right;
	BombScale = 50;

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���

	//AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Shotgun);


	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
    AimingLine->SetImage("AimingLine.bmp");
    AimingLine->SetRotFilter("AimingLineRot.bmp");
	AimingLine->SetScale({ 20,20 });

}

void WeaponShotgun::Update(float _DeltaTime)
{
	while (BulletCount > ShotGunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponShotgunInit();
	}
    if (false == isFire)
    {
        SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�
        SetAimFrameIndex();

        if (AimIndex != NextAimIndex && CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON && CurPlayer->GetCurWeapon()->GetWeaponNumber() == WeaponNumber)
        {
            float Ratio = 6 * _DeltaTime;
            AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);
            CurPlayer->ChangePlayerAnimation("ShotgunAim", static_cast<int>(AimIndex));
            AimingLine->On();
            AimingLine->SetPosition(Dir * 200); // ���ؼ� �̵�
            if (Dir.x > 0)
            {
                AimingLine->SetAngle(Dir.GetAnagleDeg());
            }
            else
            {
                AimingLine->SetAngle(-Dir.GetAnagleDeg());
            }
        }
        else
        {
            AimingLine->Off();
        }
        CheckFiring(); // ����üũ, �߻� üũ
    }
	Firing(_DeltaTime); // �Ѿ��� ������ �ӵ��� ���ư��� �����ϰ� ��

	if (true == IsDone())
	{
		isWeaponDone = true;
        GetLevel()->SetCameraPos(GetPos() - GameEngineWindow::GetScreenSize().half()); //���� �� Player�� ī�޶� �̵�- �����ʿ�
	}

}

void WeaponShotgun::SetAimFrameIndex()
{
    float Angle = Dir.GetAnagleDeg();


    int NewIndex = 0;
    if (Dir.x > 0 && Angle > 270)
    {
        Angle = Angle - 360;
    }

    else if (Dir.x < 0)
    {
        Angle = 180 - Angle;
    }

    NewIndex = Angle / 5 + 15;

    if (NewIndex < 0)
    {
        NewIndex = 0;
    }
    NextAimIndex = NewIndex;

}

bool WeaponShotgun::IsDone()
{
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == ShotGunCollision[i]->IsUpdate())
		{
			return false;
		}

	}
	return true;
}

void WeaponShotgun::CheckFiring()
{
	if (true == GameEngineInput::IsDown("Shoot")) // �߻�üũ
	{
		for (int i = 0; i < BulletCount; i++)
		{
			if (isShooted[i] == false)
			{
                AimingLine->Off();
				isFire = true;
				isShooted[i] = true;
				ShotGunDir[i] = Dir; // �߻�� ���⼳��
                CurPlayer->ChangePlayerAnimation("HandgunFire", static_cast<int>(AimIndex));
                isIsFireAnimationDone = true;
                break;
			}
		}
	}
	else // ����üũ
	{
		float4 PlayerPos = CurPlayer->GetPos();
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����
	}

}


void WeaponShotgun::Firing(float _DeltaTime)
{
	//bool isRemainBullet = false;
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == isShooted[i] && true == ShotGunCollision[i]->IsUpdate())
		{
            if (isIsFireAnimationDone == false && i == BulletCount - 1 && isWeaponDone == false)
            {
                CurPlayer->ChangePlayerAnimation("Idle");
                isIsFireAnimationDone = true;
            }

            GetLevel()->SetCameraPos(ShotGunCollision[i]->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half());
			//isRemainBullet = true;
			WeaponMove(ShotGunCollision[i], _DeltaTime, ShotGunDir[i]);

			if (true == WeaponShotgun::CheckCollision(ShotGunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
			{
                SmokeSparkEffect* Smoke = GetLevel()->CreateActor<SmokeSparkEffect>();
                Smoke->SetPos(ShotGunCollision[i]->GetActorPlusPos());
                Smoke->CreateSmokeSpark(6, 2, BombScale);

				GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();				// 1. Bomb �ݸ��� ��������
				BombCollision->SetPosition(GetPos() + ShotGunCollision[i]->GetPosition());							// 2. Bomb �ݸ��� �̵�

				AttackPlayer(BombCollision);																		// 3. Bomb�ݸ��� Player Check

				MapModifier::MainModifier->CreateHole(GetPos() + ShotGunCollision[i]->GetPosition(), BombScale);	// 4. ���� �����

				ShotGunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
                if (i+1 < BulletCount)
                {
                    GetLevel()->SetCameraPos(GetPos() - GameEngineWindow::GetScreenSize().half());
                    isFire = false;
                }
			}
		}
	}

}


void WeaponShotgun::WeaponMove(GameEngineCollision* _Col, float _DeltaTime,float4 _Dir)
{
	if (false == _Col->IsUpdate())
	{
		return;
	}
	else // �߷��� ������ ���� ����.
	{
		_Col->SetMove(_Dir * _DeltaTime * MoveSpeed);
	}
}


void WeaponShotgun::WeaponShotgunInit()
{
	// ShotGun�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 25,25 });

	ShotGunCollision.push_back(Collision);
	isShooted.push_back(false);
	ShotGunDir.push_back(float4::Right);

}

void WeaponShotgun::ResetWeapon()
{
	for (int i = 0; i < BulletCount; i++)
	{
		isShooted[i] = false;
		ShotGunDir[i] = float4::Right;
		ShotGunCollision[i]->SetPosition(float4::Zero);
		ShotGunCollision[i]->On();
	}
}