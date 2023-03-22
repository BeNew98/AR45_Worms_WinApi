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
	void Render(float _DeltaTime) override;

private:

	int BulletCount = 10;
	bool isFire = false;

	float DelayTime = 0.1f;

	std::vector <GameEngineCollision*> UziCollision;							// index�� �ش��ϴ� �Ѿ� �ݸ���
	//std::vector <float4> UziDir;		//�����ʿ�								// index�� �ش��ϴ� �Ѿ� �߻� ����
	GameEngineRender* AimingLine = nullptr;

	std::vector<bool> isShooted;			//���� �ʿ�								// index�� �ش��ϴ� �Ѿ��� �߻�Ǿ�����

	void WeaponUziInit();														// �Ѿ� �߰�
	//void WeaponMove(GameEngineCollision* _Col, float _DeltaTime, float4 _Dir);		// �Ѿ� �̵�
	void Firing(float _DeltaTime);
	void CheckFiring();

	bool IsDone();
};

