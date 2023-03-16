#include "WeaponShotgun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>

WeaponShotgun::WeaponShotgun()
{
}

WeaponShotgun::~WeaponShotgun()
{
}

void WeaponShotgun::Start()
{
	WeaponName = "Shotgun";
	EffectGravity = false;
	isAnimation = false;
	isBlocked = true;
	isTarget = false;

	MoveSpeed = 600.0f;
	//float Gravity = 0.0f;
	//float Timer = 0.0f;
	//float WindPower = 0.0f;
	//float Dmg = 0.0f;
	//float Charge = 0.0f;

	Dir = float4{ 1,0 }.NormalizeReturn();
	//float4 PlayerPos = float4::Zero;
	BombScale = float4{ 100,100 };
	//float4 Scale = float4::Zero;	

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp");
	//WeaponShotgunInit();
	//SetPos({50, 400});
}
void WeaponShotgun::Update(float _DeltaTime)
{
	if (nullptr == WeaponCollision)
	{
		WeaponShotgunInit();
		float4 PlayerPos = (GetLevel()->GetActors(WormsRenderOrder::Player))[0]->GetPos();// �ӽ� index0 PlayerPos
		SetPos(PlayerPos - float4{ 0, 30 });
	}
	

	WeaponMove(_DeltaTime);

	if (true == CheckCollision() && nullptr == BombRender)
	{
		
		BombRender = CreateRender(WormsRenderOrder::Weapon);
		BombRender->SetImage("TempBomb.bmp");
		BombRender->SetPosition(WeaponCollision->GetPosition());
		BombRender->SetScale(BombScale);

		BombCollision = CreateCollision(WormsRenderOrder::Weapon);
		BombCollision->SetPosition(WeaponCollision->GetPosition());
		BombCollision->SetScale(BombScale);

		WeaponCollision->Off();
	}


}

void WeaponShotgun::WeaponMove(float _DeltaTime)
{
	if (true == EffectGravity)
	{

	}
	else
	{
		WeaponCollision->SetMove(Dir * _DeltaTime * MoveSpeed);
	}
}
void WeaponShotgun::Render(float _DeltaTime)
{

}

void WeaponShotgun::WeaponShotgunInit()
{
	// ShotGun�� �����̹����� �������� ����
	WeaponCollision = CreateCollision(WormsRenderOrder::Weapon);
	WeaponCollision->SetScale({ 25,25 });



}

bool WeaponShotgun::CheckCollision()
{
	// �÷��̾ ���� ���
	if (nullptr != WeaponCollision && true == WeaponCollision->Collision({ .TargetGroup = static_cast<int>(WormsRenderOrder::Player), .TargetColType = CollisionType::CT_Rect, .ThisColType = CollisionType::CT_CirCle }))
	{
		return true;
	}


	// �ʰ� �浹�ϴ� ���
	if (nullptr == MapCollision)
	{
		MsgAssert("�ݸ��� �̹����� �������� �ʾҽ��ϴ�.");
		return false;
	}
	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(WeaponCollision->GetPosition(), RGB(0, 0, 255))) // - �ݸ��� �� ��ġ ���� �ʿ�
	{
		int a = 0;

		return true;
	}

	//�ӽ� Ÿ�̸�
	if (GetLiveTime() >= 5.0f)
	{
		return true;
	}

	// �������� ������ ������ ��� - �߰� �ʿ�

	return false;

}