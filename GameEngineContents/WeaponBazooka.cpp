#include "WeaponBazooka.h"
#include "ContentsEnums.h"

#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEngineCore/GameEngineResources.h>

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
	WeaponCollision = CreateCollision(static_cast<int>(WormsRenderOrder::Weapon));

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp");

	WeaponRender->SetPosition({ 500, 200 }); //�ӽ� ������
	WeaponRender->SetScale({150, 150}); //�ӽ� ������

	WeaponName = "Bazooka";

	Gravity = 100.0f; //�ӽ� ������
	GravityAccel = 0.0f; //�ӽ� ������

	MoveSpeed = 400.0f; //�ӽ� ������

	EffectGravity = true;
	isAnimation = true;
	isBlocked = true;
	isTarget = false;


						
}

void WeaponBazooka::CreatePlayerAnimation()
{

}

void WeaponBazooka::firing(float _DeltaTime)
{
	Gravity += GravityAccel * _DeltaTime;
	GravityAccel += 150.0f * _DeltaTime;
	
	Dir = { 50, -150 + Gravity }; // �ٸ� �Լ��� ����, ���� �߻� ������ ������ �� Y�࿡ +Gravity �� �����Ӹ��� �������ν� õõ�� ��(��)�����ϰ� ����� 
	Dir.Normalize();

	WeaponRender->SetMove(Dir * MoveSpeed * _DeltaTime);

	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(WeaponRender->GetActorPlusPos(), RGB(0, 0, 255))) //�ʿ� ������ �����
	{
		WeaponRender->Off(); 
	}

}