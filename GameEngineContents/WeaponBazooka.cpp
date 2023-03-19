#include "WeaponBazooka.h"
#include "ContentsEnums.h"
#include "Player.h"
#include "MapModifier.h"

#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>

WeaponBazooka::WeaponBazooka()
{
}

WeaponBazooka::~WeaponBazooka()
{
}

void WeaponBazooka::Start()
{
	WeaponBazookaInit();
	CreatePlayerAnimation();
}

void WeaponBazooka::Update(float _DeltaTime)
{
	firing(_DeltaTime);
	BazookaOn();
	BazAiming();
	Charging();

	if (isExplosion == true && ExplosionAnimation->IsAnimationEnd() == true)
	{
		ExplosionAnimation->ChangeAnimation("Idle");
		ExplosionAnimation->Off();

		CurPlayer->ChangePlayerAnimation("BazOff");
		isExplosion = false;
	}
}

void WeaponBazooka::Render(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("ChangePlayer"))
	{
		CurPlayer->ChangePlayerAnimation("BazOff");
		ResetWeapon();
		isAttack = false;
	}
	//HDC _hdc = GameEngineWindow::GetDoubleBufferImage()->GetImageDC();
	//Rectangle(_hdc, static_cast<int>(WeaponCollision->GetActorPlusPos().x) - static_cast<int>(WeaponCollision->GetScale().hx()) - static_cast<int>(GetLevel()->GetCameraPos().x),
	//				static_cast<int>(WeaponCollision->GetActorPlusPos().y) - static_cast<int>(WeaponCollision->GetScale().hy()) - static_cast<int>(GetLevel()->GetCameraPos().y),
	//				static_cast<int>(WeaponCollision->GetActorPlusPos().x) + static_cast<int>(WeaponCollision->GetScale().hx()) - static_cast<int>(GetLevel()->GetCameraPos().x),
	//				static_cast<int>(WeaponCollision->GetActorPlusPos().y) + static_cast<int>(WeaponCollision->GetScale().hy()) - static_cast<int>(GetLevel()->GetCameraPos().y));
}

void WeaponBazooka::WeaponBazookaInit()
{
	WeaponRender =  CreateRender("bazooka.bmp", static_cast<int>(WormsRenderOrder::Weapon));
	WeaponCollision = CreateCollision(static_cast<int>(WormsCollisionOrder::Weapon));

	WeaponRender->SetRotFilter("bazookaRot.bmp");

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp");

	WeaponRender->SetPosition({ 500, 200 }); //�ӽ� ������
	WeaponRender->SetScaleToImage(); //�ӽ� ������
	WeaponRender->Off();

	WeaponCollision->SetPosition(WeaponRender->GetPosition());
	WeaponCollision->SetScale(WeaponRender->GetScale());
	WeaponCollision->Off();

	WeaponName = "Bazooka";

	BazookaExplosion = GetLevel()->CreateActor<MapModifier>();
	BazookaExplosion->SetRadius(50);

	ExplosionAnimation = CreateRender("circle50.bmp", WormsRenderOrder::Weapon);
	ExplosionAnimation->CreateAnimation({ .AnimationName = "Explosion", .ImageName = "circle50.bmp", .Start = 0, .End =  8, .InterTime = 0.03f , .Loop =false });
	ExplosionAnimation->CreateAnimation({ .AnimationName = "Idle", .ImageName = "circle50.bmp", .Start = 0, .End =  0, .InterTime = 0.05f , .Loop =false });
	ExplosionAnimation->SetScale({ 100, 100 });
	ExplosionAnimation->Off();

	Gravity = 0.0f; //�ӽ� ������
	GravityAccel = 0.0f; //�ӽ� ������

	MoveSpeed = 1500.0f; //�ӽ� ������

	EffectGravity = true;
	isAnimation = true;
	isBlocked = true;
	isTarget = false;

	TimeCount = 0;

	std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player);
	
	ChargingRenderInit();
	//�÷��̾� �ٲ� ������ CurPlayer �ٲ㼭 ����
	SetCurPlayer();
}

void WeaponBazooka::CreatePlayerAnimation()
{
	std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player);

	for (int i = 0; i < PlayerList.size(); i++)
	{
		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Left_bazAim", "bazAimLeft.bmp", 0, 31, 0.1f, false);
		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Right_bazAim", "bazAimRight.bmp", 0, 31, 0.1f, false);

		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Left_bazOff", "bazOffLeft.bmp", 0, 6, 0.1f, false);
		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Right_bazOff", "bazOffRight.bmp", 0, 6, 0.1f, false);

		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Left_bazOn", "bazOnLeft.bmp", 0, 7, 0.075f, false);
		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Right_bazOn", "bazOnRight.bmp", 0, 7, 0.075f, false);
	}
}


void WeaponBazooka::Charging()
{
	if (CurPlayer->GetPlayerState() != PlayerState::IDLE)
	{
		return;
	}

	if (isAiming == false)
	{
		return;
	}

	if (GameEngineInput::IsPress("Shoot") == true)
	{
		/* timer �� �ð��� ���, �ð��� �°� ��¡������ ���� ( 0 ~ 1 ���� �ð���� ) => ( ��¡���� = ������¡�ð� / �ִ���¡�ð� ) */
		/* ������ ��� �ɼ��� �߻� �Ÿ��� ����� (�߻�ӵ��� ������) */
		CurPlayer->SetCanIMove(false);
		ChargingRenderOn();
		CurPlayer->SetPlayerAnimationFrame(Bazindex);
	}
}

void WeaponBazooka::firing(float _DeltaTime) //�߻�
{
	if (GameEngineInput::IsKey("Shoot") == false)
	{
		return;
	}

	if (CurPlayer->GetPlayerState() != PlayerState::IDLE)
	{
		return;
	}

	if (isAiming == true && isEndCharging() == true)
	{
		ChargingRenderOff();
		isFire = true;
	}

	if (isFire == false)
	{
		return;
	}

	std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player);

	if (isSet == false)
	{

		for (int i = 0; i < PlayerList.size(); i++)
		{
			if (true == dynamic_cast<Player*>(PlayerList[i])->GetIsMyTurn())
			{
				WeaponRender->SetPosition(PlayerList[i]->GetPos());
				WeaponCollision->SetPosition(PlayerList[i]->GetPos());
				break;
			}
		}

		Dir = ShootDir;

		WeaponRender->On();
		WeaponCollision->On();
		isSet = true;
	}

	Gravity = 0.005f;

	Dir += {Dir.x, Dir.y + Gravity};
	Dir.Normalize();

	WeaponRender->SetAngle(-Dir.GetAnagleDeg() - 45);
	WeaponRender->SetMove(Dir * MoveSpeed * _DeltaTime);
	WeaponCollision->SetMove(Dir * MoveSpeed * _DeltaTime);

	if (CheckCollision() == true)
	{
		Explosion();
		ResetWeapon();
	}
}

void WeaponBazooka::Explosion() //����
{
	if (CheckCollision() == false /*�߻�Ǿ����� �ƴ����� bool������ ���� ���ǿ� �־����*/)
	{
		return;
	}

	else
	{
		//Dmg��ŭ Player�� HP�� ����, ���߹ݰ��� ���� �𿩾���, �˹��� ���� ����, 
		ExplosionAnimation->SetPosition(WeaponRender->GetPosition());
		ExplosionAnimation->On();
		ExplosionAnimation->ChangeAnimation("Explosion", 0);

		BazookaExplosion->SetPos(WeaponRender->GetPosition());
		BazookaExplosion->CreateHole();

		isAttack = true;
		isExplosion = true;

		CurPlayer->SetCanIMove(true);
	}
}

void WeaponBazooka::ChangeBazReadyAnimation()
{

}

void WeaponBazooka::BazookaOn()
{
	if (CurPlayer->GetPlayerState() == PlayerState::IDLE && isBazOn == false && isAttack == false)
	{
		TimeCounting();
	}

	if (TimeCount >= 1.0f && isBazOn == false)
	{
		CurPlayer->ChangePlayerAnimation("BazOn");
		TimeCount = 0;
		isBazOn = true;
	}
}

void WeaponBazooka::ResetWeapon()
{
	std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player);

	//�÷��̾� �ٲ� ������ CurPlayer �ٲ㼭 ����
	SetCurPlayer();

	//��ġ �缳��
	WeaponRender->SetPosition(CurPlayer->GetPos());
	WeaponCollision->SetPosition(CurPlayer->GetPos());

	Gravity = 0.0f; //�ӽ� ������
	GravityAccel = 0.0f; //�ӽ� ������
	WeaponRender->Off();
	WeaponCollision->Off();

	isBazOn = false;
	isSet = false;
	isFire = false;
	isAiming = false;

	Bazindex = 0;

	ShootDir = { 0,0 };
	StartDir = { 0,0 };

	TimeCount = 0;
}

void WeaponBazooka::BazAiming()
{
	if (CurPlayer->GetPlayerState() != PlayerState::IDLE)
	{
		return;
	}

	if (isAiming == true && GameEngineInput::IsPress("Shoot") == true)
	{
		return;
	}

	//����ī�� ������ ����� ������ 0.5�ʵڿ� �������� �ִϸ��̼����� ����
	if (isBazOn == true && CurPlayer->IsPlayerAnimationEnd() == true && isAiming == false)
	{
		TimeCounting();
		CurPlayer->SetPlayerAnimationFrame(6);

		if (TimeCount > 0.5f) 
		{
			isAiming = true;
			CurPlayer->ChangePlayerAnimation("bazAim", CurIndex);
			TimeCount = 0;
		}
	}

	//�������� ���ϸ��̼��� ���۵Ǹ�, ����Ű�� ���� �������� ����
	else if (isAiming == true)
	{
		ShootDir = GetShootDir();
		float Angle = ShootDir.GetAnagleDeg();

		if (ShootDir.x > 0 && Angle > 270)
		{
			Angle = Angle - 360;
		}

		else if (ShootDir.x < 0)
		{
			Angle = 180 - Angle;
		}

		Bazindex = Angle / 5 + 15;

		if (Bazindex < 0)
		{
			Bazindex = 0;
		}

		if(Bazindex == CurIndex)
		{
			CurPlayer->SetPlayerAnimationFrame(Bazindex);
		}

		else if (Bazindex > CurIndex)
		{
			TimeCounting();

			if (TimeCount >= 0.01f)
			{
				++CurIndex;
				CurPlayer->SetPlayerAnimationFrame(CurIndex);
				TimeCount = 0;
			}
		}
		else if (Bazindex < CurIndex)
		{
			TimeCounting();

			if (TimeCount >= 0.01f)
			{
				--CurIndex;
				CurPlayer->SetPlayerAnimationFrame(CurIndex);
				TimeCount = 0;
			}
		}
	}

	if (GameEngineInput::IsDown("MoveRight") || GameEngineInput::IsDown("MoveLeft"))
	{
		isAiming = false;
		isBazOn = false;
		CurIndex = 16;
	}
}

void WeaponBazooka::ChargingRenderInit()
{
	ChargingRender.resize(16);
	size_t Size = ChargingRender.size();

	for (int i = 0; i < Size; i++)
	{
		ChargingRender[i] = CreateRender("Charging.bmp", WormsRenderOrder::Weapon);
		ChargingRender[i]->SetFrame(i);
		ChargingRender[i]->SetScale({ 64, 64 });
		ChargingRender[i]->Off();
	}
}

void WeaponBazooka::ChargingRenderOn()
{
	if (CountingIndex >= ChargingRender.size())
	{
		return;
	}

	size_t Size = ChargingRender.size();
	float4 PlayerPos = CurPlayer->GetPos();

	float4 StartPos = PlayerPos + float4{ ShootDir.x * 5, ShootDir.y * 5 } + float4{ 0, -8 };
	
	TimeCounting();
	
	if (TimeCount_2 > 0.05)
	{
		ChargingRender[CountingIndex]->SetPosition(StartPos + float4{ ShootDir.x * 3 * (CountingIndex + 1), ShootDir.y * 3 * (CountingIndex + 1) });
		ChargingRender[CountingIndex]->On();

		TimeCount_2 = 0;

		CountingIndex++;
	}
}

void WeaponBazooka::ChargingRenderOff()
{
	CountingIndex = 0;
	size_t Size = ChargingRender.size();

	for (int i = 0; i < Size; i++)
	{
		ChargingRender[i]->Off();
	}
}