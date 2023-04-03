#pragma once
#include "Weapon.h"

// ���� : F4-1 Uzi ���� : ���� �������� ��ġ�� ������ ���� �߶���ϴ�
class WeaponFirePunch : public Weapon
{
public:
	// constrcuter destructer
	WeaponFirePunch() ;
	~WeaponFirePunch();

	// delete Function
	WeaponFirePunch(const WeaponFirePunch& _Other) = delete;
	WeaponFirePunch(WeaponFirePunch&& _Other) noexcept = delete;
	WeaponFirePunch& operator=(const WeaponFirePunch& _Other) = delete;
	WeaponFirePunch& operator=(WeaponFirePunch&& _Other) noexcept = delete;

protected:

    void Start() override;
    void Update(float _DeltaTime) override;

private:

    bool isAfterAnimation = false;
    bool isCharging = false;

    float4 StartPos = float4::Zero;

    float EffectTimer = 0.05f;
    GameEngineCollision* PunchCollision = nullptr;
};

