#include "WeaponUzi.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
//#include <GameEnginePlatform/GameEngineInput.h>

#include "MapModifier.h"
#include "Player.h"

WeaponUzi::WeaponUzi()
{
}

WeaponUzi::~WeaponUzi()
{
}

void WeaponUzi::Start()
{
	// ���� �⺻ ����
	WeaponName = "Uzi";
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

void WeaponUzi::Update(float _DeltaTime)
{
	while (BulletCount > UziCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponUziInit();
	}

	if (nullptr == CurPlayer || false == CurPlayer->GetIsMyTurn()) // �÷��̾� �缳��
	{
		SetCurPlayer();
		ResetWeapon();
	}

	CheckFiring(); // ����üũ, �߻� üũ
	Firing(_DeltaTime); // �Ѿ��� ������ �ӵ��� ���ư��� �����ϰ� ��

}

void WeaponUzi::CheckFiring()
{
	if (PressShoot()) // �߻�üũ
	{
		for (int i = 0; i < BulletCount; i++)
		{
			if (isShooted[i] == false)
			{
				isShooted[i] = true;
				UziDir[i] = Dir; // �߻�� ���⼳��
				break;
			}
		}
	}
	else // ����üũ
	{
		PlayerPos = CurPlayer->GetPos();
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����
		AimingLine->SetPosition(Dir * 100); // ���ؼ� �̵�
	}

}


void WeaponUzi::Firing(float _DeltaTime)
{
	//bool isRemainBullet = false;
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == isShooted[i] && true == UziCollision[i]->IsUpdate())
		{
			//isRemainBullet = true;
			WeaponMove(UziCollision[i], _DeltaTime, UziDir[i]);

			if (true == WeaponUzi::CheckCollision(UziCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
			{
				MapModifier::MainModifier->CreateHole(GetPos() + UziCollision[i]->GetPosition(), 50);
				UziCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
			}
		}
	}

}


void WeaponUzi::WeaponMove(GameEngineCollision* _Col, float _DeltaTime, float4 _Dir)
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

void WeaponUzi::Render(float _DeltaTime)
{
}

void WeaponUzi::WeaponUziInit()
{
	// Uzi�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 25,25 });

	UziCollision.push_back(Collision);
	isShooted.push_back(false);
	UziDir.push_back(float4::Right);

}

void WeaponUzi::ResetWeapon()
{
	for (int i = 0; i < BulletCount; i++)
	{
		isShooted[i] = false;
		UziDir[i] = float4::Right;
		UziCollision[i]->SetPosition(float4::Zero);
		UziCollision[i]->On();
	}
}