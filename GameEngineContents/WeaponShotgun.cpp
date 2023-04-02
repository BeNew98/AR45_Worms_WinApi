#include "WeaponShotgun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>

#include "MapModifier.h"
#include "Player.h"
#include "SmokeSparkEffect.h"

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
	MoveSpeed = 2500;
	//Dir = float4::Right;
    BombScale = 22;

    MaxDmg = 50;
    MinDmg = 12;
    MaxKnockBackPower = 97;
    MinKnockBackPower = 22;

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // ���� �ʿ� : Level or Map���Ϳ��� �����;���

	WeaponNumber = static_cast<int>(WeaponNum::Shotgun);


	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
    AimingLine->SetImage("AimingLine.bmp");
    AimingLine->SetRotFilter("AimingLineRot.bmp");
	AimingLine->SetScale({ 20,20 });

}

void WeaponShotgun::Update(float _DeltaTime)
{
	while (BulletCount > ShotGunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponShotgunInit();
        SetCurPlayer();// �÷��̾� ��ȯ��ư ������ �߰�
	}

    if (WeaponDoneTime < GetLiveTime())
    {
        GetLevel()->SetCameraPos(CurPlayer->GetPos() - GameEngineWindow::GetScreenSize().half());
        WeaponDoneTime = 0.0f;
    }
    if (false == isFire) //�߻簡 �ȵǾ��ٸ�
    {
        Aiming(_DeltaTime);
    }
    else
    {
        if (false == isExplosion) // �߻�ǰ� ���� ������ ��
        {
            Firing(_DeltaTime);
        }
        else // �߻��ϰ� ���� ��
        {
            if (true == IsDone()) // �Ѿ� ��� ����Ͽ����� üũ
            {
                isWeaponDone = true;
                GetLevel()->SetCameraPos(GetPos() - GameEngineWindow::GetScreenSize().half());
            }
        }
    }


}
void  WeaponShotgun::Aiming(float _DeltaTime)
{

    if (CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON) // ���� �÷��̾ ���� State
    {
        // ��ġ
        float4 PlayerPos = CurPlayer->GetPos();
        Dir = GetShootDir();
        SetPos(PlayerPos);

        // ���ؼ�
        SetAimFrameIndex();
        AimingLine->On();

        float Ratio = 6 * _DeltaTime;
        AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);

        CurPlayer->ChangePlayerAnimation("ShotgunAim", static_cast<int>(AimIndex));
        AimingLine->SetPosition(Dir * 150 + float4{ 0, -10 }); // ���ؼ� �̵�

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
void WeaponShotgun::SetAimFrameIndex()
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

void WeaponShotgun::CheckFiring()
{
	if (true == GameEngineInput::IsDown("Shoot")) // �߻�üũ
	{
		for (int i = 0; i < BulletCount; i++)
		{
			if (isShooted[i] == false)
			{
				isShooted[i] = true;
				ShotGunDir[i] = Dir; // �߻�� ���⼳��
                //isIsFireAnimationDone = true;
                AimingLine->Off();
				isFire = true;
                isExplosion = false;
                LoadDelay = 0.5f;
                break;
			}
		}
	}

}


void WeaponShotgun::Firing(float _DeltaTime)
{
    LoadDelay -= _DeltaTime;
    if (LoadDelay > 0) // ���� ������(�����Ҹ�)
    {
        return;
    }
    CurPlayer->ChangePlayerAnimation("HandgunFire", static_cast<int>(AimIndex));

	for (int i = 0; i < BulletCount; i++)
	{
		if (true == isShooted[i] && true == ShotGunCollision[i]->IsUpdate()) // ���� On�� �Ѿ�
		{

            float4 MoveVec = ShotGunDir[i] * _DeltaTime * MoveSpeed;
            ShotGunCollision[i]->SetMove(MoveVec);
			if (0 < CheckCollisionSide(ShotGunCollision[i]).Size()) // �ݸ��� üũ(�÷��̾�, ��)
			{
                ShotGunCollision[i]->SetMove(-MoveVec * 0.1f);
                // ��������Ʈ
                SmokeSparkEffect* Smoke = GetLevel()->CreateActor<SmokeSparkEffect>();
                Smoke->SetPos(ShotGunCollision[i]->GetActorPlusPos());
                Smoke->CreateSmokeSpark(6, 2, BombScale);

                // Player �� �ݸ��� üũ�� ����
				GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();				// 1. Bomb �ݸ��� ��������
				BombCollision->SetPosition(ShotGunCollision[i]->GetActorPlusPos());							        // 2. Bomb �ݸ��� �̵�

                // shotgun�� �ٸ� �ѵ�� �ٸ��� �ִ� �������� ũ�⶧���� BombScale�� �� �����Ѵ�.
                float4 Dis = BombCollision->GetActorPlusPos() - GetPos();
                float Dis_Ratio = (Dis.Size() / 500) > 1 ? 1 : Dis.Size() / 500;
                BombScale = static_cast<int>(MaxKnockBackPower * (1 - Dis_Ratio) + MinKnockBackPower * Dis_Ratio);
                BombCollision->SetScale(float4{ static_cast<float>(BombScale) });  // ��źũ�� �缳��

                AttackPlayerGun(BombCollision, 500);																// 3. Bomb�ݸ��� Player Check

				MapModifier::MainModifier->CreateHole(GetPos() + ShotGunCollision[i]->GetPosition(), BombScale);	// 4. ���� �����

				ShotGunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
                isExplosion = true;

                if (i+1 < BulletCount)
                {
                    isFire = false;

                }
                WeaponDoneTime = GetLiveTime() + 1.0f;
                CurPlayer->ChangePlayerAnimation("Idle");
			}
            // ī�޶� �̵�
            float4 CamPos = float4::Zero.Lerp(GetLevel()->GetCameraPos(), ShotGunCollision[i]->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half(), _DeltaTime * 50);
            GetLevel()->SetCameraPos(CamPos);
		}
	}

}


void WeaponShotgun::WeaponShotgunInit()
{
	// ShotGun�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 10,10 });
    Collision->SetPosition({ 0,-10 });

	ShotGunCollision.push_back(Collision);
	isShooted.push_back(false);
	ShotGunDir.push_back(float4::Right);

}


bool WeaponShotgun::IsDone()
{
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == ShotGunCollision[i]->IsUpdate())
		{
			return false;
		}

	}
    if (WeaponDoneTime > GetLiveTime())
    {
        return false;
    }
	return true;
}