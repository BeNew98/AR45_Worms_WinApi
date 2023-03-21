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

}

void WeaponHandgun::CheckFiring()
{
	if (false == isFire)
	{
		if (PressShoot()) // �߻�üũ
		{
			isFire = true;
			//for (int i = 0; i < BulletCount; i++)
			//{
			//	if (isShooted[i] == false)
			//	{
			//		isShooted[i] = true;
			//		HandgunDir[i] = Dir; // �߻�� ���⼳��
			//		break;
			//	}
			//}
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
		//bool isRemainBullet = false;
		for (int i = 0; i < BulletCount; i++)
		{
			if (true == isShooted[i] && true == HandgunCollision[i]->IsUpdate())
			{
				//isRemainBullet = true;
				WeaponMove(HandgunCollision[i], _DeltaTime, Dir);

				if (true == WeaponHandgun::CheckCollision(HandgunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
				{
					MapModifier::MainModifier->CreateHole(GetPos() + HandgunCollision[i]->GetPosition(), 11);
					HandgunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
				}
			}
		}
	}

}


void WeaponHandgun::WeaponMove(GameEngineCollision* _Col, float _DeltaTime, float4 _Dir)
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

void WeaponHandgun::Render(float _DeltaTime)
{
}

void WeaponHandgun::WeaponHandgunInit()
{
	// Handgun�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 25,25 });

	HandgunCollision.push_back(Collision);
	isShooted.push_back(false);
	//HandgunDir.push_back(float4::Right);

}

void WeaponHandgun::ResetWeapon()
{
	isFire = false;
	for (int i = 0; i < BulletCount; i++)
	{
		isShooted[i] = false;
		//HandgunDir[i] = float4::Right;
		HandgunCollision[i]->SetPosition(float4::Zero);
		HandgunCollision[i]->On();
	}
}