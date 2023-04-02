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

	//void ResetWeapon();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;


private:

    float AimIndex = 15;
    int NextAimIndex = 15;
    bool isExplosion = false;

	int BulletCount = 2;

    //bool isIsFireAnimationDone = false;

    float LoadDelay = 0.0f;
    float WeaponDoneTime = 0.0f;

	GameEngineRender* AimingLine = nullptr;

	std::vector <GameEngineCollision*> ShotGunCollision;							// index�� �ش��ϴ� �Ѿ� �ݸ���
	std::vector <float4> ShotGunDir;												// index�� �ش��ϴ� �Ѿ� �߻� ����
	std::vector<bool> isShooted;													// index�� �ش��ϴ� �Ѿ��� �߻�Ǿ�����



	bool IsDone();
	void WeaponShotgunInit();														// �Ѿ� �߰�
	//void WeaponMove(GameEngineCollision* _Col, float _DeltaTime, float4 _Dir);		// �Ѿ� �̵�
	void Firing(float _DeltaTime);	
	void CheckFiring();
    void SetAimFrameIndex();

    void Aiming(float _DeltaTime);


};

