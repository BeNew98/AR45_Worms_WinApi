#include "WeaponHandgun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineWindow.h>

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
	MoveSpeed = 600.0f;
	Dir = float4::Right;
	//float Dmg = 0.0f;
	BombScale = 11;

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���

	AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Handgun);
	SetCurPlayer();


	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
	AimingLine->SetImage("TempBomb.bmp");
	AimingLine->SetScale({ 20,20 });


}

void WeaponHandgun::Update(float _DeltaTime)
{
    // �Ѿ� ����ŭ init
	while (BulletCount > HandgunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponHandgunInit();
	}
	SetCurPlayer(); // �÷��̾� ��ȯ��ư ������ �߰�

	CheckFiring(); // ����üũ, �߻� üũ
	Firing(_DeltaTime); // �Ѿ��� ������ �ӵ��� ���ư��� �����ϰ� ��

    // ��� �Ѿ˹߻�Ǿ� �������� üũ
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
		float4 PlayerPos = CurPlayer->GetPos();
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����
		AimingLine->SetPosition(Dir * 100); // ���ؼ� �̵�

	}

}


void WeaponHandgun::Firing(float _DeltaTime)
{
	if (true == isFire)
	{

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
				HandgunCollision[i]->SetMove(Dir * _DeltaTime * MoveSpeed);
				if (true == CheckCollision(HandgunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
				{
					GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();								  // 1. Bomb �ݸ��� ��������
					BombCollision->SetPosition(GetPos() + HandgunCollision[i]->GetPosition());											  // 2. Bomb �ݸ��� �̵�

					AttackPlayer(BombCollision);																						  // 3. Bomb�ݸ��� Player Check

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