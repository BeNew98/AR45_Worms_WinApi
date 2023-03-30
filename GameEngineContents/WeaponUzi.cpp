#include "WeaponUzi.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineWindow.h>

#include "MapModifier.h"
#include "Player.h"
#include "SmokeSparkEffect.h"

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

	//AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Uzi);


	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
	AimingLine->SetImage("AimingLine.bmp");
    AimingLine->SetRotFilter("AimingLineRot.bmp");
	AimingLine->SetScale({ 30,30  });


}

void WeaponUzi::Update(float _DeltaTime)
{
	while (BulletCount > UziCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponUziInit();
        SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�
        CurPlayer->ChangePlayerAnimation("UziAim", static_cast<int>(AimIndex));
	}
    if (false == isFire)
    {
        //SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�
        SetAimFrameIndex();

        if (AimIndex != NextAimIndex && CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON && CurPlayer->GetCurWeapon()->GetWeaponNumber() == WeaponNumber)
        {
            float Ratio = 6 * _DeltaTime;
            AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);
            CurPlayer->ChangePlayerAnimation("UziAim", static_cast<int>(AimIndex));
            AimingLine->On();
            AimingLine->SetPosition(Dir * 200); // ���ؼ� �̵�
            if (Dir.x > 0)
            {
                AimingLine->SetAngle(Dir.GetAnagleDeg());
            }
            else
            {
                AimingLine->SetAngle(-Dir.GetAnagleDeg());
            }
        }
        else
        {
            AimingLine->Off();
        }
	    CheckFiring(); // ����üũ, �߻� üũ
    }
	Firing(_DeltaTime); // �Ѿ��� ������ �ӵ��� ���ư��� �����ϰ� ��

	if (true == IsDone())
	{
		isWeaponDone = true;
        GetLevel()->SetCameraPos(GetPos() - GameEngineWindow::GetScreenSize().half()); //���� �� Player�� ī�޶� �̵�- �����ʿ�
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
            AimingLine->Off();
            CurPlayer->ChangePlayerAnimation("UziFire", static_cast<int>(AimIndex));
		}
		float4 PlayerPos = CurPlayer->GetPos();
		SetPos(PlayerPos);
		Dir = GetShootDir(); // ���� ����
		//AimingLine->SetPosition(Dir * 100); // ���ؼ� �̵�
        //AimingLine->SetAngle(Dir.GetAnagleDeg());

	}

}

void WeaponUzi::SetAimFrameIndex()
{
    float Angle = Dir.GetAnagleDeg();


    int NewIndex = 0;
    if (Dir.x > 0 && Angle > 270)
    {
        Angle = Angle - 360;
    }

    else if (Dir.x < 0)
    {
        Angle = 180 - Angle;
    }

    NewIndex = Angle / 5 + 15;

    if (NewIndex < 0)
    {
        NewIndex = 0;
    }
    NextAimIndex = NewIndex;

}


void WeaponUzi::Firing(float _DeltaTime)
{

	if (true == isFire && false == isWeaponDone)
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
                if (isIsFireAnimationDone == false && i == BulletCount - 1 && isWeaponDone == false)
                {
                    CurPlayer->ChangePlayerAnimation("Idle");
                    isIsFireAnimationDone = true;
                }

				UziCollision[i]->SetMove(Dir * _DeltaTime * MoveSpeed);
				if (true == CheckCollision(UziCollision[i])) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
				{
                    SmokeSparkEffect* Smoke = GetLevel()->CreateActor<SmokeSparkEffect>();
                    Smoke->SetPos(UziCollision[i]->GetActorPlusPos());
                    Smoke->CreateSmokeSpark(6, 1, BombScale);

					GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();
					BombCollision->SetPosition(GetPos() + UziCollision[i]->GetPosition());

					AttackPlayer(BombCollision,BombScale);

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