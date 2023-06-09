#pragma once
#include <vector>
#include "Weapon.h"

class MapModifier;
class WeaponSheep : public Weapon
{

public:

	WeaponSheep();
	~WeaponSheep();

	WeaponSheep(const WeaponSheep& _Other) = delete;
	WeaponSheep(WeaponSheep&& _Other) noexcept = delete;
	WeaponSheep& operator=(const WeaponSheep& _Other) = delete;
	WeaponSheep& operator=(WeaponSheep&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	void ResetWeapon();
private:

    void WeaponSheepInit();
    void SheepFalling(float _DeltaTime);
    void SheepWalking(float _DeltaTime);
    void CheckMoveAngle();
    bool CanIMove();
    void Explosion();
    void SheepJump(float _DeltaTime);
    void Timer();
    void CameraUpdate(float _DeltaTime);

    void SheepMove(float _DeltaTime);

    void ExplosionAnimationOff();
    void DamageToPlayer();

    void DebrisAnimation(float _DeltaTime);
    void DebrisInit();

    std::vector<GameEngineRender*> Sparks;
    std::vector<GameEngineRender*> Smokes;

    std::vector<float4> SparksDir;
    std::vector<float4> SmokesDir;

    bool isDebrisSet = false;
    float DebrisGravity = false;

	MapModifier* SheepExplosion = nullptr;

	bool isSheepOn = false;
	bool isOnMap = false;
	bool isDirRight = false;
	bool isShoot = false;
	bool isJump = true;
	bool isJumpDirSet = false;
	bool isSet = false;
    bool isExplosion = false;

    float CurTime = 0.0f;
    float PrevTime = 0.0f;

    float TimeCount = 0.0f;

    float CameraTimeCount = 0.0f;

	float4 JumpDir = float4::Zero;
	float4 MoveDir = float4::Zero;

    float LeftMoveAngle = 0;
    float RightMoveAngle = 0;

    float JumpGravity = 0.0f;
    float DebrisMoveSpeed = 0.0f;

    std::vector<GameEngineRender*> DebrisList;
    std::vector<float4> DebrisDirList;
    std::vector<int> DebrisCountList;

	GameEngineRender* WeaponRender = nullptr;		//����
	GameEngineCollision* WeaponCollision = nullptr;	//�ݸ���
};

