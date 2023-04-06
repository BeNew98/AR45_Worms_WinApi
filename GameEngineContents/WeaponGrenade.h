#pragma once
#include "Weapon.h"
#include <GameEngineCore/NumberRenderObject.h>


// ���� : F2-1 Grenade ����ź
class MapModifier;

class WeaponGrenade : public Weapon
{
public:
	// constrcuter destructer
	WeaponGrenade();
	~WeaponGrenade();

	// delete Function
	WeaponGrenade(const WeaponGrenade& _Other) = delete;
	WeaponGrenade(WeaponGrenade&& _Other) noexcept = delete;
	WeaponGrenade& operator=(const WeaponGrenade& _Other) = delete;
	WeaponGrenade& operator=(WeaponGrenade&& _Other) noexcept = delete;
	
	void ResetWeapon();

protected:

	void Start() override;
	void Update(float _DeltaTime) override;



private:


	bool isExplosion = false;
    bool isPress = false;
    float AimIndex = 12;
    int NextAimIndex = 15;
    float WaitTime = 0.0f;

    // ���� ���ؼ�
    GameEngineRender* AimingLine = nullptr;
    // ���� �ִϸ��̼�
    GameEngineRender* ChargeAnimation = nullptr;

    // ��ź �̹�������, �ݸ���
	GameEngineRender* WeaponRender = nullptr;		//����
	GameEngineCollision* WeaponCollision = nullptr;	//�ݸ���

    //Ÿ�̸� ����
    GameEngineRender* TimerRenderBack = nullptr;
    NumberRenderObject TimerRender;
    
    // ���� �� ��ź �̹��� �����Լ�
	void WeaponGrenadeInit();

    // ���� ������ �´� Frame Indexã�� �Լ�
    void SetAimFrameIndex();

    // ���� ������ ����
	void SetCharge(); // charge
	
    // ���ؿ� �´� ���� ����
    void Aiming(float _DeltaTime);
    
    // �߻翡 �´� ���� ����
    void Firing(float _DeltaTime);

    // Ÿ�̸� ���� üũ
    void CheckTimer(float _DeltaTime);

    //�ٶ� �߰�
    //float4 CulWindMoveVec(float _DeltaTime);

    // ���� �ִϸ��̼� ���� ����
    void CheckExplosion();
};

