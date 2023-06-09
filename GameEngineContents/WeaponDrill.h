#pragma once
#include "Weapon.h"

#include <GameEngineCore/GameEngineResources.h>

class MapModifier;
class WeaponDrill : public Weapon
{

public:

	WeaponDrill();
	~WeaponDrill();

	WeaponDrill(const WeaponDrill& _Other) = delete;
	WeaponDrill(WeaponDrill&& _Other) noexcept = delete;
	WeaponDrill& operator=(const WeaponDrill& _Other) = delete;
	WeaponDrill& operator=(WeaponDrill&& _Other) noexcept = delete;

protected:

    void Start() override;
    void Update(float _DeltaTime) override;
    void Render(float _DeltaTime) override;

private:

    void DrillInit();
    void Timer();
    void Drilling(float _DeltaTime);

    bool isAttack = false;
    bool isDrillEnd = false;

    float TurnChangeCount = 0.0f;

    float TimeCount = 0.0f;
    float CurTime = 0.0f;
    float PrevTime = 0.0f;

    float DrillOnTime = 0.0f;

    float DrillCycleTime = 0.0f;

    float4 StartPos = { 0,0 };

    GameEngineSoundPlayer DrillSound;
    GameEngineCollision* HoleCollision = nullptr;
};

