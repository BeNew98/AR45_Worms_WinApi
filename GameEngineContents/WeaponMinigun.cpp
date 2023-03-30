#include "WeaponMinigun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include "MapModifier.h"
#include "Player.h"
#include "SmokeSparkEffect.h"

WeaponMinigun::WeaponMinigun()
{
}

WeaponMinigun::~WeaponMinigun()
{
}

void WeaponMinigun::Start()
{
	// �̴ϰ� �⺻ ����
	WeaponName = "Minigun";
	MoveSpeed = 1200.0f;
	//Dir = float4::Right;
    BombScale = 11;

    MaxDmg = 10;
    MinDmg = 3;

    MaxKnockBackPower = 11;
    MinKnockBackPower = 11;

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���
	//AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Minigun);



	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
    AimingLine->SetImage("AimingLine.bmp");
    AimingLine->SetRotFilter("AimingLineRot.bmp");
	AimingLine->SetScale({ 20,20 });


}

void WeaponMinigun::Update(float _DeltaTime)
{
	while (BulletCount > MinigunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponMinigunInit();
        SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�
        CurPlayer->ChangePlayerAnimation("MinigunAim", static_cast<int>(AimIndex));

	}

    if (false == isFire)
    {
        //SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�
        SetAimFrameIndex();

        if (AimIndex != NextAimIndex && CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON && CurPlayer->GetCurWeapon()->GetWeaponNumber() == WeaponNumber)
        {
            float Ratio = 6 * _DeltaTime;
            AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);
            CurPlayer->ChangePlayerAnimation("MinigunAim", static_cast<int>(AimIndex));
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

void WeaponMinigun::SetAimFrameIndex()
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

bool WeaponMinigun::IsDone()
{
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == MinigunCollision[i]->IsUpdate())
		{
			return false;
		}

	}
	return true;
}

void WeaponMinigun::CheckFiring()
{
	if (false == isFire)
	{
		if (PressShoot()) // �߻�üũ
		{
			isFire = true;
            AimingLine->Off();
            CurPlayer->ChangePlayerAnimation("MinigunFire", static_cast<int>(AimIndex));
		}
		float4 PlayerPos = CurPlayer->GetPos();
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����

	}

}


void WeaponMinigun::Firing(float _DeltaTime)
{
	if (true == isFire)
	{
        GetLevel()->SetCameraPos(MinigunCollision[0]->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half());
		DelayTime -= _DeltaTime;
		if (DelayTime < 0)
		{
			DelayTime = 0.1f;
			for (int i = 0; i < BulletCount; i++)
			{
				if (isShooted[i] == false)
				{
					isShooted[i] = true;
					break;
				}

			}
		}

		for (int i = 0; i < BulletCount; i++)
		{
			if (true == isShooted[i] && true == MinigunCollision[i]->IsUpdate())
			{
				MinigunCollision[i]->SetMove(Dir * _DeltaTime * MoveSpeed);
				if (true == CheckCollision(MinigunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
				{
                    if (isIsFireAnimationDone == false && i == BulletCount - 1 && isWeaponDone == false)
                    {
                        CurPlayer->ChangePlayerAnimation("Idle");
                        isIsFireAnimationDone = true;
                    }
                    SmokeSparkEffect* Smoke = GetLevel()->CreateActor<SmokeSparkEffect>();
                    Smoke->SetPos(MinigunCollision[i]->GetActorPlusPos());
                    Smoke->CreateSmokeSpark(6, 2, BombScale);

					GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();										  // 1. Bomb �ݸ��� ��������
					BombCollision->SetPosition(GetPos() + MinigunCollision[i]->GetPosition());													  // 2. Bomb �ݸ��� �̵�
                    BombCollision->SetScale(float4{ static_cast<float>(BombScale * 2) });

                    AttackPlayerGun(BombCollision, 500);																								  // 3. Bomb�ݸ��� Player Check

					MapModifier::MainModifier->CreateHole(GetPos() + MinigunCollision[i]->GetPosition(), BombScale);							  // 4. ���� �����
					MinigunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
				}
			}
		}
	}

}


void WeaponMinigun::WeaponMinigunInit()
{
	// Minigun�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 25,25 });

	MinigunCollision.push_back(Collision);
	isShooted.push_back(false);
	DelayTime = 0.05f;
}

void WeaponMinigun::ResetWeapon()
{
	isFire = false;
	DelayTime = 0.05f;
	for (int i = 0; i < BulletCount; i++)
	{
		isShooted[i] = false;
		MinigunCollision[i]->SetPosition(float4::Zero);
		MinigunCollision[i]->On();
	}
}