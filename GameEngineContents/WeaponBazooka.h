#pragma once
#include <vector>
#include "Weapon.h"

class MapModifier;
class WeaponBazooka : public Weapon
{

public:	

	WeaponBazooka();
	~WeaponBazooka();

	void WeaponBazookaInit();

	void ChangeBazReadyAnimation();


	void BazookaOn();				//changeAnimation
	void BazookaOff();
	void BazAiming();

	void ChargingRenderInit();	//���ؼ� ����
	void ChargingRenderOn();
	void ChargingRenderOff();

	void MakeSmoke();			//���� �� ���� �ڿ� ����ũ

	void Charging();			
	void firing(float _DeltaTime);
	void Explosion();
    void TimeCounting();
	void DamageToPlayer();

    void DebrisAnimation(float _DeltaTime);
    void DebrisInit();

	WeaponBazooka(const WeaponBazooka& _Other) = delete;
	WeaponBazooka(WeaponBazooka&& _Other) noexcept = delete;
	WeaponBazooka& operator=(const WeaponBazooka& _Other) = delete;
	WeaponBazooka& operator=(WeaponBazooka&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	void ResetWeapon();
private:
	GameEngineRender* shell = nullptr; //��ź (����ü)

    //����, �ִϸ��̼� 
	GameEngineRender* ExplosionCircle = nullptr;
	GameEngineRender* ExplosionElipse = nullptr;
	GameEngineRender* PootTextAnimation = nullptr;

    std::vector<GameEngineRender*> Sparks;
    std::vector<GameEngineRender*> Smokes;

    std::vector<float4> SparksDir;
    std::vector<float4> SmokesDir;

    bool isDebrisSet = false;
    float DebrisGravity = false;

    std::vector<GameEngineRender*> ChargingRender;

    //���
	bool isExplosion = false;
	bool isBazOn = false;

	bool isSet = false;

	bool isFire = false;
	bool isAiming = false;
	bool isAttack = false;

	bool isEndBazOn = false;
    bool isTimeSet = false;

    float TimeCount = 0.0f;
    float TimeCount_2 = 0.0f;

	int Bazindex = 0;
	int CurIndex = 16;

	int CountingIndex = 0;

	float4 ShootDir = { 0,0 };

	float4 StartDir = { 0,0 };

    float4 CurPos = { 0,0 };
    float4 NextPos = { 0,0 };

	Player* PrevPlayer = nullptr;



	float4 PrevCamPos = float4::Zero;
	float4 CurPlayerPos = float4::Zero;
	float4 LerpCamPos = float4::Zero;
	float fLerpRatio = 0.f;
	float fLerpSpeed = 2.f;

	GameEngineRender* WeaponRender = nullptr;		//����
	GameEngineCollision* WeaponCollision = nullptr;	//�ݸ���
};

