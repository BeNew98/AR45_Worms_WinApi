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
	// ����ź �⺻ ����
	isBlocked = true;
	MoveSpeed = 500.0f;
	Gravity = 0.05f;// �ӽð�
	GravityAccel = 0.0f;
	//WindPower = 0.0f;
	//Dmg = 0.0f;
	Timer = 2.0f;// �ӽð�
	Dir = float4::Right;
	BombScale = { 250,250 };
	WeaponName = "Grenade";

	//WeaponGrenadeInit();
	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp");

	//Explosion = GetLevel()->CreateActor<MapModifier>();
	/*Explosion->SetRadius(BombScale.hix());*/

	AllWeapons[WeaponName] = this;
}

void WeaponGrenade::Update(float _DeltaTime)
{
	if (nullptr == WeaponRender)
	{
		WeaponGrenadeInit();
	}

	if (isEndCharging() == true) // �߻�üũ
	{
		isFire = true;
	}
	else if(false == isFire)// ����üũ
	{
		if (nullptr == CurPlayer || false == CurPlayer->GetIsMyTurn())
		{
			FindCurPlayer();
		}
		PlayerPos = CurPlayer->GetPos();
		Dir = GetShootDir();
		SetPos(PlayerPos);
	}


	if (true == isFire)
	{
		Timer -= _DeltaTime;
		Dir += {Dir.x, Dir.y + Gravity};
		Dir.Normalize();

		WeaponRender->SetMove(Dir * MoveSpeed * _DeltaTime);
		WeaponCollision->SetMove(Dir * MoveSpeed * _DeltaTime);

		if (true == CheckCollision())
		{
			Dir.y = -Dir.y;
		}

		if (Timer < 0)
		{
			//Explosion->SetPos(GetPos() + WeaponCollision->GetPosition()); // ���� ���� ����Ÿ ��ĥ�ϱ�
			MapModifier::MainModifier->CreateHole(GetPos() + WeaponCollision->GetPosition(), 120);
			WeaponRender->Off();
			WeaponCollision->Off();
			this->Off();
		}
	}


}


void WeaponGrenade::ResetWeapon()
{

	WeaponRender->On();
	WeaponRender->SetPosition(float4::Zero);

	WeaponCollision->On();
	WeaponCollision->SetPosition(float4::Zero);
	isFire = false;
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
			break;
		}
	}
	if (nullptr == CurPlayer)
	{
		MsgAssert("�����÷��̾ ã�� ���߽��ϴ�.");
	}
}