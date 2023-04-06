#pragma once
#include "Weapon.h"

// ���� : F3-1 Shotgun ����
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


private:

	int BulletCount = 2;
    float AimIndex = 17;
    int NextAimIndex = 16;
    bool isExplosion = false;


    float LoadDelay = 0.0f;
    float WaitingTime = 0.0f;

	GameEngineRender* AimingLine = nullptr;

	std::vector <GameEngineCollision*> ShotGunCollision;							// index�� �ش��ϴ� �Ѿ� �ݸ���
	std::vector <float4> ShotGunDir;												// index�� �ش��ϴ� �Ѿ� �߻� ����
	std::vector<bool> isShooted;													// index�� �ش��ϴ� �Ѿ��� �߻�Ǿ�����



	bool IsDone();
	void WeaponShotgunInit();														// �Ѿ� �߰�
	void Firing(float _DeltaTime);	
	void CheckFiring();
    void SetAimFrameIndex();

    void Aiming(float _DeltaTime);


};

