#include "WeaponBazooka.h"
#include "ContentsEnums.h"
#include "Player.h"

#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEnginePlatform/GameEngineInput.h>
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
}

void WeaponBazooka::Update(float _DeltaTime)
{

	firing(_DeltaTime);
	BazookaOn();
}

void WeaponBazooka::Render(float _DeltaTime)
{
	if (GameEngineInput::IsDown("ChangePlayer"))
	{
		CurPlayer->ChangePlayerAnimation("BazOff");
		SetCurPlayer();
	}
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

	Gravity = 0.0f; //�ӽ� ������
	GravityAccel = 0.0f; //�ӽ� ������

	MoveSpeed = 1000.0f; //�ӽ� ������

	EffectGravity = true;
	isAnimation = true;
	isBlocked = true;
	isTarget = false;

	std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player);

	//�÷��̾� �ٲ� ������ CurPlayer �ٲ㼭 ����
	SetCurPlayer();
	CreatePlayerAnimation();

}

void WeaponBazooka::CreatePlayerAnimation()
{
	std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player);

	for (int i = 0; i < PlayerList.size(); i++)
	{
		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Left_bazAim", "bazAimLeft.bmp", 0, 31, 0.1f);
		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Right_bazAim", "bazAimRight.bmp", 0, 31, 0.1f);

		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Left_bazOff", "bazOffLeft.bmp", 0, 6, 0.1f, false);
		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Right_bazOff", "bazOffRight.bmp", 0, 6, 0.1f, false);

		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Left_bazOn", "bazOnLeft.bmp", 0, 6, 0.1f, false);
		dynamic_cast<Player*>(PlayerList[i])->CreatePlayerAnimation("Right_bazOn", "bazOnRight.bmp", 0, 6, 0.1f, false);
	}


}

bool WeaponBazooka::CheckCollision()
{
	if (WeaponCollision != nullptr && true == WeaponCollision->Collision({ .TargetGroup = static_cast<int>(WormsRenderOrder::Player), .TargetColType = CollisionType::CT_Rect, .ThisColType = CollisionType::CT_CirCle }))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void WeaponBazooka::Charging()
{
	if (/* isPress(�����̽���) == true */ true)
	{
		/* timer �� �ð��� ���, �ð��� �°� ��¡������ ���� ( 0 ~ 1 ���� �ð���� ) => ( ��¡���� = ������¡�ð� / �ִ���¡�ð� ) */
		/* ������ ��� �ɼ��� �߻� �Ÿ��� ����� (�߻�ӵ��� ������) */
	}
}

void WeaponBazooka::firing(float _DeltaTime) //�߻�
{
	if (GameEngineInput::IsKey("Shoot") == false)
	{
		return;
	}

	if (isEndCharging() == true)
	{
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
				break;
			}
		}

		WeaponRender->On();
		WeaponCollision->On();
		isSet = true;
	}

	Gravity = 1.0f * _DeltaTime;
	
	Dir += { 100 , -100 + Gravity }; // �ٸ� �Լ��� ����, ���� �߻� ������ ������ �� Y�࿡ +Gravity �� �����Ӹ��� �������ν� õõ�� ��(��)�����ϰ� ����� 
	Dir.Normalize();

	WeaponRender->SetAngle(-Dir.GetAnagleDeg() - 45);
	WeaponRender->SetMove(Dir * MoveSpeed * _DeltaTime);

	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(WeaponRender->GetActorPlusPos(), RGB(0, 0, 255))) //�ʿ� ������ �����
	{
		ResetWeapon(_DeltaTime);
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
	}
}

void WeaponBazooka::SetAngle()
{
	if (/*�÷��̾ �̵����̶��*/ true)
	{
		return;
	}
	else if (/*�÷��̾ �̵��� ����ٸ�*/ true)
	{
		//����ī�� ������ ������� �ִϸ��̼��� ����
	}

	if (/*����ī�� ������ ����� ��� ���´ٸ�(isAnimationEnd) */ true)
	{
		//Ű�Է��� �޾� ���Ʒ��� ������ ���� Dir�� ������ �ִ´�.
		//������ ���ϸ鼭 Dir�� ��ǥ���� ���ϰ�, Dir - PlayerPos �ؼ� ���⺤�͸� ���Ѵ��� Normalize
	}
}

void WeaponBazooka::ChangeBazReadyAnimation()
{

}

void WeaponBazooka::BazookaOn()
{
	if (CurPlayer->GetPlayerState() == PlayerState::IDLE)
	{
		TimeCounting();
	}
	else
	{	
		CurPlayer->ChangePlayerAnimation("BazOff");
		TimeCount = 0;
	}

	if (TimeCount >= 1.5f) 
	{
		CurPlayer->ChangePlayerAnimation("BazOn");
	}
}
void WeaponBazooka::ResetWeapon(float _DeltaTime)
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
	isFire = false;
	isSet = false;
}

