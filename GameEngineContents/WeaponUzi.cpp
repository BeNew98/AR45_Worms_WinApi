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
	MoveSpeed = 3000;
	//Dir = float4::Right;
    BombScale = 22;

    MaxDmg = 10;
    MinDmg = 3;
    MaxKnockBackPower = 40;
    MinKnockBackPower = 22;

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
	}

    if (false == isFire) //�߻簡 �ȵǾ��ٸ�
    {
        Aiming(_DeltaTime);
    }
    else
    {
        Firing(_DeltaTime); // �Ѿ��� ������ �ӵ��� ���ư��� �����ϰ� ��
        if (true == isExplosion)
        {
            // ��� �Ѿ˹߻�Ǿ� �������� üũ
            if (true == IsDone())
            {
                if (WaitingTime < 0)
                {
                    isWeaponDone = true;
                    GetLevel()->SetCameraPos(GetPos() - GameEngineWindow::GetScreenSize().half()); //���� �� Player�� ī�޶� �̵�- �����ʿ�
                    return;
                }
                WaitingTime -= _DeltaTime;
            }
        }

    }

}



void WeaponUzi::Aiming(float _DeltaTime)
{
    if (CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON) // ���� �÷��̾ ���� State
    {
        // ��ġ
        float4 PlayerPos = CurPlayer->GetPos() + float4{ 0,-15 };
        Dir = GetShootDir();
        SetPos(PlayerPos);

        // ���ؼ�
        SetAimFrameIndex();
        AimingLine->On();

        float Ratio = 6 * _DeltaTime;
        AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);

        CurPlayer->ChangePlayerAnimation("UziAim", static_cast<int>(AimIndex));
        AimingLine->SetPosition(Dir * 150); // ���ؼ� �̵�

        if (Dir.x > 0)
        {
            AimingLine->SetAngle(Dir.GetAnagleDeg());
        }
        else
        {
            AimingLine->SetAngle(-Dir.GetAnagleDeg());
        }

        CheckFiring(); // ����üũ, �߻� üũ
    }
    else
    {
        AimingLine->Off();
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
    if (PressShoot()) // �߻�üũ
    {
        isFire = true;
        AimingLine->Off();
        CurPlayer->ChangePlayerAnimation("UziFire", static_cast<int>(AimIndex));
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
    // ī�޶� �̵�
    float4 CamPos = float4::Zero.Lerp(GetLevel()->GetCameraPos(), UziCollision[0]->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half(), _DeltaTime * 100);
    GetLevel()->SetCameraPos(CamPos);

    DelayTime -= _DeltaTime;
    if (DelayTime < 0)
    {
        DelayTime = 0.05f;
        for (int i = 0; i < BulletCount; i++)
        {
            if (isShooted[i] == false)
            {
                isShooted[i] = true;
                UziCollision[i]->On(); UziCollision[i]->On();
                if (0 == (1 & i))
                {
                    UziCollision[i]->SetMove({ 0,2.0f * i });
                }
                else
                {
                    UziCollision[i]->SetMove({ 0,-2.0f * i });
                }
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
            float4 CheckCollision = CheckCollisionSide(UziCollision[i]);
            if (CheckCollision == float4::Up && Dir.Size() > 0.001f)
            {
                UziCollision[i]->SetMove(-Dir * _DeltaTime * MoveSpeed);
                Dir *= 0.7f;
                return;
            }
            if (CheckCollision.Size() > 0 || Dir.Size() < 0.001f) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
            {
                SmokeSparkEffect* Smoke = GetLevel()->CreateActor<SmokeSparkEffect>();
                Smoke->SetPos(UziCollision[i]->GetActorPlusPos());
                Smoke->CreateSmokeSpark(3, 1, BombScale);

                GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();								  // 1. Bomb �ݸ��� ��������
                BombCollision->SetPosition(GetPos() + UziCollision[i]->GetPosition());											  // 2. Bomb �ݸ��� �̵�
                BombCollision->SetScale(float4{ static_cast<float>(BombScale) });


                AttackPlayerGun(BombCollision, 500);																				  // 3. Bomb�ݸ��� Player Check
                MapModifier::MainModifier->CreateHole(GetPos() + UziCollision[i]->GetPosition(), BombScale);					  // 4. ���� �����

                UziCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
                isExplosion = true;
            }
        }
    }

}


void WeaponUzi::WeaponUziInit()
{
	// Uzi�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 10,10 });
    Collision->Off();

	UziCollision.push_back(Collision);
	isShooted.push_back(false);
	DelayTime = 0.1f;

}
