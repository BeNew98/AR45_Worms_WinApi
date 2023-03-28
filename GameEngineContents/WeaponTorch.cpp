#include "WeaponTorch.h"
#include "MapModifier.h"
#include "ContentsEnums.h"
#include "Player.h"

#include <ctime>

#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>

WeaponTorch::WeaponTorch()
{
}

WeaponTorch::~WeaponTorch()
{
}

void WeaponTorch::Start()
{
    TorchInit();
    PrevTime = clock();
}
void WeaponTorch::Update(float _DeltaTime)
{

    Timer();

    if (GameEngineInput::IsDown("Shoot") == true)
    {
        isAttack = true;
        StartPos = CurPlayer->GetPos();
        CurPlayer->ChangePlayerAnimation("Torchfire");
    }

    if(isAttack == true && isFireEnd == false)
    {
        TorchOn();
    }
}

void WeaponTorch::Render(float _DeltaTime)
{

}

void WeaponTorch::TorchInit()
{
    SetCurPlayer();

    MapCollision = GameEngineResources::GetInst().ImageFind("MapCity_Ground.bmp");

    ScreenSize = GameEngineWindow::GetScreenSize();

    WeaponNumber = static_cast<int>(WeaponNum::Torch);

    BombScale = 12;

    WeaponName = "Torch";
}

void WeaponTorch::TorchOn()
{
    if (TorchTime >= 5.0f)
    {
        isFireEnd = true;
        isWeaponDone = true;
        return;
    }

    TorchTime += TimeCount;

    float4 HolePos = { CurPlayer->GetPos().x, StartPos.y - BombScale};
    MapModifier::MainModifier->CreateHole(HolePos, BombScale);
  
}

void WeaponTorch::Timer()
{
    CurTime = clock();

    TimeCount = (CurTime - PrevTime) / 1000.0f;

    PrevTime = CurTime;
}