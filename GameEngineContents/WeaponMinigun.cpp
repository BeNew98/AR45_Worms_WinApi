#include "WeaponMinigun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
//#include <GameEnginePlatform/GameEngineInput.h>

#include "MapModifier.h"
#include "Player.h"

WeaponMinigun::WeaponMinigun()
{
}

WeaponMinigun::~WeaponMinigun()
{
}

void WeaponMinigun::Start()
{
	// ���� �⺻ ����
	WeaponName = "Minigun";
	//EffectGravity = false;
	//isBlocked = true;
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

void WeaponMinigun::Update(float _DeltaTime)
{
	while (BulletCount > MinigunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponMinigunInit();
	}

	if (nullptr == CurPlayer || false == CurPlayer->GetIsMyTurn()) // �÷��̾� �缳��
	{
		SetCurPlayer();
		ResetWeapon();
	}

	CheckFiring(); // ����üũ, �߻� üũ
	Firing(_DeltaTime); // �Ѿ��� ������ �ӵ��� ���ư��� �����ϰ� ��

}

void WeaponMinigun::CheckFiring()
{
	if (PressShoot()) // �߻�üũ
	{
		for (int i = 0; i < BulletCount; i++)
		{
			if (isShooted[i] == false)
			{
				isShooted[i] = true;
				MinigunDir[i] = Dir; // �߻�� ���⼳��
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


void WeaponMinigun::Firing(float _DeltaTime)
{
	//bool isRemainBullet = false;
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == isShooted[i] && true == MinigunCollision[i]->IsUpdate())
		{
			//isRemainBullet = true;
			WeaponMove(MinigunCollision[i], _DeltaTime, MinigunDir[i]);

			if (true == WeaponMinigun::CheckCollision(MinigunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
			{
				MapModifier::MainModifier->CreateHole(GetPos() + MinigunCollision[i]->GetPosition(), 50);
				MinigunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
			}
		}
	}

}


void WeaponMinigun::WeaponMove(GameEngineCollision* _Col, float _DeltaTime, float4 _Dir)
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

void WeaponMinigun::Render(float _DeltaTime)
{
}

void WeaponMinigun::WeaponMinigunInit()
{
	// Minigun�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 25,25 });

	MinigunCollision.push_back(Collision);
	isShooted.push_back(false);
	MinigunDir.push_back(float4::Right);

}

void WeaponMinigun::ResetWeapon()
{
	for (int i = 0; i < BulletCount; i++)
	{
		isShooted[i] = false;
		MinigunDir[i] = float4::Right;
		MinigunCollision[i]->SetPosition(float4::Zero);
		MinigunCollision[i]->On();
	}
}