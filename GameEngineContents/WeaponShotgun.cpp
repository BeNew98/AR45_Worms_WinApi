#include "WeaponShotgun.h"
#include "ContentsEnums.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineWindow.h>


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
	MoveSpeed = 3000;
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

    if (false == isFire) //�߻簡 �ȵǾ��ٸ�
    {
        Aiming(_DeltaTime);
        if (WaitingTime < GetLiveTime())
        {
            GetLevel()->SetCameraPos(CurPlayer->GetPos() - GameEngineWindow::GetScreenSize().half());
        }
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
        float4 PlayerPos = CurPlayer->GetPos() + float4{ 0,-15 };
        Dir = GetShootDir();
        SetPos(PlayerPos);

        // ���ؼ�
        SetAimFrameIndex();
        AimingLine->On();

        float Ratio = 6 * _DeltaTime;
        AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);

        CurPlayer->ChangePlayerAnimation("ShotgunAim", static_cast<int>(AimIndex));
        AimingLine->SetPosition(Dir * 150 + float4{ 0,15 }); // ���ؼ� �̵�
        AimingLine->SetAngle(-Dir.GetAnagleDeg());

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

    NewIndex = static_cast<int>(Angle / 5 + 15);

    if (NewIndex < 0)
    {
        NewIndex = 0;
    }
    NextAimIndex = NewIndex;

}

void WeaponShotgun::CheckFiring()
{
	if (true == DownShoot()) // �߻�üũ
	{
		for (int i = 0; i < BulletCount; i++)
		{
			if (isShooted[i] == false)
			{
				isShooted[i] = true;
                ShotGunCollision[i]->On();
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
            float4 CheckCollision = CheckCollisionSide(ShotGunCollision[i]);
            if (CheckCollision == float4::Up &&Dir.Size() > 0.001f)
            {
                ShotGunCollision[i]->SetMove(-MoveVec);
                Dir *= 0.3f;
                return;
            }
            // ī�޶� �̵�
            float4 CamPos = float4::Zero.Lerp(GetLevel()->GetCameraPos(), ShotGunCollision[i]->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half(), _DeltaTime * 100);
            GetLevel()->SetCameraPos(CamPos);

			if (0 < CheckCollision.Size() || Dir.Size() < 0.001f) // �ݸ��� üũ(�÷��̾�, ��)
			{
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
                BombScale = MaxKnockBackPower * (1 - Dis_Ratio) + MinKnockBackPower * Dis_Ratio;
                BombCollision->SetScale(float4{ static_cast<float>(BombScale) });  // ��źũ�� �缳��

                AttackPlayerGun(BombCollision, 500);																// 3. Bomb�ݸ��� Player Check

				MapModifier::MainModifier->CreateHole(GetPos() + ShotGunCollision[i]->GetPosition(), static_cast<int>(BombScale));	// 4. ���� �����

				ShotGunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
                isExplosion = true;

                if (i+1 < BulletCount)
                {
                    isFire = false;

                }
                WaitingTime = GetLiveTime() + 1.5f;
			}
		}
	}

}


void WeaponShotgun::WeaponShotgunInit()
{
	// ShotGun�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 10,10 });
    Collision->Off();

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
    if (WaitingTime > GetLiveTime())
    {
        return false;
    }
	return true;
}