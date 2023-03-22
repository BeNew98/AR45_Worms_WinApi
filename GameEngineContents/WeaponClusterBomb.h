#pragma once
#include "Weapon.h"

// ���� : F2-2 ClusterBomb Ŭ������ ��ź : ���� ������ �����ϴ� Ŭ�����Ͱ� ����ȴ�
class WeaponClusterBomb : public Weapon
{
public:
	// constrcuter destructer
	WeaponClusterBomb() ;
	~WeaponClusterBomb();

	// delete Function
	WeaponClusterBomb(const WeaponClusterBomb& _Other) = delete;
	WeaponClusterBomb(WeaponClusterBomb&& _Other) noexcept = delete;
	WeaponClusterBomb& operator=(const WeaponClusterBomb& _Other) = delete;
	WeaponClusterBomb& operator=(WeaponClusterBomb&& _Other) noexcept = delete;

protected:

    void Start() override;
    void Update(float _DeltaTime) override;

private:

    void WeaponClusterBombInit();

    bool isExplosion = false;

    void SetCharge(); // charge
    void Firing(float _DeltaTime);
    void ResetWeapon();

    GameEngineRender* WeaponRender = nullptr;		//����
    GameEngineCollision* WeaponCollision = nullptr;	//�ݸ���

};

