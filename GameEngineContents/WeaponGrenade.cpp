#include "WeaponGrenade.h"
#include "ContentsEnums.h"
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineInput.h>

#include <GameEnginePlatform/GameEngineWindow.h>
#include "Player.h"
#include "MapModifier.h"
#include "GlobalValue.h"


WeaponGrenade::WeaponGrenade() 
{
}

WeaponGrenade::~WeaponGrenade() 
{
}

void WeaponGrenade::Start()
{
	// ����ź �⺻ ���� -- ����� ���� �ʿ�
	MoveSpeed = 1200; // �ӽð�
	Gravity = 1.2f;// �ӽð�
	Timer = 3.0f;// �ӽð�
	WeaponName = "Grenade";
	BombScale = 147;

    MaxDmg = 75;
    MinDmg = 35;

    MaxKnockBackPower = 294;
    MinKnockBackPower = 146;

	MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // �̹��� �̸� ����or �Լ�ȭ �ʿ�
	WeaponNumber = static_cast<int>(WeaponNum::Grenade);



    // ���� �ִϸ��̼�
    ChargeAnimation = CreateRender(WormsRenderOrder::Weapon);
    ChargeAnimation->CreateAnimation({ .AnimationName = "Charge", .ImageName = "ChargeAni.bmp",.FilterName = "ChargeAniRot.bmp", .Start = 0, .End = 15, .InterTime = 0.07f , .Loop = false });
    ChargeAnimation->ChangeAnimation("Charge");
    ChargeAnimation->SetScale({ 64,192 });
    ChargeAnimation->Off();
    ChargeAnimation->SetPosition({ 0,-10 });

    // �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ
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
        else   // �߻� �ǰ� ���� ��
        {
            CheckExplosion();
        }
    }
}

void WeaponGrenade::CheckExplosion()
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
    if (WaitTime < GetLiveTime())
    {
        isWeaponDone = true;
        GetLevel()->SetCameraPos(GetPos() - GameEngineWindow::GetScreenSize().half()); //���� �� Player�� ī�޶� �̵�- �����ʿ�
    }
}

void WeaponGrenade::Aiming(float _DeltaTime)
{

    if (CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON) // ���� �÷��̾ ���� State
    {
        // ��ġ
        float4 PlayerPos = CurPlayer->GetPos() + float4{ 0,-15 };
        Dir = GetShootDir();
        SetPos(PlayerPos);


        // ���ؼ�
        SetAimFrameIndex();// Aim ���ؼ� index ã��
        AimingLine->On();
        // lerf�� ������ ����
        float Ratio = (3 * _DeltaTime > 1 ? 1 : 3 * _DeltaTime);
        AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);

        // FrameIndex ����
        CurPlayer->ChangePlayerAnimation("GrenadeAim", static_cast<int>(AimIndex));
        AimingLine->SetPosition(Dir * 150); // ���ؼ� �̵�
        if (Dir.x > 0)
        {
            AimingLine->SetAngle(Dir.GetAnagleDeg());
        }
        else
        {
            AimingLine->SetAngle(-Dir.GetAnagleDeg());
        }


        // �߻� üũ
        if (true == PressShoot()) // IsPress
        {
            if (false == isPress)
            {
                ChargeAnimation->On();
                ChargeAnimation->ChangeAnimation("Charge", 0);
                ChargeAnimation->SetAngle(270 - Dir.GetAnagleDeg());
                isPress = true;
            }
            SetCharge();// ��¡����Ʈ ���
        }
        if (true == isEndCharging()) // IsUp
        {
            ChargeAnimation->Off();
            AimingLine->Off();

            WeaponRender->On();
            WeaponCollision->On();

            MoveSpeed *= Charge;
            isFire = true;
        }

    }
    else
    {
        AimingLine->Off();
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

void WeaponGrenade::Firing(float _DeltaTime)// �߻� �� isFire == true
{
    // ī�޶� �̵�
    float4 CamPos = float4::Zero.Lerp(GetLevel()->GetCameraPos(), WeaponRender->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half(), _DeltaTime * 10);
    GetLevel()->SetCameraPos(CamPos);

    Timer -= _DeltaTime;
    if (false == isExplosion) // ���� �������� �ʾҴٸ�
    {
        Dir.y = Dir.y + Gravity * _DeltaTime; // dt���� �߷��� ���� V == Vo + At

        float4 MoveVec = CulWindMoveVec(_DeltaTime);

        WeaponRender->SetAngle(Dir.GetAnagleDeg());

        WeaponRender->SetMove(MoveVec);         //WeaponRender->SetMove(Dir * MoveSpeed * _DeltaTime);
        WeaponCollision->SetMove(MoveVec);      //WeaponCollision->SetMove(Dir * MoveSpeed * _DeltaTime);

        if (Timer > 3.0f - 0.08f) // ������ ( ���� ������ ���� )
        {
            return;
        }


        float4 CheckedCol = CheckCollisionSide(WeaponCollision);

        if (Dir.x * CheckedCol.x > 0) // ������ �޶�
        {
            WeaponRender->SetMove(-MoveVec);    //WeaponRender->SetMove(-Dir * MoveSpeed * _DeltaTime);
            WeaponCollision->SetMove(-MoveVec); //WeaponCollision->SetMove(-Dir * MoveSpeed * _DeltaTime);

            Dir.x = -Dir.x * 0.5f;										  //  x���� ���������
            Dir.y = -Dir.y * 0.25f;
        }
        else if (Dir.x * CheckedCol.x < 0 || CheckedCol.x == 0 && CheckedCol.y > 0)// ������ ���� or �¿� �̵�x
        {
            WeaponRender->SetMove({ 0, -MoveVec.y });       //WeaponRender->SetMove({ 0, -Dir.y * MoveSpeed * _DeltaTime });
            WeaponCollision->SetMove({ 0, -MoveVec.y });    //WeaponCollision->SetMove({ 0, -Dir.y * MoveSpeed * _DeltaTime });
            Dir.x = Dir.x * 0.5;
            Dir.y = -Dir.y * 0.25f;
        }
        else if(CheckedCol == float4::Up)
        {
            WeaponRender->SetMove(-MoveVec);
            WeaponCollision->SetMove(-MoveVec);
            Dir *= 0.1f;
        }
    }

    // ���� üũ
    if (Timer < 0 && isExplosion == false) //����, isExplosion�ߺ� üũ
    {
        // ������ �ִϸ��̼�
        ExplosionCircle->SetPosition(WeaponRender->GetPosition());
        ExplosionCircle->On();
        ExplosionCircle->ChangeAnimation("Explosion", 0);

        ExplosionElipse->SetPosition(WeaponRender->GetPosition());
        ExplosionElipse->On();
        ExplosionElipse->ChangeAnimation("ExplosionElipse", 0);

        PootTextAnimation->SetPosition(WeaponRender->GetPosition());
        PootTextAnimation->On();
        PootTextAnimation->ChangeAnimation("Poot", 0);

        //Player ����
        GameEngineCollision* BombCollision = MapModifier::MainModifier->GetModifierCollision();
        BombCollision->SetPosition(GetPos() + WeaponCollision->GetPosition());
        BombCollision->SetScale(float4{ static_cast<float>(BombScale) });
        AttackPlayer(BombCollision); // �ӽð�
        BombCollision->SetPosition(float4::Zero);

        // �� ���̰�
        MapModifier::MainModifier->CreateHole(GetPos() + WeaponCollision->GetPosition(), BombScale);




        isExplosion = true;
        WeaponRender->Off();
        WeaponCollision->Off();
        WaitTime = GetLiveTime() + 1.5;

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
	Timer = 3.0f;
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
    WeaponCollision->SetScale({15,25});
    WeaponRender->Off();
    WeaponCollision->Off();
	isFire = false;
    ExplosionEffectInit(BombScale);
}

float4 WeaponGrenade::CulWindMoveVec(float _DeltaTime)
{
    //if (abs(Dir.x * MoveSpeed) > 2000)
    //{
    //    return Dir * MoveSpeed * _DeltaTime;
    //}
    //Dir.x += GlobalValue::gValue.GetWindPhase() * 0.1f * _DeltaTime; ����
    float4 ReturnVec = Dir * MoveSpeed * _DeltaTime;
    ReturnVec.x += (GlobalValue::gValue.GetWindPhase() / 7) * (MoveSpeed / 2) * _DeltaTime; // ���
    return ReturnVec;
}