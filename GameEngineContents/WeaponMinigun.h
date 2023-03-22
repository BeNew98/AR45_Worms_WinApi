#pragma once
#include "Weapon.h"

// 설명 : F3-3 Minigun 미니건 : 미니건을 사용하면 3프레임마다 1발씩 20발의 총알을 연속적으로 발사합니다.
class MapModifier;
class WeaponMinigun : public Weapon
{
public:
	// constrcuter destructer
	WeaponMinigun();
	~WeaponMinigun();

	// delete Function
	WeaponMinigun(const WeaponMinigun& _Other) = delete;
	WeaponMinigun(WeaponMinigun&& _Other) noexcept = delete;
	WeaponMinigun& operator=(const WeaponMinigun& _Other) = delete;
	WeaponMinigun& operator=(WeaponMinigun&& _Other) noexcept = delete;

	void ResetWeapon();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:

	int BulletCount = 20;

	std::vector <GameEngineCollision*> MinigunCollision;							// index에 해당하는 총알 콜리전
	std::vector <float4> MinigunDir;		//수정필요								// index에 해당하는 총알 발사 방향
	GameEngineRender* AimingLine = nullptr;

	std::vector<bool> isShooted;			//수정 필요								// index에 해당하는 총알이 발사되었는지

	void WeaponMinigunInit();														// 총알 추가
	void WeaponMove(GameEngineCollision* _Col, float _DeltaTime, float4 _Dir);		// 총알 이동
	void Firing(float _DeltaTime);
	void CheckFiring();

};

