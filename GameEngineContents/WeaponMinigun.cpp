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
	MoveSpeed = 1200.0f;
	Dir = float4::Right;
	//float Dmg = 0.0f;
	BombScale = 30;//??

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���
	AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Minigun);



	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
	AimingLine->SetImage("TempBomb.bmp");
	AimingLine->SetScale({ 20,20 });


}

void WeaponMinigun::Update(float _DeltaTime)
{
	while (BulletCount > MinigunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponMinigunInit();
	}

	SetCurPlayer();
	//if (nullptr == CurPlayer || false == CurPlayer->GetIsMyTurn()) // �÷��̾� �缳�� - ���� �ʿ���
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
		}
		float4 PlayerPos = CurPlayer->GetPos();
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����
		AimingLine->SetPosition(Dir * 100); // ���ؼ� �̵�

	}

}


void WeaponMinigun::Firing(float _DeltaTime)
{
	if (true == isFire)
	{
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

					GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();										  // 1. Bomb �ݸ��� ��������
					BombCollision->SetPosition(GetPos() + MinigunCollision[i]->GetPosition());													  // 2. Bomb �ݸ��� �̵�

					AttackPlayer(BombCollision);																								  // 3. Bomb�ݸ��� Player Check

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