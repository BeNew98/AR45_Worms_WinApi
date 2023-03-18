#include "WeaponShotgun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>

#include "MapModifier.h"

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
	EffectGravity = false;
	isAnimation = false;
	isBlocked = true;
	isTarget = false;

	MoveSpeed = 600.0f;
	//float Dmg = 0.0f;

	Dir = float4{ 1,0 }.NormalizeReturn();
	//float4 PlayerPos = float4::Zero;
	BombScale = float4{ 100,100 };
	//float4 Scale = float4::Zero;	

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp");
	//WeaponShotgunInit();


	// ���� �ʱ��
	Bomb = GetLevel()->CreateActor<MapModifier>();
	Bomb->SetRadius(BombScale.hix());

	// - ���ؼ� 
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
	AimingLine->SetImage("TempBomb.bmp");
	AimingLine->SetScale({ 20,20 });
	AllWeapons[WeaponName] = this;
}

void WeaponShotgun::Update(float _DeltaTime)
{

	if (0 == ShotGunCollision.size()) // ���Ⱑ �������� �ʾ��� ��� �����ǵ��� ��.
	{
		for (int i = 0;i < BulletCount;i++)
		{
			WeaponShotgunInit();
		}
	}
	

	if (PressShoot())
	{
		for (int i = 0;i < BulletCount;i++)
		{
			if (isShooted[i] == false)
			{
				isShooted[i] = true;
				ShotGunDir[i] = Dir; // �߻�� ���⼳��
				break;
			}
		}
	}
	else
	{
		float4 PlayerPos = (GetLevel()->GetActors(WormsRenderOrder::Player))[0]->GetPos();// �ӽ� index0 PlayerPos
		SetPos(PlayerPos /*- float4{0,30}*/);
		Dir = GetShootDir(); // ���� ����
		// ���ؼ� �̵�
		AimingLine->SetPosition(Dir*100 /*+ float4{ 0,30 }*/);

	}

	for (int i = 0;i < BulletCount;i++)
	{
		if (true == isShooted[i] && true == ShotGunCollision[i]->IsUpdate())
		{
			WeaponMove(ShotGunCollision[i], _DeltaTime, ShotGunDir[i]);

			if (true == WeaponShotgun::CheckCollision(ShotGunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
			{
				MakeBomb(GetPos()+ShotGunCollision[i]->GetPosition()); // ���� ���� ǥ��

				ShotGunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
			}
		}
	}


}

void WeaponShotgun::MakeBomb(float4 _Pos) // �ӽ� - ��ź ������ ��ġ ǥ��
{
	Bomb->SetPos(_Pos);
	Bomb->CreateHole();
}


void WeaponShotgun::WeaponMove(GameEngineCollision* _Col, float _DeltaTime,float4 _Dir)
{
	if (false == _Col->IsUpdate())
	{
		return;
	}

	if (true == EffectGravity)
	{

	}
	else // �߷��� ������ ���� ����.
	{
		_Col->SetMove(_Dir * _DeltaTime * MoveSpeed);
	}
}

void WeaponShotgun::Render(float _DeltaTime)
{

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

void WeaponShotgun::ResetWeapon(float _DeltaTime)
{

}