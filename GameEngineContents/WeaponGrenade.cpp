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
	isBlocked = true;
	MoveSpeed = 700.0f; // �ӽð�
	Gravity = 1.0f;// �ӽð�
	GravityAccel = 0.0f;
	//WindPower = 0.0f;
	//Dmg = 0.0f;
	Timer = 2.0f;// �ӽð�
	Dir = float4::Right;
	//BombScale = { 250,250 };
	WeaponName = "Grenade";

	//WeaponGrenadeInit();
	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // �̹��� �̸� ����or �Լ�ȭ �ʿ�


	AllWeapons[WeaponName] = this;
}

void WeaponGrenade::Update(float _DeltaTime)
{
	if (nullptr == WeaponRender) // ���� ������ �������� �ʴ� ���
	{
		WeaponGrenadeInit();
	}
	if (nullptr == CurPlayer || false == CurPlayer->GetIsMyTurn()) // �÷��̾� �缳��
	{
		FindCurPlayer();
	}

	Firing(_DeltaTime);
	// ���� üũ
	if (Timer < 0 && isExplosion == false)
	{
		MapModifier::MainModifier->CreateHole(GetPos() + WeaponCollision->GetPosition(), 120);
		isExplosion = true;
		WeaponRender->Off();
		WeaponCollision->Off();
		//this->Off();
	}

}

void WeaponGrenade::Firing(float _DeltaTime)
{
	if (false == isFire) // �߻��ϱ� ��
	{
		SetCharge(); // ��¡����Ʈ ���
		PlayerPos = CurPlayer->GetPos();
		Dir = GetShootDir() * Charge;
		SetPos(PlayerPos);
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
			//Dir += {Dir.x, Dir.y + Gravity * _DeltaTime};
			Dir.y = Dir.y + Gravity * _DeltaTime; // dt���� �߷��� ����
			//Dir.Normalize();

			WeaponRender->SetMove(Dir * MoveSpeed * _DeltaTime);
			WeaponCollision->SetMove(Dir * MoveSpeed * _DeltaTime);

			if (Dir.x * CheckCollisionSide().x > 0) // ������ �޶�
			{
				WeaponRender->SetMove(-Dir * MoveSpeed * _DeltaTime);
				WeaponCollision->SetMove(-Dir * MoveSpeed * _DeltaTime);

				Dir.x = -Dir.x * 0.5;										  //  x���� ���������
				Dir.y = -Dir.y * 0.25;
			}

			else if (Dir.x * CheckCollisionSide().x < 0 || CheckCollisionSide().x == 0 && CheckCollisionSide().y > 0)// ������ ���� or �¿� �̵�x
			{
				WeaponRender->SetMove({ 0, -Dir.y * MoveSpeed * _DeltaTime });
				WeaponCollision->SetMove({ 0, -Dir.y * MoveSpeed * _DeltaTime });
				Dir.x = Dir.x * 0.5;
				Dir.y = -Dir.y * 0.25;
			}
		}
	}
}

void WeaponGrenade::SetCharge()
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

	WeaponRender->On();
	WeaponRender->SetPosition(float4::Zero);

	WeaponCollision->On();
	WeaponCollision->SetPosition(float4::Zero);
	isFire = false;
	isExplosion = false;
	Timer = 5.0f;

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

void WeaponGrenade::FindCurPlayer()
{
	std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player);
	for (int i = 0; i < PlayerList.size(); i++)
	{
		if (true == dynamic_cast<Player*>(PlayerList[i])->GetIsMyTurn())
		{
			CurPlayer = dynamic_cast<Player*>(PlayerList[i]);
			ResetWeapon();
			break;
		}
	}
	if (nullptr == CurPlayer)
	{
		MsgAssert("�����÷��̾ ã�� ���߽��ϴ�.");
	}
}