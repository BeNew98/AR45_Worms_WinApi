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
	// ���� �⺻ ����
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

	AimingLine = CreateRender(WormsRenderOrder::Weapon);
	AimingLine->SetImage("TempBomb.bmp");
	AimingLine->SetScale({ 10,10 });
	AllWeapons[WeaponName] = this;
}
void WeaponShotgun::Update(float _DeltaTime)
{

	if (0 == ShotGunCollision.size()) // ���Ⱑ �������� �ʾ��� ��� �����ǵ��� ��.
	{
		for (int i = 0;i < BulletCount;i++)
		{
			WeaponShotgunInit();
		}
	}
	
	// ź ������ ������
	if (DelayTime > 0)
	{
		DelayTime -= _DeltaTime;
	}

	if (PressShoot() && DelayTime <= 0)
	{
		for (int i = 0;i < BulletCount;i++)
		{
			if (isShooted[i] == false)
			{
				isShooted[i] = true;
				ShotGunDir[i] = Dir; // �߻�� ���⼳��
				DelayTime = 1.0f;
				break;
			}
		}
		int a = 0;
	}
	else
	{
		float4 PlayerPos = (GetLevel()->GetActors(WormsRenderOrder::Player))[0]->GetPos();// �ӽ� index0 PlayerPos
		SetPos(PlayerPos - float4{0,30});
		Dir = GetShootDir(); // ���� ����
		AimingLine->SetPosition(Dir*50 + float4{ 0,30 });

	}

	for (int i = 0;i < BulletCount;i++)
	{
		if (true == isShooted[i] && true == ShotGunCollision[i]->IsUpdate())
		{
			WeaponMove(ShotGunCollision[i], _DeltaTime, ShotGunDir[i]);

			if (true == CheckCollision(ShotGunCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
			{
				MakeBomb(ShotGunCollision[i]->GetPosition()); // ���� ���� ǥ��

				ShotGunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
			}
		}
	}


}

void WeaponShotgun::MakeBomb(float4 _Pos) // �ӽ� - ��ź ������ ��ġ ǥ��
{
	GameEngineRender* BombRender = CreateRender(WormsRenderOrder::Weapon);
	BombRender->SetImage("TempBomb.bmp"); // �ӽ� �׸�
	BombRender->SetPosition(_Pos);
	BombRender->SetScale(BombScale);

	/*GameEngineCollision* BombCollision = CreateCollision(WormsRenderOrder::Weapon);
	BombCollision->SetPosition(_Pos);
	BombCollision->SetScale(BombScale);*/
}


void WeaponShotgun::WeaponMove(GameEngineCollision* _Col, float _DeltaTime,float4 _Dir)
{
	if (false == _Col->IsUpdate())
	{
		return;
	}

	if (true == EffectGravity)
	{

	}
	else // �߷��� ������ ���� ����.
	{
		_Col->SetMove(_Dir * _DeltaTime * MoveSpeed);
	}
}

void WeaponShotgun::Render(float _DeltaTime)
{

}

void WeaponShotgun::WeaponShotgunInit()
{
	// ShotGun�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsRenderOrder::Weapon);
	Collision->SetScale({ 25,25 });

	ShotGunCollision.push_back(Collision);
	isShooted.push_back(false);
	ShotGunDir.push_back(float4::Right);

}

bool WeaponShotgun::CheckCollision(GameEngineCollision* _Col)
{
	// �÷��̾ ���� ���
	if (nullptr != _Col && true == _Col->Collision({ .TargetGroup = static_cast<int>(WormsRenderOrder::Player), .TargetColType = CollisionType::CT_Rect, .ThisColType = CollisionType::CT_CirCle }))
	{
		return true;
	}


	// �ʰ� �浹�ϴ� ���
	if (nullptr == MapCollision)
	{
		MsgAssert("�ݸ��� �̹����� �������� �ʾҽ��ϴ�.");
		return false;
	}
	//if (RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetPosition(), RGB(255, 0, 255))) // - �ݸ��� �� ��ġ ���� �ʿ�
	//{

	//	return true;
	//}

	//�ӽ� Ÿ�̸�
	/*if (GetLiveTime() >= 5.0f)
	{
		return true;
	}*/

	// �������� ������ ������ ��� - �߰� �ʿ�

	return false;

}