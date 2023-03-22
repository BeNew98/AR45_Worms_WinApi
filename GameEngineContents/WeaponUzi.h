#pragma once
#include "Weapon.h"

// ���� : F3-3 Uzi ���� : 6�����Ӹ��� 1�߾� ������ �����ؼ� 10���� �Ѿ��� �߻��մϴ�
class MapModifier;
class WeaponUzi : public Weapon
{
public:
	// constrcuter destructer
	WeaponUzi();
	~WeaponUzi();

	// delete Function
	WeaponUzi(const WeaponUzi& _Other) = delete;
	WeaponUzi(WeaponUzi&& _Other) noexcept = delete;
	WeaponUzi& operator=(const WeaponUzi& _Other) = delete;
	WeaponUzi& operator=(WeaponUzi&& _Other) noexcept = delete;

	void ResetWeapon();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

	int BulletCount = 10;
	float DelayTime = 0.1f;

	GameEngineRender* AimingLine = nullptr;
	std::vector <GameEngineCollision*> UziCollision;							// index�� �ش��ϴ� �Ѿ� �ݸ���
	std::vector<bool> isShooted;			//���� �ʿ�								// index�� �ش��ϴ� �Ѿ��� �߻�Ǿ�����



	bool IsDone();
	void WeaponUziInit();														// �Ѿ� �߰�
	void Firing(float _DeltaTime);
	void CheckFiring();

};

