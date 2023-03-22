#include "WeaponShotgun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
//#include <GameEnginePlatform/GameEngineInput.h>

#include "MapModifier.h"
#include "Player.h"

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
	MoveSpeed = 600.0f;
	//float Dmg = 0.0f;
	Dir = float4::Right;
	BombScale = 50;

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���

	AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Shotgun);


	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
	AimingLine->SetImage("TempBomb.bmp");
	AimingLine->SetScale({ 20,20 });

}

void WeaponShotgun::Update(float _DeltaTime)
{
	while (BulletCount > ShotGunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponShotgunInit();
	}
	
	SetCurPlayer();
	//if (nullptr == CurPlayer || false == CurPlayer->GetIsMyTurn()) // �÷��̾� �缳�� - �����ʿ�
	//{
	//	SetCurPlayer();
	//	ResetWeapon();
	//}

	CheckFiring(); // ����üũ, �߻� üũ
	Firing(_DeltaTime); // �Ѿ��� ������ �ӵ��� ���ư��� �����ϰ� ��

	if (true == IsDone())
	{
		isWeaponDone = true;
	}

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
	if (PressShoot()) // �߻�üũ
	{
		for (int i = 0; i < BulletCount; i++)
		{
			if (isShooted[i] == false)
			{
				isFire = true;
				isShooted[i] = true;
				ShotGunDir[i] = Dir; // �߻�� ���⼳��
				break;
			}
		}
	}
	else // ����üũ
	{
		float4 PlayerPos = CurPlayer->GetPos();
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����
		AimingLine->SetPosition(Dir * 100); // ���ؼ� �̵�
	}

}


void WeaponShotgun::Firing(float _DeltaTime)
{
	//bool isRemainBullet = false;
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == isShooted[i] && true == ShotGunCollision[i]->IsUpdate())
		{
			//isRemainBullet = true;
			WeaponMove(ShotGunCollision[i], _DeltaTime, ShotGunDir[i]);

			if (true == WeaponShotgun::CheckCollision(ShotGunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
			{
				GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();				// 1. Bomb �ݸ��� ��������
				BombCollision->SetPosition(GetPos() + ShotGunCollision[i]->GetPosition());							// 2. Bomb �ݸ��� �̵�

				AttackPlayer(BombCollision);																		// 3. Bomb�ݸ��� Player Check

				MapModifier::MainModifier->CreateHole(GetPos() + ShotGunCollision[i]->GetPosition(), BombScale);	// 4. ���� �����

				ShotGunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
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