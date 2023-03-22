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

	void ResetWeapon() override;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	//void Render(float _DeltaTime) override;

private:

	int BulletCount = 6;
	bool isFire = false;

	float DelayTime = 0.5f;

	std::vector <GameEngineCollision*> HandgunCollision;							// index�� �ش��ϴ� �Ѿ� �ݸ���
	//std::vector <float4> HandgunDir;		//�����ʿ�								// index�� �ش��ϴ� �Ѿ� �߻� ����

	std::vector<bool> isShooted;			//���� �ʿ�								// index�� �ش��ϴ� �Ѿ��� �߻�Ǿ�����

	void WeaponHandgunInit();														// �Ѿ� �߰�
	//void WeaponMove(GameEngineCollision* _Col, float _DeltaTime, float4 _Dir);		// �Ѿ� �̵�
	void Firing(float _DeltaTime);
	void CheckFiring();

	bool IsDone();

};

