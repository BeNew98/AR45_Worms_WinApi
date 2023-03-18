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

	
	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���



	// ���� �ʱ�� - �����ʿ� : WeaponClass���� Bomb�� �����ؾ� ���� ���͸� ������ �ʾƵ� ��
	Explosion = GetLevel()->CreateActor<MapModifier>();
	Explosion->SetRadius(BombScale.hix());

	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
	AimingLine->SetImage("TempBomb.bmp");
	AimingLine->SetScale({ 20,20 });


	AllWeapons[WeaponName] = this;
}

void WeaponShotgun::Update(float _DeltaTime)
{
	while (BulletCount > ShotGunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponShotgunInit();
	}
	


	CheckFiring();
	Firing(_DeltaTime);

}

void WeaponShotgun::CheckFiring()
{
	if (PressShoot()) // �߻�
	{
		for (int i = 0; i < BulletCount; i++)
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
		float4 PlayerPos = float4::Zero;// �ӽ� index0 PlayerPos
		std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player); // ȿ�������� ���� ����??
		for (int i = 0; i < PlayerList.size(); i++)
		{
			if (true == dynamic_cast<Player*>(PlayerList[i])->GetIsMyTurn())
			{
				PlayerPos = PlayerList[i]->GetPos();
				break;
			}
		}
		if (true == PlayerPos.IsZero())
		{
			MsgAssert("���� ������ ������ �÷��̾ �����ϴ�.");
			return;
		}
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����
		AimingLine->SetPosition(Dir * 100); // ���ؼ� �̵�
	}

}


void WeaponShotgun::Firing(float _DeltaTime)
{
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == isShooted[i] && true == ShotGunCollision[i]->IsUpdate())
		{
			WeaponMove(ShotGunCollision[i], _DeltaTime, ShotGunDir[i]);

			if (true == WeaponShotgun::CheckCollision(ShotGunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
			{
				Explosion->SetPos(GetPos() + ShotGunCollision[i]->GetPosition()); // ���� ���� ����Ÿ ��ĥ�ϱ�
				Explosion->CreateHole();
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
	for (int i = 0; i < BulletCount; i++)
	{
		isShooted[i] = false;
		ShotGunDir[i] = float4::Right;
		ShotGunCollision[i]->SetPosition(float4::Zero);
		ShotGunCollision[i]->On();
	}
}