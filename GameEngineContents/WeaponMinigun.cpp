#include "WeaponMinigun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineBase/GameEngineRandom.h>
#include "MapModifier.h"
#include "Player.h"
#include "SmokeSparkEffect.h"

WeaponMinigun::WeaponMinigun()
{
}

WeaponMinigun::~WeaponMinigun()
{
}

void WeaponMinigun::Start()
{
	// �̴ϰ� �⺻ ����
	WeaponName = "Minigun";
	MoveSpeed = 3000;
	//Dir = float4::Right;
    BombScale = 22;

    MaxDmg = 10;
    MinDmg = 3;

    MaxKnockBackPower = 22;
    MinKnockBackPower = 22;

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���
	//AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Minigun);



	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
    AimingLine->SetImage("AimingLine.bmp");
    AimingLine->SetRotFilter("AimingLineRot.bmp");
	AimingLine->SetScale({ 20,20 });


}

void WeaponMinigun::Update(float _DeltaTime)
{
	while (BulletCount > MinigunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponMinigunInit();
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


void WeaponMinigun::Aiming(float _DeltaTime)
{
    if (CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON) // ���� �÷��̾ ���� State
    {
        // ��ġ
        float4 PlayerPos = CurPlayer->GetPos() + float4{ 0,-10 };
        Dir = GetShootDir();
        SetPos(PlayerPos);

        // ���ؼ�
        SetAimFrameIndex();
        AimingLine->On();

        float Ratio = 6 * _DeltaTime;
        AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);

        CurPlayer->ChangePlayerAnimation("MinigunAim", static_cast<int>(AimIndex));
        AimingLine->SetPosition(Dir * 150 ); // ���ؼ� �̵�
        AimingLine->SetAngle(-Dir.GetAnagleDeg());

        CheckFiring(); // ����üũ, �߻� üũ
    }
    else
    {
        AimingLine->Off();
    }
}

void WeaponMinigun::SetAimFrameIndex()
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

    NewIndex = static_cast<int>(Angle / 5 + 15);

    if (NewIndex < 0)
    {
        NewIndex = 0;
    }
    NextAimIndex = NewIndex;

}



bool WeaponMinigun::IsDone()
{
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == MinigunCollision[i]->IsUpdate())
		{
			return false;
		}

	}
	return true;
}

void WeaponMinigun::CheckFiring()
{
    if (PressShoot()) // �߻�üũ
    {
        isFire = true;
        AimingLine->Off();
        CurPlayer->ChangePlayerAnimation("MinigunFire", static_cast<int>(AimIndex));

    }

}


void WeaponMinigun::Firing(float _DeltaTime)
{
    // ī�޶� �̵�
    float4 CamPos = float4::Zero.Lerp(GetLevel()->GetCameraPos(), MinigunCollision[0]->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half(), _DeltaTime * 100);
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
                MinigunCollision[i]->On();
                MinigunCollision[i]->SetMove(float4(0, GameEngineRandom::MainRandom.RandomFloat(-3.0f, 3.0f)));
                break;
            }

        }
    }

    for (int i = 0; i < BulletCount; i++)
    {
        if (true == isShooted[i] && true == MinigunCollision[i]->IsUpdate())
        {
            if (isIsFireAnimationDone == false && i == BulletCount - 1 && isWeaponDone == false)
            {
                CurPlayer->ChangePlayerAnimation("Idle");
                isIsFireAnimationDone = true;
            }

            float4 MoveVec = Dir * MoveSpeed * _DeltaTime;
            MinigunCollision[i]->SetMove(MoveVec);
            float4 CheckCol = Check4Side(MinigunCollision[i], MinigunCollision[i]->GetActorPlusPos());
            if (CheckCol.AddAllVec() > 0)
            {
                if (CheckCol.AddAllVec() == 4)
                {
                    MinigunCollision[i]->SetMove(-MoveVec * 0.3f);
                }
                //MinigunCollision[i]->SetMove(-MoveVec * 0.1f* CheckCol.AddAllVec());
                SmokeSparkEffect* Smoke = GetLevel()->CreateActor<SmokeSparkEffect>();
                Smoke->SetPos(MinigunCollision[i]->GetActorPlusPos());
                Smoke->CreateSmokeSpark(6, 2, BombScale);

                MinigunCollision[i]->SetScale(float4{ static_cast<float>(BombScale) });


                AttackPlayerGun(MinigunCollision[i], 500);																				  // 3. Bomb�ݸ��� Player Check
                MapModifier::MainModifier->CreateHole(GetPos() + MinigunCollision[i]->GetPosition(), static_cast<int>(BombScale));					  // 4. ���� �����

                MinigunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
                isExplosion = true;
            }
        }
    }

}


void WeaponMinigun::WeaponMinigunInit()
{
	// Minigun�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 20,20 });
    Collision->Off();

	MinigunCollision.push_back(Collision);
	isShooted.push_back(false);
	DelayTime = 0.05f;
}

