#include "WeaponHandgun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
//#include <GameEnginePlatform/GameEngineInput.h>

#include "MapModifier.h"
#include "Player.h"

WeaponHandgun::WeaponHandgun()
{
}

WeaponHandgun::~WeaponHandgun()
{
}

void WeaponHandgun::Start()
{
	// ���� �⺻ ����
	WeaponName = "Handgun";
	EffectGravity = false;
	isBlocked = true;
	MoveSpeed = 600.0f;
	//float Dmg = 0.0f;
	Dir = float4::Right;


	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���



	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
	AimingLine->SetImage("TempBomb.bmp");
	AimingLine->SetScale({ 20,20 });


	AllWeapons[WeaponName] = this;
}

void WeaponHandgun::Update(float _DeltaTime)
{
	while (BulletCount > HandgunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponHandgunInit();
	}

	if (nullptr == CurPlayer || false == CurPlayer->GetIsMyTurn()) // �÷��̾� �缳��
	{
		SetCurPlayer();
		ResetWeapon();
	}

	CheckFiring(); // ����üũ, �߻� üũ
	Firing(_DeltaTime); // �Ѿ��� ������ �ӵ��� ���ư��� �����ϰ� ��
	if (true == IsDone())
	{
		isWeaponDone = true;
	}
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
		}
		PlayerPos = CurPlayer->GetPos();
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����
		AimingLine->SetPosition(Dir * 100); // ���ؼ� �̵�

	}

}


void WeaponHandgun::Firing(float _DeltaTime)
{
	if (true == isFire)
	{
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
				HandgunCollision[i]->SetMove(Dir * _DeltaTime * MoveSpeed);
				if (true == CheckCollision(HandgunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
				{
					MapModifier::MainModifier->CreateHole(GetPos() + HandgunCollision[i]->GetPosition(), 11);
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
	//HandgunDir.push_back(float4::Right);
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