#include "WeaponAirStrike.h"
#include "ContentsEnums.h"
#include "Player.h"
#include "MapModifier.h"
#include "Map.h"

#include <cmath>

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineResources.h>

WeaponAirStrike::WeaponAirStrike()
{
}

WeaponAirStrike::~WeaponAirStrike()
{
}

void WeaponAirStrike::Start()
{
	WeaponAirStrikeInit();
    DebrisInit();
    MarkerInit();

    PrevTime = static_cast<float>(clock());
}

void WeaponAirStrike::Update(float _DeltaTime)
{
    Timer();

	Attack(_DeltaTime);
    DebrisAnimation(_DeltaTime);
    ExplosionAnimationOff();
	CameraUpdate(_DeltaTime);
}

void WeaponAirStrike::Render(float _DeltaTime)
{

}

void WeaponAirStrike::WeaponAirStrikeInit()
{
	SetCurPlayer();

	Airplane = CreateRender("Airplane.bmp", WormsRenderOrder::Weapon);
	Airplane->SetScaleToImage();
	Airplane->Off();

    std::string Name = Map::MainMap->GetColMapName();
    MapCollision = GameEngineResources::GetInst().ImageFind(Name);

	ScreenSize = GameEngineWindow::GetScreenSize();

	WeaponNumber = static_cast<int>(WeaponNum::AirStrike);

    BombScale = 122.0f;

    MinDmg = 24;
    MaxDmg = 45;

    WeaponName = "AirStrike";

    MissileNum = 5;
    MissileList.reserve(MissileNum);

    MissileInit();
    ExplosionEffectInit();
}

void WeaponAirStrike::Attack(float _DeltaTime)
{
	if (GameEngineInput::IsKey("LeftMouseClick") == false)
	{
		GameEngineInput::CreateKey("LeftMouseClick", VK_LBUTTON);
	}

	if (GameEngineInput::IsDown("LeftMouseClick") == true)
	{
		SetAirPlanePos();
	}

	AirPlaneMove(_DeltaTime);

	if (isHideAirPlane == true && isMissileSet == false)
	{
		SetMissiles();
	}

	else if (isHideAirPlane == true && MissileList.size() > 0)
	{
		Firing(_DeltaTime);
	}

	Explosion();
}

void WeaponAirStrike::SetAirPlanePos()
{
	TargetPos = GetLevel()->GetMousePosToCamera();

	AirPlaneStartPos.x = TargetPos.x;
	AirPlaneStartPos.y = -200.0f;

	if (AirPlaneStartPos.x > CurPlayer->GetPos().x)
	{
		isMoveRight = true;
        Airplane->SetImage("AirPlaneRight.bmp");
	}
	else
	{
		isMoveRight = false;
        Airplane->SetImage("AirPlane.bmp");
	}

	if (isMoveRight == true)
	{
		AirPlaneStartPos.x -= 1000.0f;
	}
	else
	{
		AirPlaneStartPos.x += 1000.0f;
	}

	Airplane->SetPosition(AirPlaneStartPos);
    Marker->SetPosition(TargetPos);
    Marker->ChangeAnimation("MarkerOn");
    Marker->On();
}

void WeaponAirStrike::AirPlaneMove(float _DeltaTime)
{
    float MovePos = Airplane->GetPosition().x - AirPlaneStartPos.x;

    if (abs(MovePos) > 2000.0f)
    {
        Airplane->Off();
        return;
    }

    else if (abs(MovePos) > 500.0f)
    {
        isHideAirPlane = true;
    }

    if (GameEngineInput::IsDown("Shoot") == true)
    {
        CurPlayer->SetCanIMove(false);

        Airplane->On();
        Marker->Off();

        GameEngineResources::GetInst().SoundPlay("Airstrike.wav");
        isFire = true;
    }

	if (Airplane->IsUpdate() == true)
	{
		if (isMoveRight == true)
		{
			Airplane->SetMove(float4::Right * 1500.0f * _DeltaTime);
		}

		else
		{
			Airplane->SetMove(float4::Left * 1500.0f * _DeltaTime);
		}
	}
}

void WeaponAirStrike::Firing(float _DeltaTime)
{	
	if(Dir == float4{0,0})
	{
		//Dir = TargetPos - MiddleMissileStartPos;
		//Dir.Normalize();
        if (isMoveRight == true)
        {
            Dir = float4::Right;
        }
        else
        {
            Dir = float4::Left;
        }

		for (int i = 0; i < MissileList.size(); i++)
		{
			MissileList[i]->On();
		}

        PrevAirPlanePos = MissileList[0]->GetActorPlusPos();
	}

	//Gravity += 0.02f * _DeltaTime;
	//Dir += float4{0, Gravity};
 //   Dir.Normalize();
    Gravity += 25.0f * _DeltaTime;

	for (int i = 0; i < MissileList.size(); i++)
	{
		MissileList[i]->SetMove(Dir * 700.0f * _DeltaTime + float4{0, Gravity});
		MissileCollisionList[i]->SetMove(Dir * 700.0f * _DeltaTime + float4{ 0, Gravity });
        
        CurAirPlanePos = MissileList[0]->GetActorPlusPos();

        float4 RotDir = CurAirPlanePos - PrevAirPlanePos;

        float rot = -RotDir.GetAnagleDeg();
        MissileList[i]->SetAngle(-RotDir.GetAnagleDeg());
	}

    PrevAirPlanePos = CurAirPlanePos;

	CurPos = MissileList[MissileNum / 2]->GetPosition();
}

void WeaponAirStrike::MissileInit()
{
    for (int i = 0; i < MissileNum; i++)
    {
        MissileList.push_back(CreateRender("AirMissile.bmp", WormsRenderOrder::Weapon));
        MissileList[i]->SetRotFilter("AirMissileRot.bmp");
        MissileList[i]->SetScaleToImage();
        MissileList[i]->Off();

        MissileCollisionList.push_back(CreateCollision(WormsCollisionOrder::Weapon));
        MissileCollisionList[i]->SetScale({ 30, 30 });
    }
}

void WeaponAirStrike::SetMissiles()
{
	float4 AirPlanePos = Airplane->GetPosition();
    float MissileXpos = 0.0f;

    if (isMoveRight == true)
    {
        MissileXpos = -120.0f;
    }
    else
    {
        MissileXpos = 120.0f;
    }

	for (int i = 0; i < MissileNum; i++)
	{
		MissileList[i]->SetPosition(AirPlanePos - float4{ MissileXpos, -50 });

		MissileCollisionList[i]->SetPosition(MissileList[i]->GetPosition());

		if (isMoveRight == true)
		{
			MissileXpos += 60.0f;
		}
		else
		{
			MissileXpos -= 60.0f;
		}
	}

	MiddleMissileStartPos = MissileList[MissileNum / 2]->GetPosition();

    isMissileSet = true;
}

void WeaponAirStrike::Explosion()
{
    if (ExplosionCount == MissileNum)
    {
        return;
    }

	for (int i = 0; i < MissileCollisionList.size(); i++)
	{
		if (MissileCollisionList[i]->IsUpdate() == true && CheckCollision(MissileCollisionList[i]) == true)
		{
            GameEngineResources::GetInst().SoundPlay("Explosion1.wav");

			MapModifier::MainModifier->CreateHole(MissileCollisionList[i]->GetActorPlusPos(), static_cast<int>(BombScale));
			MissileCollisionList[i]->Off();
			MissileList[i]->Off();

            DebrisSet(i);
            ExplosionAnimation(i);
            DamageToPlayer(i);

            ExplosionCount++;
		}
	}
}

void WeaponAirStrike::CameraUpdate(float _DeltaTime)
{
	if (Airplane->IsUpdate() == true && isHideAirPlane == false)
	{
        float4 AirPlanePos = Airplane->GetActorPlusPos();
        PrevCamPos = GetLevel()->GetCameraPos();
        fLerpRatio += _DeltaTime * fLerpSpeed;
        GetLevel()->SetCameraPos(LerpCamPos.LerpClamp(PrevCamPos, AirPlanePos - GameEngineWindow::GetScreenSize().half(), fLerpRatio));
	}
	else if (isHideAirPlane == true && MissileList[2]->IsUpdate() == true)
	{
		GetLevel()->SetCameraPos(MissileList[2]->GetActorPlusPos() - ScreenSize.half());

        PrevCamPos = { 0, 0 };
        fLerpRatio = 0.0f;
	}


    for (int i = 0; i < MissileNum; i++)
    {
        if (isDebrisSetList[i] == false)
        {
            return;
        }
    }

    CameraTimeCount += TimeCount;

    if (CameraTimeCount >= 2.0f && fLerpRatio < 1)
    {
        CurPlayerPos = CurPlayer->GetPos();
        PrevCamPos = GetLevel()->GetCameraPos();
        fLerpRatio += _DeltaTime * fLerpSpeed;
        GetLevel()->SetCameraPos(LerpCamPos.LerpClamp(PrevCamPos, CurPlayerPos - GameEngineWindow::GetScreenSize().half(), fLerpRatio));
    }

    if (fLerpRatio >= 1)
    {
        CurPlayer->ChangePlayerAnimation("AirStrikeOff");
        isWeaponDone = true;
    }

}



void WeaponAirStrike::DebrisSet(int MissileIndex)
{
    if(isDebrisSetList[MissileIndex] == false)
    {
        for (int i = MissileIndex * 9; i < (MissileIndex + 1) * 9; i++)
        {

            float X = GameEngineRandom::MainRandom.RandomFloat(-40, 40);
            float Y = GameEngineRandom::MainRandom.RandomFloat(-40, 40);

            Smokes[i]->SetPosition(MissileList[MissileIndex]->GetPosition() + float4{ X,Y });
            Smokes[i]->ChangeAnimation("Smoke");
            Smokes[i]->On();

            float4 Dir = float4{ X,Y };
            Dir.Normalize();

            SmokesDir[i] = Dir;
        }

        for (int i = MissileIndex * 10; i < (MissileIndex + 1) * 10; i++)
        {
            float X = GameEngineRandom::MainRandom.RandomFloat(-20, 20);
            float Y = GameEngineRandom::MainRandom.RandomFloat(-20, 20);

            Sparks[i]->SetPosition(MissileList[MissileIndex]->GetPosition() + float4{ X,Y });
            Sparks[i]->ChangeAnimation("Spark");
            Sparks[i]->On();

            float4 Dir = float4{ X,Y };
            Dir.Normalize();

            SparksDir[i] = Dir;
        }

        isDebrisSetList[MissileIndex] = true;
    }
}

void WeaponAirStrike::DebrisInit()
{
    for (int i = 0; i < MissileNum * 9; i++)
    {
        GameEngineRender* Smoke = CreateRender("Smoke100.bmp", WormsRenderOrder::Weapon);
        Smoke->CreateAnimation({ .AnimationName = "Smoke", .ImageName = "Smoke100.bmp", .Start = 0, .End = 27, .InterTime = 0.03f , .Loop = false });
        Smoke->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Smoke100.bmp", .Start = 0, .End = 0, .InterTime = 0.05f , .Loop = false });
        Smoke->SetScale({ 134, 134 });
        Smoke->Off();

        Smokes.push_back(Smoke);
        SmokesDir.push_back({ 0, 0 });
    }

    for (int i = 0; i < MissileNum * 10; i++)
    {
        GameEngineRender* Spark = CreateRender("Spark1.bmp", WormsRenderOrder::Weapon);
        Spark->CreateAnimation({ .AnimationName = "Spark", .ImageName = "Spark1.bmp", .Start = 0, .End = 31, .InterTime = 0.05f , .Loop = false });
        Spark->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Spark1.bmp", .Start = 0, .End = 0, .InterTime = 0.05f , .Loop = false });
        Spark->SetScale({ 60, 60 });
        Spark->Off();

        Sparks.push_back(Spark);
        SparksDir.push_back({ 0, 0 });
    }

    for (int i = 0; i < MissileNum; i++)
    {
        isDebrisSetList.push_back(false);
        DebrisGravityList.push_back(0);
    }
}

void WeaponAirStrike::DebrisAnimation(float _DeltaTime)
{
    if (isHideAirPlane == false)
    {
        return;
    }
    if (ExplosionCount == 0)
    {
        return;
    }

    for (int i = 0; i < MissileNum; i++)
    {
        if (isDebrisSetList[i] == false)
        {
            continue;
        }

        else
        {
            for (int Index = i * 10; Index < (i + 1) * 10; Index++)
            {
                Sparks[Index]->SetMove(SparksDir[Index] * 150.0f * _DeltaTime + float4{ 0, DebrisGravityList[i] } *_DeltaTime);
            }

            DebrisGravityList[i] += 250.0f * _DeltaTime;

            for (int Index = i * 9; Index < (i+1) * 9; Index++)
            {
                if (Smokes[Index]->IsAnimationEnd() == true)
                {
                    Smokes[Index]->Off();
                }
            }
        }
    }
}

void WeaponAirStrike::Timer()
{
    CurTime = static_cast<float>(clock());

    TimeCount = (CurTime - PrevTime) / 1000;

    PrevTime = CurTime;
}

void WeaponAirStrike::DamageToPlayer(int _Index)
{
    GameEngineCollision* HoleCollision = CreateCollision(WormsCollisionOrder::Weapon);
    HoleCollision->SetScale({ BombScale, BombScale });
    HoleCollision->SetPosition(MissileCollisionList[_Index]->GetActorPlusPos());

    AttackPlayer(HoleCollision);
}

void WeaponAirStrike::ExplosionEffectInit()
{

    for (int i = 0; i < MissileNum; i++)
    {
        GameEngineRender* Circle = CreateRender("circle50.bmp", WormsRenderOrder::Weapon);
        Circle->CreateAnimation({ .AnimationName = "Explosion", .ImageName = "circle50.bmp", .Start = 0, .End = 8, .InterTime = 0.05f , .Loop = false });
        Circle->CreateAnimation({ .AnimationName = "Idle", .ImageName = "circle50.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
        Circle->SetScale({ BombScale, BombScale });

        Circle->ChangeAnimation("Idle");
        Circle->Off();

        ExplosionCircleList.push_back(Circle);

        GameEngineRender* Elipse = CreateRender("Elipse75.bmp", WormsRenderOrder::Weapon);
        Elipse->CreateAnimation({ .AnimationName = "ExplosionElipse", .ImageName = "Elipse75.bmp", .Start = 0, .End = 9, .InterTime = 0.05f , .Loop = false });
        Elipse->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Elipse75.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
        Elipse->SetScale({ BombScale * 1.5f , BombScale * 1.5f });

        Elipse->ChangeAnimation("Idle");
        Elipse->Off();

        ExplosionElipseList.push_back(Elipse);

        GameEngineRender* PootText = CreateRender("Poot.bmp", WormsRenderOrder::Weapon);
        PootText->CreateAnimation({ .AnimationName = "Poot", .ImageName = "Poot.bmp", .Start = 0, .End = 17, .InterTime = 0.05f , .Loop = false });
        PootText->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Poot.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
        PootText->SetScale({ 70, 70 });

        PootText->ChangeAnimation("Idle");
        PootText->Off();

        PootTextAnimationList.push_back(PootText);
    }
}

void WeaponAirStrike::ExplosionAnimation(int _Index)
{
    ExplosionCircleList[_Index]->SetPosition(MissileList[_Index]->GetPosition());
    ExplosionCircleList[_Index]->On();
    ExplosionCircleList[_Index]->ChangeAnimation("Explosion", 0);

    ExplosionElipseList[_Index]->SetPosition(MissileList[_Index]->GetPosition());
    ExplosionElipseList[_Index]->On();
    ExplosionElipseList[_Index]->ChangeAnimation("ExplosionElipse", 0);

    PootTextAnimationList[_Index]->SetPosition(MissileList[_Index]->GetPosition());
    PootTextAnimationList[_Index]->On();
    PootTextAnimationList[_Index]->ChangeAnimation("Poot", 0);
}

void WeaponAirStrike::ExplosionAnimationOff()
{
    if (isHideAirPlane == false)
    {
        return;
    }

    CircleOff();
    ElipseOff();
    PootOff();

}

void WeaponAirStrike::CircleOff()
{
    if (CircleOffCount == MissileNum)
    {
        return;
    }

    for (int i = 0; i < MissileNum; i++)
    {
        if (ExplosionCircleList[i]->IsUpdate() == true && ExplosionCircleList[i]->IsAnimationEnd() == true)
        {
            ExplosionCircleList[i]->Off();
            CircleOffCount++;
        }
    }
}
void WeaponAirStrike::ElipseOff()
{
    if (ElipseOffCount == MissileNum)
    {
        return;
    }

    for (int i = 0; i < MissileNum; i++)
    {
        if (ExplosionElipseList[i]->IsUpdate() == true && ExplosionElipseList[i]->IsAnimationEnd() == true)
        {
            ExplosionElipseList[i]->Off();
            ElipseOffCount++;
        }
    }
}
void WeaponAirStrike::PootOff()
{
    if (PootOffCount == MissileNum)
    {
        return;
    }

    for (int i = 0; i < MissileNum; i++)
    {
        if (PootTextAnimationList[i]->IsUpdate() == true && PootTextAnimationList[i]->IsAnimationEnd() == true)
        {
            PootTextAnimationList[i]->Off();
            PootOffCount++;
        }
    }
}

void WeaponAirStrike::MarkerInit()
{
    Marker = CreateRender("Marker.bmp", WormsRenderOrder::Weapon);
    Marker->SetScale({ 60, 60 });

    Marker->CreateAnimation({ .AnimationName = "MarkerOn",.ImageName = "Marker.bmp",.Start = 0,.End = 9,.InterTime = 0.05f ,. Loop = true });
    Marker->Off();
}