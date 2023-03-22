#pragma once
#include <vector>
#include "Weapon.h"

class MapModifier;
class WeaponAirStrike : public Weapon
{

public:

	WeaponAirStrike();
	~WeaponAirStrike();

	WeaponAirStrike(const WeaponAirStrike& _Other) = delete;
	WeaponAirStrike(WeaponAirStrike&& _Other) noexcept = delete;
	WeaponAirStrike& operator=(const WeaponAirStrike& _Other) = delete;
	WeaponAirStrike& operator=(WeaponAirStrike&& _Other) noexcept = delete;

protected:

	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:

	void WeaponAirStrikeInit();
	void SetAirPlanePos();
	void AirPlaneMove(float _DeltaTime);
	void Attack(float _DeltaTime);
	void Firing(float _DeltaTime);
	void SetMissiles();
	void Explosion();
	void CameraUpdate();

	GameEngineRender* Airplane = nullptr;

	float4 AirPlaneStartPos = { 0,0 };
	float4 TargetPos = { 0,0 };

	float4 PrevPos = { 0,0 };
	float4 CurPos = { 0,0 };

	bool isMoveRight = 0;
	bool isHideAirPlane = false;

	float4 MiddleMissileStartPos = { 0,0 };

	std::vector<GameEngineRender*> MissileList;
	std::vector<GameEngineCollision*> MissileCollisionList;
};

