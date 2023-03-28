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

    GameEngineRender* AimingLine = nullptr;

    GameEngineRender* WeaponRender = nullptr;		        //����
    GameEngineCollision* WeaponCollision = nullptr;	        //�ݸ���

    std::vector<GameEngineRender*> ClusterRender;           // �ܿ� Ŭ������ ����
    std::vector<GameEngineCollision*> ClusterCollision;     // �ܿ� Ŭ������ �ݸ���
    std::vector<float4> ClusterDir;

    //����, �ִϸ��̼� 
    GameEngineRender* ExplosionCircle = nullptr;
    GameEngineRender* ExplosionElipse = nullptr;
    GameEngineRender* BiffTextAnimation = nullptr;
    GameEngineRender* PootTextAnimation = nullptr;

    GameEngineRender* ChargeAnimation = nullptr;

    float ClusterSpeed = 100.0f;

    void ClusterFiring(float _DeltaTime);
    void ClusterOn(float4 _Pos);
    void ClusterOff();

    float4 DropDir = float4::Zero;
    bool isDone();


    float AimIndex = 15;
    int NextAimIndex = 15;
    void SetAimFrameIndex();
};

