#pragma once
#include "Weapon.h"


class MapModifier;
class WeaponShotgun : public Weapon
{
public:
	// constrcuter destructer
	WeaponShotgun() ;
	~WeaponShotgun();

	// delete Function
	WeaponShotgun(const WeaponShotgun& _Other) = delete;
	WeaponShotgun(WeaponShotgun&& _Other) noexcept = delete;
	WeaponShotgun& operator=(const WeaponShotgun& _Other) = delete;
	WeaponShotgun& operator=(WeaponShotgun&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;
	void ResetWeapon(float _DeltaTime) override;

private:

	int BulletCount = 20;

	std::vector <GameEngineCollision*> ShotGunCollision;	//�ݸ���
	std::vector <float4> ShotGunDir;

	std::vector<bool> isShooted;

	void WeaponShotgunInit();
	void WeaponMove(GameEngineCollision* _Col, float _DeltaTime, float4 _Dir);

	void MakeBomb(float4 _Pos);

	MapModifier* Bomb = nullptr;

};

