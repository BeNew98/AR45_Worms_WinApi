#include "WeaponBazooka.h"
#include "ContentsEnums.h"
#include "Player.h"

#include <GameEnginePlatform/GameEngineImage.h>
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
}

void WeaponBazooka::Render(float _DeltaTime)
{

}

void WeaponBazooka::WeaponBazookaInit()
{
	WeaponRender =  CreateRender("Weapon1.bmp", static_cast<int>(WormsRenderOrder::Weapon));
	WeaponCollision = CreateCollision(static_cast<int>(WormsCollisionOrder::Weapon));

	WeaponRender->SetRotFilter("Weapon1Rot.bmp");

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp");

	WeaponRender->SetPosition({ 500, 200 }); //�ӽ� ������
	WeaponRender->SetScale({150, 150}); //�ӽ� ������

	WeaponCollision->SetPosition(WeaponRender->GetPosition());
	WeaponCollision->SetScale(WeaponRender->GetScale());

	WeaponName = "Bazooka";

	Gravity = 100.0f; //�ӽ� ������
	GravityAccel = 0.0f; //�ӽ� ������

	MoveSpeed = 400.0f; //�ӽ� ������

	EffectGravity = true;
	isAnimation = true;
	isBlocked = true;
	isTarget = false;

	CreatePlayerAnimation();						
}

void WeaponBazooka::CreatePlayerAnimation()
{
	//�÷��̾ �޾ƿͼ� ������ �ִϸ��̼� �߰�
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
	std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player);

	if (isSet == false)
	{
		for (int i = 0; i < PlayerList.size(); i++)
		{
			if (true == dynamic_cast<Player*>(PlayerList[i])->GetIsMyTurn())
			{
				WeaponRender->SetPosition(PlayerList[i]->GetPos());
			}
		}

		isSet = true;
	}

	Gravity += GravityAccel * _DeltaTime;
	GravityAccel += 150.0f * _DeltaTime;
	
	Dir = { 50, -150 + Gravity }; // �ٸ� �Լ��� ����, ���� �߻� ������ ������ �� Y�࿡ +Gravity �� �����Ӹ��� �������ν� õõ�� ��(��)�����ϰ� ����� 
	Dir.Normalize();

	WeaponRender->SetAngle(-Dir.GetAnagleDeg());
	WeaponRender->SetMove(Dir * MoveSpeed * _DeltaTime);

	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(WeaponRender->GetActorPlusPos(), RGB(0, 0, 255))) //�ʿ� ������ �����
	{
		//WeaponRender->Off(); 
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