#pragma once
#include "Weapon.h"

// ���� : F3-2 Handgun ���� : �����ؼ� 6���� �߻��մϴ�
class MapModifier;
class WeaponHandgun : public Weapon
{
public:
	// constrcuter destructer
	WeaponHandgun();
	~WeaponHandgun();

	// delete Function
	WeaponHandgun(const WeaponHandgun& _Other) = delete;
	WeaponHandgun(WeaponHandgun&& _Other) noexcept = delete;
	WeaponHandgun& operator=(const WeaponHandgun& _Other) = delete;
	WeaponHandgun& operator=(WeaponHandgun&& _Other) noexcept = delete;

	void ResetWeapon();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

	int BulletCount = 6;
	float DelayTime = 0.5f;

	GameEngineRender* AimingLine = nullptr;
	std::vector <GameEngineCollision*> HandgunCollision;							// index�� �ش��ϴ� �Ѿ� �ݸ���
	std::vector<bool> isShooted;			//���� �ʿ�								// index�� �ش��ϴ� �Ѿ��� �߻�Ǿ�����

	bool IsDone();
	void WeaponHandgunInit();														// �Ѿ� �߰�
	void Firing(float _DeltaTime);
	void CheckFiring();

    float AimIndex = 15;
    int NextAimIndex = 15;
    void SetAimFrameIndex();

};

