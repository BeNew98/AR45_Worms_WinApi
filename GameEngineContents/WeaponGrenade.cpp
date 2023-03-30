#include "WeaponGrenade.h"
#include "ContentsEnums.h"
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>

#include <GameEnginePlatform/GameEngineWindow.h>
#include "Player.h"
#include "MapModifier.h"


WeaponGrenade::WeaponGrenade() 
{
}

WeaponGrenade::~WeaponGrenade() 
{
}

void WeaponGrenade::Start()
{
	// ����ź �⺻ ���� -- ����� ���� �ʿ�
	MoveSpeed = 700.0f; // �ӽð�
	Gravity = 1.0f;// �ӽð�
	//Dmg = 0.0f;
	Timer = 5.0f;// �ӽð�
	Dir = float4::Right;
	BombScale = 120;
	WeaponName = "Grenade";

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // �̹��� �̸� ����or �Լ�ȭ �ʿ�

	//AllWeapons[WeaponName] = this;
	WeaponNumber = static_cast<int>(WeaponNum::Grenade);

    // ������ �ִϸ��̼� ����
    ExplosionCircle = CreateRender("circle50.bmp", WormsRenderOrder::Weapon);
    ExplosionCircle->CreateAnimation({ .AnimationName = "Explosion", .ImageName = "circle50.bmp", .Start = 0, .End = 8, .InterTime = 0.05f , .Loop = false });
    ExplosionCircle->CreateAnimation({ .AnimationName = "Idle", .ImageName = "circle50.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    ExplosionCircle->SetScale({ 240, 240 });

    ExplosionCircle->ChangeAnimation("Idle");
    ExplosionCircle->Off();

    ExplosionElipse = CreateRender("Elipse50.bmp", WormsRenderOrder::Weapon);
    ExplosionElipse->CreateAnimation({ .AnimationName = "ExplosionElipse", .ImageName = "Elipse50.bmp", .Start = 0, .End = 19, .InterTime = 0.03f , .Loop = false });
    ExplosionElipse->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Elipse50.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    ExplosionElipse->SetScale({ 360, 360 });

    ExplosionElipse->ChangeAnimation("Idle");
    ExplosionElipse->Off();

    PootTextAnimation = CreateRender("Poot.bmp", WormsRenderOrder::Weapon);
    PootTextAnimation->CreateAnimation({ .AnimationName = "Poot", .ImageName = "Poot.bmp", .Start = 0, .End = 17, .InterTime = 0.02f , .Loop = false });
    PootTextAnimation->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Poot.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    PootTextAnimation->SetScale({ 170, 170 });

    PootTextAnimation->ChangeAnimation("Idle");
    PootTextAnimation->Off();

    // ���� �ִϸ��̼�
    ChargeAnimation = CreateRender(WormsRenderOrder::Weapon);
    ChargeAnimation->CreateAnimation({ .AnimationName = "Charge", .ImageName = "ChargeAni.bmp",.FilterName = "ChargeAniRot.bmp", .Start = 0, .End = 15, .InterTime = 0.1f , .Loop = false });
    ChargeAnimation->ChangeAnimation("Charge");
    ChargeAnimation->SetScale({ 64,192 });
    ChargeAnimation->Off();
    ChargeAnimation->SetPosition({ 0,-10 });

    // �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
    AimingLine = CreateRender(WormsRenderOrder::Weapon);
    AimingLine->SetImage("AimingLine.bmp");
    AimingLine->SetRotFilter("AimingLineRot.bmp");
    AimingLine->SetScale({ 20,20 });

}

void WeaponGrenade::Update(float _DeltaTime)
{
	if (nullptr == WeaponRender) // ���� ������ �������� �ʴ� ���
	{
		WeaponGrenadeInit();
        SetCurPlayer();
        CurPlayer->ChangePlayerAnimation("GrenadeAim", static_cast<int>(AimIndex));
	}


    if (false == isFire)
    {
        //SetCurPlayer(); // �÷��̾� ��ȯ��ư ������ �߰�
        SetAimFrameIndex();
        if (AimIndex != NextAimIndex && CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON)
        {
            float Ratio = (6 * _DeltaTime > 1 ? 1 : 6 * _DeltaTime);
            AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);
            CurPlayer->ChangePlayerAnimation("GrenadeAim", static_cast<int>(AimIndex));
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
    }


	Firing(_DeltaTime);

    if (true == isExplosion)
    {
        if (true == ExplosionCircle->IsAnimationEnd())
        {
            ExplosionCircle->Off();
        }
        if (true == ExplosionElipse->IsAnimationEnd())
        {
            ExplosionElipse->Off();
        }
        if (true == PootTextAnimation->IsAnimationEnd())
        {
            PootTextAnimation->Off();
        }
    }
}


void WeaponGrenade::SetAimFrameIndex()
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

void WeaponGrenade::Firing(float _DeltaTime)
{
	if (false == isFire) // �߻��ϱ� ��
	{
		float4 PlayerPos = CurPlayer->GetPos();
        Dir = GetShootDir();

		SetPos(PlayerPos);
		if (true == PressShoot())
		{
            if (false == ChargeAnimation->IsUpdate())
            {
                ChargeAnimation->On();
                ChargeAnimation->ChangeAnimation("Charge", 0);
                ChargeAnimation->SetAngle(270 - Dir.GetAnagleDeg());
            }
			SetCharge();// ��¡����Ʈ ���
		}
		if (isEndCharging() == true) // �߻�üũ
		{
            ChargeAnimation->Off();
            Dir *= Charge;
            WeaponRender->On();
            AimingLine->Off();
			isFire = true;
		}

	}

	else // �߻� ��
	{
        // ī�޶� �̵�
        GetLevel()->SetCameraPos(WeaponRender->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half());

		Timer -= _DeltaTime;
		if (false == isExplosion)// && true == isFire)
		{
			Dir.y = Dir.y + Gravity * _DeltaTime; // dt���� �߷��� ����

            WeaponRender->SetAngle(Dir.GetAnagleDeg());
			WeaponRender->SetMove(Dir * MoveSpeed  * _DeltaTime);
			WeaponCollision->SetMove(Dir * MoveSpeed *_DeltaTime);

            if (Timer > 5.0f - 0.1f)
            {
                return;
            }
			float4 CheckedCol = CheckCollisionSide(WeaponCollision);
			if (Dir.x * CheckedCol.x > 0) // ������ �޶�
			{
				WeaponRender->SetMove(-Dir * MoveSpeed * _DeltaTime);
				WeaponCollision->SetMove(-Dir * MoveSpeed * _DeltaTime);

				Dir.x = -Dir.x * 0.5f;										  //  x���� ���������
				Dir.y = -Dir.y * 0.25f;
			}

			else if (Dir.x * CheckedCol.x < 0 || CheckedCol.x == 0 && CheckedCol.y > 0)// ������ ���� or �¿� �̵�x
			{
				WeaponRender->SetMove({ 0, -Dir.y * MoveSpeed * _DeltaTime });
				WeaponCollision->SetMove({ 0, -Dir.y * MoveSpeed * _DeltaTime });
				Dir.x = Dir.x * 0.5f;
				Dir.y = -Dir.y * 0.25f;
			}
		}

        // ���� üũ
        if (Timer < 0 && isExplosion == false)
        {
            GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();
            BombCollision->SetPosition(GetPos() + WeaponCollision->GetPosition());

            AttackPlayer(BombCollision,BombScale); // �ӽð�

            ExplosionCircle->SetPosition(WeaponRender->GetPosition());
            ExplosionCircle->On();
            ExplosionCircle->ChangeAnimation("Explosion", 0);

            ExplosionElipse->SetPosition(WeaponRender->GetPosition());
            ExplosionElipse->On();
            ExplosionElipse->ChangeAnimation("ExplosionElipse", 0);

            PootTextAnimation->SetPosition(WeaponRender->GetPosition());
            PootTextAnimation->On();
            PootTextAnimation->ChangeAnimation("Poot", 0);

            MapModifier::MainModifier->CreateHole(GetPos() + WeaponCollision->GetPosition(), 120);




            isExplosion = true;
            WeaponRender->Off();
            WeaponCollision->Off();
            isWeaponDone = true;
            GetLevel()->SetCameraPos(GetPos() - GameEngineWindow::GetScreenSize().half()); //���� �� Player�� ī�޶� �̵�- �����ʿ�
        }
	}
}

void WeaponGrenade::SetCharge() // Charging���� �Լ��̸� ����
{
	if (Charge > GetChargeTime())
	{
		return;
	}
	if (GetChargeTime() > 1.5f) // �ִ� 2�� ��¡
	{
		Charge = 1.5f;
	}
	else if (GetChargeTime() < 0.5f)
	{
		Charge = 0.5f;
	}
	else {
		Charge = GetChargeTime();
	}
}

void WeaponGrenade::ResetWeapon()
{

	isFire = false;
	isExplosion = false;
	Timer = 5.0f;
	if (nullptr == WeaponRender)
	{
		return;
	}
	WeaponRender->On();
	WeaponRender->SetPosition(float4::Zero);

	WeaponCollision->On();
	WeaponCollision->SetPosition(float4::Zero);

}

void WeaponGrenade::WeaponGrenadeInit()
{
	WeaponRender = CreateRender(WormsRenderOrder::Weapon);		//����
	WeaponRender->SetImage("Grenade.bmp");
    WeaponRender->SetRotFilter("GrenadeRot.bmp");
	WeaponRender->SetScale({ 15,25 }); // �ӽ� ���� �� 

	WeaponCollision = CreateCollision(WormsCollisionOrder::Weapon);	//�ݸ���
	WeaponCollision->SetScale(WeaponRender->GetScale());
    WeaponRender->Off();
	isFire = false;
}
