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
	isBlocked = true;
	MoveSpeed = 600.0f;
	//float Dmg = 0.0f;
	Dir = float4::Right;
	BombScale = float4{ 100,100 };
	//float4 Scale = float4::Zero;	

	
	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���



	// ���� �ʱ�� - �����ʿ� : WeaponClass���� Bomb�� �����ؾ� ���� ���͸� ������ �ʾƵ� ��
	//Explosion = GetLevel()->CreateActor<MapModifier>();
	//Explosion->SetRadius(BombScale.hix());

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
	


	CheckFiring(); // ����üũ, �߻� üũ
	Firing(_DeltaTime); // �Ѿ��� ������ �ӵ��� ���ư��� �����ϰ� ��

}

void WeaponShotgun::CheckFiring()
{
	if (PressShoot()) // �߻�üũ
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
	else // ����üũ
	{
		if (nullptr == CurPlayer || false == CurPlayer->GetIsMyTurn())
		{
			FindCurPlayer();
		}
		PlayerPos = CurPlayer->GetPos();
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����
		AimingLine->SetPosition(Dir * 100); // ���ؼ� �̵�
	}

}

void WeaponShotgun::FindCurPlayer()
{
	std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player); 
	for (int i = 0; i < PlayerList.size(); i++)
	{
		if (true == dynamic_cast<Player*>(PlayerList[i])->GetIsMyTurn())
		{
			CurPlayer = dynamic_cast<Player*>(PlayerList[i]);
			break;
		}
	}
	if (nullptr == CurPlayer)
	{
		MsgAssert("�����÷��̾ ã�� ���߽��ϴ�.");
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
				MapModifier::MainModifier->CreateHole(GetPos() + ShotGunCollision[i]->GetPosition(), 50);
				//Explosion->SetPos(GetPos() + ShotGunCollision[i]->GetPosition()); // ���� ���� ����Ÿ ��ĥ�ϱ�
				//Explosion->CreateHole();
				ShotGunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
			}
		}
	}

	// ���� �ʿ� : ������ �ִϸ��̼Ǳ��� Weapon�� On�Ǿ��־�� �ϴ��� ���� �ʿ�
	//if (false == isRemainBullet) // �����ִ� ��ź�� ���ٸ� WeaponOff
	//{
	//	this->Off();
	//	return;
	//}

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