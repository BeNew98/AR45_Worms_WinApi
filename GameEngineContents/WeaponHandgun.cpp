#include "WeaponHandgun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineWindow.h>

#include "MapModifier.h"
#include "Player.h"
#include "SmokeSparkEffect.h"

WeaponHandgun::WeaponHandgun()
{
}

WeaponHandgun::~WeaponHandgun()
{
}

void WeaponHandgun::Start()
{
	// �ڵ�� �⺻ ����
	WeaponName = "Handgun";
	MoveSpeed = 1200.0f;
	//Dir = float4::Right;
	BombScale = 22;

    MaxDmg = 10;
    MinDmg = 3;
    MaxKnockBackPower = 22;
    MinKnockBackPower = 22;

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���

	//AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Handgun);
	SetCurPlayer();


	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
    AimingLine->SetImage("AimingLine.bmp");
    AimingLine->SetRotFilter("AimingLineRot.bmp");
	AimingLine->SetScale({ 20,20 });


}

void WeaponHandgun::Update(float _DeltaTime)
{
    // �Ѿ� ����ŭ init
	while (BulletCount > HandgunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponHandgunInit();
        SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�
	}
    if (false == isFire)
    {
        //SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�
        SetAimFrameIndex();

        if (AimIndex != NextAimIndex && CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON && CurPlayer->GetCurWeapon()->GetWeaponNumber() == WeaponNumber)
        {
            float Ratio = 6 * _DeltaTime;
            AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);
            CurPlayer->ChangePlayerAnimation("HandgunAim", static_cast<int>(AimIndex));
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

    // ��� �Ѿ˹߻�Ǿ� �������� üũ
	if (true == IsDone())
	{
		isWeaponDone = true;
        GetLevel()->SetCameraPos(GetPos() - GameEngineWindow::GetScreenSize().half()); //���� �� Player�� ī�޶� �̵�- �����ʿ�
    }
}

void WeaponHandgun::SetAimFrameIndex()
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

bool WeaponHandgun::IsDone()
{
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == HandgunCollision[i]->IsUpdate())
		{
			return false;
		}

	}
	return true;
}

void WeaponHandgun::CheckFiring()
{
	if (false == isFire)
	{
		if (PressShoot()) // �߻�üũ
		{
			isFire = true;
            AimingLine->Off();
            CurPlayer->ChangePlayerAnimation("HandgunFire", static_cast<int>(AimIndex));

		}
		float4 PlayerPos = CurPlayer->GetPos();
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����

	}

}


void WeaponHandgun::Firing(float _DeltaTime)
{
	if (true == isFire && false == isWeaponDone)
	{
        /*CurPlayer->ChangePlayerAnimation("HandgunFire", static_cast<int>(AimIndex));*/
        GetLevel()->SetCameraPos(HandgunCollision[0]->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half());

		DelayTime -= _DeltaTime;
		if (DelayTime < 0)
		{
			DelayTime = 0.5f;
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
			if (true == isShooted[i] && true == HandgunCollision[i]->IsUpdate())
			{
                if (isIsFireAnimationDone == false && i == BulletCount - 1 && isWeaponDone == false)
                {
                    CurPlayer->ChangePlayerAnimation("Idle");
                    isIsFireAnimationDone = true;
                }

				HandgunCollision[i]->SetMove(Dir * _DeltaTime * MoveSpeed);
				if (true == CheckCollision(HandgunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
				{
                    SmokeSparkEffect* Smoke = GetLevel()->CreateActor<SmokeSparkEffect>();
                    Smoke->SetPos(HandgunCollision[i]->GetActorPlusPos());
                    Smoke->CreateSmokeSpark(6, 2, BombScale);

					GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();								  // 1. Bomb �ݸ��� ��������
					BombCollision->SetPosition(GetPos() + HandgunCollision[i]->GetPosition());											  // 2. Bomb �ݸ��� �̵�
                    BombCollision->SetScale(float4{ static_cast<float>(BombScale) });


                    AttackPlayerGun(BombCollision, 500);																				  // 3. Bomb�ݸ��� Player Check

					MapModifier::MainModifier->CreateHole(GetPos() + HandgunCollision[i]->GetPosition(), BombScale);					  // 4. ���� �����


					HandgunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
				}
			}
		}
	}
}


void WeaponHandgun::WeaponHandgunInit()
{
	// Handgun�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 25,25 });

	HandgunCollision.push_back(Collision);
	isShooted.push_back(false);
	DelayTime = 0.5f;

}

void WeaponHandgun::ResetWeapon()
{
	isFire = false;
	DelayTime = 0.5f;
	for (int i = 0; i < BulletCount; i++)
	{
		isShooted[i] = false;
		HandgunCollision[i]->SetPosition(float4::Zero);
		HandgunCollision[i]->On();
	}
}