#include "WeaponClusterBomb.h"
#include "Player.h"
#include "MapModifier.h"
#include "ContentsEnums.h"
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include "GlobalValue.h"

WeaponClusterBomb::WeaponClusterBomb()
{
}

WeaponClusterBomb::~WeaponClusterBomb()
{
}

void WeaponClusterBomb::Start()
{
    // ����ź �⺻ ���� -- ����� ���� �ʿ�
    MoveSpeed = 1200.0f; // �ӽð�
    Gravity = 1.0f;// �ӽð�
    Timer = 1.2f;// �ӽð�
    //Dir = float4::Right;
    BombScale = 61;

    MaxDmg = 30;
    MinDmg = 14;
    MaxKnockBackPower = 122;
    MinKnockBackPower = 70;
    WeaponName = "ClusterBomb";

    MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp"); // �̹��� �̸� ����or �Լ�ȭ �ʿ�
    WeaponNumber = static_cast<int>(WeaponNum::ClusterBomb);

    // ������ �ִϸ��̼� ����
    ExplosionCircle = CreateRender("circle50.bmp", WormsRenderOrder::Weapon);
    ExplosionCircle->CreateAnimation({ .AnimationName = "Explosion", .ImageName = "circle50.bmp", .Start = 0, .End = 8, .InterTime = 0.05f , .Loop = false });
    ExplosionCircle->CreateAnimation({ .AnimationName = "Idle", .ImageName = "circle50.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    ExplosionCircle->SetScale({ 104, 104 });

    ExplosionCircle->ChangeAnimation("Idle");
    ExplosionCircle->Off();

    ExplosionElipse = CreateRender("Elipse50.bmp", WormsRenderOrder::Weapon);
    ExplosionElipse->CreateAnimation({ .AnimationName = "ExplosionElipse", .ImageName = "Elipse50.bmp", .Start = 0, .End = 19, .InterTime = 0.03f , .Loop = false });
    ExplosionElipse->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Elipse50.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    ExplosionElipse->SetScale({ 104 * 1.5f, 104 * 1.5f });

    ExplosionElipse->ChangeAnimation("Idle");
    ExplosionElipse->Off();

    BiffTextAnimation = CreateRender("Biff.bmp", WormsRenderOrder::Weapon);
    BiffTextAnimation->CreateAnimation({ .AnimationName = "Biff", .ImageName = "Biff.bmp", .Start = 0, .End = 11, .InterTime = 0.02f , .Loop = false });
    BiffTextAnimation->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Biff.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    BiffTextAnimation->SetScale({ 104, 104 });

    BiffTextAnimation->ChangeAnimation("Idle");
    BiffTextAnimation->Off();

    
    PowTextAnimation = CreateRender("Pow.bmp", WormsRenderOrder::Weapon);
    PowTextAnimation->CreateAnimation({ .AnimationName = "Pow", .ImageName = "Pow.bmp", .Start = 0, .End = 11, .InterTime = 0.02f , .Loop = false });
    PowTextAnimation->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Pow.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    PowTextAnimation->SetScale({ BombScale, BombScale });

    PowTextAnimation->ChangeAnimation("Idle");
    PowTextAnimation->Off();


    // ���� �ִϸ��̼�
    ChargeAnimation = CreateRender(WormsRenderOrder::Weapon);
    ChargeAnimation->CreateAnimation({ .AnimationName = "Charge", .ImageName = "ChargeAni.bmp",.FilterName = "ChargeAniRot.bmp", .Start = 0, .End = 15, .InterTime = 0.1f , .Loop = false });
    ChargeAnimation->ChangeAnimation("Charge");
    ChargeAnimation->SetScale({64,192});
    ChargeAnimation->Off();
    //ChargeAnimation->SetPosition({ 0,-10 });


    // �ӽ� ���ؼ� - �����ʿ� : ���ؼ� ���� ��ġ, �̹��� , �̹��� ����
    AimingLine = CreateRender(WormsRenderOrder::Weapon);
    AimingLine->SetImage("AimingLine.bmp");
    AimingLine->SetRotFilter("AimingLineRot.bmp");
    AimingLine->SetScale({ 20,20 });

    // Ÿ�̸�
    TimerRenderBack = CreateRender("TimerBack.bmp", WormsRenderOrder::UI);
    TimerRenderBack->SetScaleToImage();
    TimerRenderBack->Off();

    TimerRender.SetOwner(this);
    TimerRender.SetCameraEffect(true);
    TimerRender.SetImage("RedNumberRender.bmp", { 15, 15 }, 10, RGB(255, 0, 255));
    TimerRender.SetAlign(Align::Center);
    TimerRender.SetValue(static_cast<int>(Timer));
    TimerRender.Off();

}

void WeaponClusterBomb::Update(float _DeltaTime)
{
    if (nullptr == WeaponRender) // ���� ������ �������� �ʴ� ���
    {
        WeaponClusterBombInit();
        SetCurPlayer();
    }

    if (false == isFire) //�߻簡 �ȵǾ��ٸ�
    {
        Aiming(_DeltaTime);
    }
    else // true == isFire
    {
        if (false == isExplosion) // �߻�ǰ� ���� ������ ��
        {
            CheckTimer(_DeltaTime);
            Firing(_DeltaTime);
        }
        else   
        {
            CheckAnimationDone();
        }
    }

    if (true == isClusterFire)
    {

        ClusterFiring(_DeltaTime);

        if (true == isClusterExplosion)
        {
            if (0.0f == WaitTime)
            {
                WaitTime = GetLiveTime() + 1.5f;
            }
            else if (GetLiveTime() > WaitTime && true == isDone())
            {
                isWeaponDone = true;
                GetLevel()->SetCameraPos(GetPos() - GameEngineWindow::GetScreenSize().half()); //���� �� Player�� ī�޶� �̵�- �����ʿ�

            }
        }
    }


}
void WeaponClusterBomb::CheckAnimationDone()
{
    if (true == ExplosionCircle->IsAnimationEnd())
    {
        ExplosionCircle->Off();
        ExplosionCircle->ChangeAnimation("Idle");
    }
    if (true == ExplosionElipse->IsAnimationEnd())
    {
        ExplosionElipse->Off();
        ExplosionElipse->ChangeAnimation("Idle");
    }
    if (true == BiffTextAnimation->IsAnimationEnd())
    {
        BiffTextAnimation->Off();
        BiffTextAnimation->ChangeAnimation("Idle");
    }
    if (true == PowTextAnimation->IsAnimationEnd())
    {
        PowTextAnimation->Off();
        PowTextAnimation->ChangeAnimation("Idle");
    }
}

void WeaponClusterBomb::Aiming(float _DeltaTime)
{
    if (CurPlayer->GetPlayerState() == PlayerState::EQUIPWEAPON) // ���� �÷��̾ ���� State
    {
        if (isPress == false)
        {
            // ��ġ
            float4 PlayerPos = CurPlayer->GetPos() + float4{ 0,-15 };
            Dir = GetShootDir();
            SetPos(PlayerPos);

            // ���ؼ�
            SetAimFrameIndex();
            AimingLine->On();

            // lerf�� ������ ����
            float Ratio = (3 * _DeltaTime > 1 ? 1 : 3 * _DeltaTime);
            AimIndex = AimIndex * (1.0f - Ratio) + (NextAimIndex * Ratio);

            // FrameIndex ����
            CurPlayer->ChangePlayerAnimation("ClusterBombAim", static_cast<int>(AimIndex));
            AimingLine->SetPosition(Dir * 150 + float4{ 0,15 }); // ���ؼ� �̵�
            AimingLine->SetAngle(-Dir.GetAnagleDeg());
        }

        // �߻� üũ
        if (true == PressShoot())
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

        if (isEndCharging() == true) // �߻�üũ2
        {
            ChargeAnimation->Off();
            AimingLine->Off();

            WeaponRender->On();
            WeaponCollision->On();
            NextPosCheckCollision->On();

            MoveSpeed *= Charge;
            isFire = true;
            // Ÿ�̸ӷ���
            TimerRenderBack->On();
            TimerRender.On();
        }

    }
    else
    {
        AimingLine->Off();
    }
}

void WeaponClusterBomb::SetAimFrameIndex()
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

void WeaponClusterBomb::ClusterFiring(float _DeltaTime)
{

    // Ŭ������ ī�޶� �̵� 
    float4 CamPos = float4::Zero.Lerp(GetLevel()->GetCameraPos(), ClusterRender[0]->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half(), _DeltaTime * 10);
    GetLevel()->SetCameraPos(CamPos);
    for (int i = 0;i < ClusterRender.size();i++)
    {
        if (false == ClusterCollision[i]->IsUpdate())
        {
            continue;
        }
        ClusterDir[i].y = ClusterDir[i].y + Gravity * 10 * _DeltaTime;

        ClusterRender[i]->SetAngle(ClusterDir[i].GetAnagleDeg());
        ClusterRender[i]->SetMove(ClusterDir[i] * _DeltaTime * ClusterSpeed);
        ClusterCollision[i]->SetMove(ClusterDir[i] * _DeltaTime * ClusterSpeed);
        MakeSmoke();

        float4 CheckedCol = CheckCollisionSide(ClusterCollision[i]);
        if (CheckedCol.Size() > 0) // �ݸ��� üũ(�÷��̾�, ��, ��ü �� ��)
        {
            isClusterExplosion = true;
            ClusterCollision[i]->SetScale(float4{ static_cast<float>(BombScale) });

            AttackPlayer(ClusterCollision[i]); // �ӽð�

            // �ణ�� ������ �ʿ���
            ExplosionCircle->SetPosition(ClusterCollision[i]->GetPosition());
            ExplosionCircle->On();
            ExplosionCircle->ChangeAnimation("Explosion", 0);

            ExplosionElipse->SetPosition(ClusterCollision[i]->GetPosition());
            ExplosionElipse->On();
            ExplosionElipse->ChangeAnimation("ExplosionElipse", 0);

            PowTextAnimation->SetPosition(ClusterCollision[i]->GetPosition());
            PowTextAnimation->On();
            PowTextAnimation->ChangeAnimation("Pow", 0);

            MapModifier::MainModifier->CreateHole(ClusterCollision[i]->GetActorPlusPos(), static_cast<int>(BombScale));
            ClusterRender[i]->Off();
            ClusterCollision[i]->Off();
        }

    }
    
}


void WeaponClusterBomb::Firing(float _DeltaTime)
{
    // �߻��� true == isFire 

    // ī�޶� �̵�
    float4 CamPos = float4::Zero.Lerp(GetLevel()->GetCameraPos(), WeaponRender->GetActorPlusPos() - GameEngineWindow::GetScreenSize().half(), _DeltaTime * 10);
    GetLevel()->SetCameraPos(CamPos);

    // Ÿ�̸�
    Timer -= _DeltaTime;
    TimerRenderBack->SetPosition(WeaponRender->GetPosition() - float4{ 30,30 });
    TimerRender.SetRenderPos(WeaponRender->GetPosition() - float4{ 30,30 });
    if (static_cast<int>(Timer) + 1 >= 0)
    {
        TimerRender.SetValue(static_cast<int>(Timer) + 1);

    }

    if (false == isExplosion)// && true == isFire)
    {
        Dir.y += Gravity * _DeltaTime;
        Dir.x += GlobalValue::gValue.GetWindPhase() / 10 * _DeltaTime;
        float4 MoveVec = Dir * MoveSpeed * _DeltaTime;
        float4 CheckCol = Check4Side(WeaponCollision, WeaponCollision->GetActorPlusPos() + MoveVec);
        if (CheckCol.AddAllVec() == 0)
        {
            WeaponRender->SetAngle(Dir.GetAnagleDeg());
            WeaponRender->SetMove(MoveVec);
            WeaponCollision->SetMove(MoveVec);
            return;
        }
        else if (1 == CheckCol.AddAllVec() || 2 == CheckCol.AddAllVec())
        {
            if (1 == CheckCol.z)
            {
                Dir.y = -abs(Dir.y) * 0.25f;
            }
            if (1 == CheckCol.w)
            {
                Dir.y = abs(Dir.y) * 0.25f;
            }

            if (1 == CheckCol.x)
            {
                Dir.x = -abs(Dir.x * 0.5f);
            }
            if (1 == CheckCol.y)
            {
                Dir.x = abs(Dir.x * 0.5f);
            }

            return;
        }
        else if (3 == CheckCol.AddAllVec())
        {
            if (0 == CheckCol.z)
            {
                Dir.y = abs(Dir.y) * 0.25f;
            }
            if (0 == CheckCol.w)
            {
                Dir.y = -abs(Dir.y) * 0.25f;
            }

            if (0 == CheckCol.x)
            {
                Dir.x = abs(Dir.x * 0.5f);
            }
            if (0 == CheckCol.y)
            {
                Dir.x = -abs(Dir.x * 0.5f);
            }
        }
        else
        {
            Dir.x = -Dir.x * 0.5f;
            Dir.y = -Dir.y * 0.25f;
        }


    }

}

void WeaponClusterBomb::SetCharge() // Charging���� �Լ��̸� ����
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

void WeaponClusterBomb::CheckTimer(float _DeltaTime)
{
    //Ÿ�̸� üũ -> ū ���� üũ
    if (Timer < 0 && isExplosion == false)
    {
        TimerRenderBack->Off();
        TimerRender.Off();
        GameEngineCollision* BombCollision = CreateCollision(WormsCollisionOrder::Weapon);
        BombCollision->SetPosition(WeaponCollision->GetPosition());
        BombCollision->SetScale(float4{ static_cast<float>(MainBombScale) });

        AttackPlayer(BombCollision);// �ӽð�

        ExplosionCircle->SetPosition(WeaponRender->GetPosition());
        ExplosionCircle->On();
        ExplosionCircle->ChangeAnimation("Explosion", 0);
        ExplosionCircle->SetScale({ BombScale,BombScale });

        ExplosionElipse->SetPosition(WeaponRender->GetPosition());
        ExplosionElipse->On();
        ExplosionElipse->ChangeAnimation("ExplosionElipse", 0);
        ExplosionElipse->SetScale({ BombScale,BombScale });

        BiffTextAnimation->SetPosition(WeaponRender->GetPosition());
        BiffTextAnimation->On();
        BiffTextAnimation->ChangeAnimation("Biff", 0);

        MapModifier::MainModifier->CreateHole(GetPos() + WeaponCollision->GetPosition(), MainBombScale);
        isExplosion = true;
        WeaponRender->Off();
        WeaponCollision->Off();

        ClusterOn(WeaponRender->GetPosition());
        isClusterFire = true;
    }

}


void WeaponClusterBomb::WeaponClusterBombInit()
{
    WeaponRender = CreateRender(WormsRenderOrder::Weapon);		//����
    WeaponRender->SetImage("Clusterbomb.bmp");
    WeaponRender->SetRotFilter("ClusterbombRot.bmp");
    WeaponRender->SetPosition(float4{ 0,-30 });
    WeaponRender->SetScale({ 30,30 }); // �ӽ� ���� �� 

    WeaponCollision = CreateCollision(WormsCollisionOrder::Weapon);	//�ݸ���
    WeaponCollision->SetScale({15,25});
    WeaponCollision->SetPosition(float4{ 0,-30 });

    NextPosCheckCollision = CreateCollision(WormsCollisionOrder::Weapon);	//�ݸ���
    NextPosCheckCollision->SetScale({ 15,25 });
    NextPosCheckCollision->SetPosition(float4{ 0,-30 });

    isFire = false;
    WeaponRender->Off();
    WeaponCollision->Off();
    NextPosCheckCollision->Off();

    for (int i = 0;i < 6;i++)
    {
        GameEngineRender* NewClusterRender = CreateRender("cluster.bmp", WormsRenderOrder::Weapon);
        NewClusterRender->SetScale({ 24,24 });
        NewClusterRender->SetRotFilter("clusterRot.bmp");

        GameEngineCollision* NewClusterCollision = CreateCollision(WormsCollisionOrder::Weapon);
        NewClusterCollision->SetScale(NewClusterRender->GetScale());

        ClusterRender.push_back(NewClusterRender);
        ClusterCollision.push_back(NewClusterCollision);
        ClusterDir.push_back(float4::Zero);
        ClusterSmokeInterval.push_back(float4::Zero);

    }
    ClusterOff();
}

void WeaponClusterBomb::MakeSmoke()
{
    for (int i = 0;i < ClusterRender.size();i++)
    {
        float4 ClusterPos = ClusterRender[i]->GetPosition();
        if ((ClusterPos - ClusterSmokeInterval[i]).Size() > 70)
        {
            ClusterSmokeInterval[i] = ClusterPos;
            GameEngineRender* Smoke = CreateRender("BazSmoke.bmp", static_cast<int>(WormsRenderOrder::Weapon));
            Smoke->SetPosition(ClusterPos);
            Smoke->SetScale({ 60, 60 });
            Smoke->CreateAnimation({ .AnimationName = "Smoke", .ImageName = "BazSmoke.bmp", .Start = 0, .End = 63, .InterTime = 0.00001f , .Loop = false });
            Smoke->SetAlpha(GameEngineRandom::MainRandom.RandomInt(100, 160));
            Smoke->ChangeAnimation("Smoke");
        }
    }
}


void WeaponClusterBomb::ClusterOn(float4 _Pos)
{
    for (int i = 0;i < ClusterRender.size();i++)
    {
        ClusterRender[i]->On();
        ClusterCollision[i]->On();
        float Randx = GameEngineRandom::MainRandom.RandomFloat(-BombScale *0.5f, BombScale * 0.5f);
        float Randy = GameEngineRandom::MainRandom.RandomFloat(-BombScale * 0.5f, BombScale * 0.5f);
        float4 RandomPos = float4{ Randx, Randy };

        ClusterDir[i] = RandomPos.NormalizeReturn();
        ClusterRender[i]->SetPosition(_Pos/*+ RandomPos*/);
        ClusterCollision[i]->SetPosition(_Pos/*+ RandomPos*/);
        ClusterSmokeInterval[i] = _Pos;
    }
}
void WeaponClusterBomb::ClusterOff()
{
    for (int i = 0;i < ClusterRender.size();i++)
    {
        ClusterRender[i]->Off();
        ClusterCollision[i]->Off();
    }
}

bool WeaponClusterBomb::isDone()
{
    if (true == isFire && true == isExplosion && true == isClusterFire)
    {
        for (int i = 0;i < ClusterCollision.size();i++)
        {
            if (true == ClusterCollision[i]->IsUpdate())
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
