#include "WeaponUzi.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineWindow.h>

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
	MoveSpeed = 1200.0f;
	//float Dmg = 0.0f;
	Dir = float4::Right;
	BombScale = 11;//?

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���

	AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Uzi);


	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
	AimingLine->SetImage("TempBomb.bmp");
	AimingLine->SetScale({ 20,20 });


}

void WeaponUzi::Update(float _DeltaTime)
{
	while (BulletCount > UziCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponUziInit();
	}
	SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�

	CheckFiring(); // ����üũ, �߻� üũ
	Firing(_DeltaTime); // �Ѿ��� ������ �ӵ��� ���ư��� �����ϰ� ��

	if (true == IsDone())
	{
		isWeaponDone = true;
	}

}

bool WeaponUzi::IsDone()
{
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == UziCollision[i]->IsUpdate())
		{
			return false;
		}

	}
	return true;
}

void WeaponUzi::CheckFiring()
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


void WeaponUzi::Firing(float _DeltaTime)
{

	if (true == isFire)
	{
        GetLevel()->SetCameraPos(UziCollision[0]->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half());
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
			if (true == isShooted[i] && true == UziCollision[i]->IsUpdate())
			{
				UziCollision[i]->SetMove(Dir * _DeltaTime * MoveSpeed);
				if (true == CheckCollision(UziCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
				{
					GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();
					BombCollision->SetPosition(GetPos() + UziCollision[i]->GetPosition());

					AttackPlayer(BombCollision);

					MapModifier::MainModifier->CreateHole(GetPos() + UziCollision[i]->GetPosition(), BombScale);
					UziCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
				}
			}
		}
	}

}


void WeaponUzi::WeaponUziInit()
{
	// Uzi�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 25,25 });

	UziCollision.push_back(Collision);
	isShooted.push_back(false);
	DelayTime = 0.1f;

}

void WeaponUzi::ResetWeapon()
{
	isFire = false;
	DelayTime = 0.1f;
	for (int i = 0; i < BulletCount; i++)
	{
		isShooted[i] = false;
		UziCollision[i]->SetPosition(float4::Zero);
		UziCollision[i]->On();
	}
}