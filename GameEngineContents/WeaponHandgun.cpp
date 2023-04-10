#include "WeaponHandgun.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>

#include "ContentsEnums.h"
#include "MapModifier.h"
#include "Player.h"
#include "SmokeSparkEffect.h"
#include "Map.h"

WeaponHandgun::WeaponHandgun()
{
}

WeaponHandgun::~WeaponHandgun()
{
}

void WeaponHandgun::Start()
{
	// �ڵ�� �⺻ ����
	WeaponName = "Handgun";
	MoveSpeed = 3000;
	//Dir = float4::Right;
	BombScale = 22;

    MaxDmg = 10;
    MinDmg = 3;
    MaxKnockBackPower = 22;
    MinKnockBackPower = 22;

    std::string Name = Map::MainMap->GetColMapName();
    MapCollision = GameEngineResources::GetInst().ImageFind(Name);

	//AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Handgun);
	SetCurPlayer();


	// �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
	AimingLine = CreateRender(WormsRenderOrder::Weapon);
    AimingLine->SetImage("AimingLine.bmp");
    AimingLine->SetRotFilter("AimingLineRot.bmp");
	AimingLine->SetScale({ 20,20 });


}

void WeaponHandgun::Aiming(float _DeltaTime)
{
    if (CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON) // ���� �÷��̾ ���� State
    {
        // ��ġ
        float4 PlayerPos = CurPlayer->GetPos() + float4{ 0,-10 };;
        Dir = GetShootDir();
        SetPos(PlayerPos);

        // ���ؼ�
        SetAimFrameIndex();
        AimingLine->On();

        float Ratio = 6 * _DeltaTime;
        AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);

        CurPlayer->ChangePlayerAnimation("HandgunAim", static_cast<int>(AimIndex));
        AimingLine->SetPosition(Dir * 150); // ���ؼ� �̵�
        AimingLine->SetAngle(-Dir.GetAnagleDeg());


        CheckFiring(); // ����üũ, �߻� üũ
    }
    else
    {
        AimingLine->Off();
    }
}

void WeaponHandgun::Update(float _DeltaTime)
{
    // �Ѿ� ����ŭ init
	while (BulletCount > HandgunCollision.size()) // ��ź ������ŭ WeaponInit
	{
		WeaponHandgunInit();
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

void WeaponHandgun::SetAimFrameIndex()
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

bool WeaponHandgun::IsDone()
{
	for (int i = 0; i < BulletCount; i++)
	{
		if (true == HandgunCollision[i]->IsUpdate())
		{
			return false;
		}

	}
	return true;
}

void WeaponHandgun::CheckFiring()
{
    if (PressShoot()) // �߻�üũ
    {
        isFire = true;
        AimingLine->Off();
        CurPlayer->ChangePlayerAnimation("HandgunFire", static_cast<int>(AimIndex));

    }
}


void WeaponHandgun::Firing(float _DeltaTime)
{
    // ī�޶� �̵�
    float4 CamPos = float4::Zero.Lerp(GetLevel()->GetCameraPos(), HandgunCollision[0]->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half(), _DeltaTime * 100);
    GetLevel()->SetCameraPos(CamPos);

    DelayTime -= _DeltaTime;
    if (DelayTime < 0)
    {
        DelayTime = 0.3f;
        for (int i = 0; i < BulletCount; i++)
        {
            if (isShooted[i] == false)
            {
                isShooted[i] = true;
                HandgunCollision[i]->On();
                if (0 == (1 & i))
                {
                    HandgunCollision[i]->SetMove({ 0,1.0f*i });
                }
                else
                {
                    HandgunCollision[i]->SetMove({ 0,-1.0f * i });
                }
                break;
            }

        }
    }

    for (int i = 0; i < BulletCount; i++)
    {
        if (true == isShooted[i] && true == HandgunCollision[i]->IsUpdate())
        {
            if (isIsFireAnimationDone == false && i == BulletCount - 1 && isWeaponDone == false)
            {
                CurPlayer->ChangePlayerAnimation("Idle");
                isIsFireAnimationDone = true;
            }
            float4 MoveVec = Dir * MoveSpeed * _DeltaTime;
            HandgunCollision[i]->SetMove(MoveVec);
            float4 CheckCol = Check4Side(HandgunCollision[i], HandgunCollision[i]->GetActorPlusPos());
            if (CheckCol.AddAllVec() > 0)
            {
                if (CheckCol.AddAllVec() == 4)
                {
                    HandgunCollision[i]->SetMove(-MoveVec * 0.3f);
                }
                SmokeSparkEffect* Smoke = GetLevel()->CreateActor<SmokeSparkEffect>();
                Smoke->SetPos(HandgunCollision[i]->GetActorPlusPos());
                Smoke->CreateSmokeSpark(6, 2, BombScale);

                HandgunCollision[i]->SetScale(float4{ static_cast<float>(BombScale) });


                AttackPlayerGun(HandgunCollision[i], 500);																				  // 3. Bomb�ݸ��� Player Check
                MapModifier::MainModifier->CreateHole(GetPos() + HandgunCollision[i]->GetPosition(), static_cast<int>(BombScale));					  // 4. ���� �����

                HandgunCollision[i]->Off(); // �߻簡 ���� ��ź �ݸ���
                isExplosion = true;

                GameEngineResources::GetInst().SoundPlay("HANDGUNFIRE.wav");
            }
        }
    }
	
}


void WeaponHandgun::WeaponHandgunInit()
{
	// Handgun�� �����̹����� �������� ����
	GameEngineCollision* Collision = CreateCollision(WormsCollisionOrder::Weapon);
	Collision->SetScale({ 20,20 });
    Collision->Off();

	HandgunCollision.push_back(Collision);
	isShooted.push_back(false);
	DelayTime = 0.3f;

}
