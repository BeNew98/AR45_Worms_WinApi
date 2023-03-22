#include "WeaponGrenade.h"
#include "ContentsEnums.h"
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "Player.h"
#include "MapModifier.h"


WeaponGrenade::WeaponGrenade() 
{
}

WeaponGrenade::~WeaponGrenade() 
{
}

void WeaponGrenade::Start()
{
	// ����ź �⺻ ���� -- ����� ���� �ʿ�
	MoveSpeed = 700.0f; // �ӽð�
	Gravity = 1.0f;// �ӽð�
	//Dmg = 0.0f;
	Timer = 2.0f;// �ӽð�
	Dir = float4::Right;
	BombScale = 120;
	WeaponName = "Grenade";

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // �̹��� �̸� ����or �Լ�ȭ �ʿ�

	AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Grenade);

}

void WeaponGrenade::Update(float _DeltaTime)
{
	if (nullptr == WeaponRender) // ���� ������ �������� �ʴ� ���
	{
		WeaponGrenadeInit();
	}
	SetCurPlayer();
	//if (nullptr == CurPlayer || false == CurPlayer->GetIsMyTurn()) // �÷��̾� �缳��
	//{
	//	SetCurPlayer();
	//	ResetWeapon();
	//}

	Firing(_DeltaTime);
	// ���� üũ
	if (Timer < 0 && isExplosion == false)
	{
		GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();
		BombCollision->SetPosition(GetPos() + WeaponCollision->GetPosition());

		AttackPlayer(BombCollision);

		MapModifier::MainModifier->CreateHole(GetPos() + WeaponCollision->GetPosition(), 120);
		isExplosion = true;
		WeaponRender->Off();
		WeaponCollision->Off();
	}

}

void WeaponGrenade::Firing(float _DeltaTime)
{
	if (false == isFire) // �߻��ϱ� ��
	{
		float4 PlayerPos = CurPlayer->GetPos();
		Dir = GetShootDir() * Charge;
		SetPos(PlayerPos);
		if (true == PressShoot())
		{
			SetCharge();// ��¡����Ʈ ���
		}
		if (isEndCharging() == true) // �߻�üũ
		{
			isFire = true;
		}

	}

	else // �߻� ��
	{
		Timer -= _DeltaTime;
		if (false == isExplosion)// && true == isFire)
		{
			Dir.y = Dir.y + Gravity * _DeltaTime; // dt���� �߷��� ����

			WeaponRender->SetMove(Dir * MoveSpeed * _DeltaTime);
			WeaponCollision->SetMove(Dir * MoveSpeed * _DeltaTime);


			float4 CheckedCol = CheckCollisionSide(WeaponCollision);
			if (Dir.x * CheckedCol.x > 0) // ������ �޶�
			{
				WeaponRender->SetMove(-Dir * MoveSpeed * _DeltaTime);
				WeaponCollision->SetMove(-Dir * MoveSpeed * _DeltaTime);

				Dir.x = -Dir.x * 0.5f;										  //  x���� ���������
				Dir.y = -Dir.y * 0.25f;
			}

			else if (Dir.x * CheckedCol.x < 0 || CheckedCol.x == 0 && CheckedCol.y > 0)// ������ ���� or �¿� �̵�x
			{
				WeaponRender->SetMove({ 0, -Dir.y * MoveSpeed * _DeltaTime });
				WeaponCollision->SetMove({ 0, -Dir.y * MoveSpeed * _DeltaTime });
				Dir.x = Dir.x * 0.5f;
				Dir.y = -Dir.y * 0.25f;
			}
		}
	}
}

void WeaponGrenade::SetCharge() // Charging���� �Լ��̸� ����
{
	if (Charge > GetChargeTime())
	{
		return;
	}
	if (GetChargeTime() > 2.0f) // �ִ� 2�� ��¡
	{
		Charge = 2.0f;
	}
	else if (GetChargeTime() < 1.0f)
	{
		Charge = 1.0f;
	}
	else {
		Charge = GetChargeTime();
	}
}

void WeaponGrenade::ResetWeapon()
{

	isFire = false;
	isExplosion = false;
	Timer = 2.0f;
	if (nullptr == WeaponRender)
	{
		return;
	}
	WeaponRender->On();
	WeaponRender->SetPosition(float4::Zero);

	WeaponCollision->On();
	WeaponCollision->SetPosition(float4::Zero);

}

void WeaponGrenade::WeaponGrenadeInit()
{
	WeaponRender = CreateRender(WormsRenderOrder::Weapon);		//����
	WeaponRender->SetImage("Grenade.bmp");
	WeaponRender->SetScale({ 20,40 }); // �ӽ� ���� �� 

	WeaponCollision = CreateCollision(WormsCollisionOrder::Weapon);	//�ݸ���
	WeaponCollision->SetScale(WeaponRender->GetScale());
	isFire = false;
	ResetWeapon();
}
