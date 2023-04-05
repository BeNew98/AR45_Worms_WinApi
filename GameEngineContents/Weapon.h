#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineCollision.h>

class Player;
class GameEngineImage;
class Weapon :public GameEngineActor
{
public:


	static std::map<std::string, Weapon*> AllWeapons;
	// constrcuter destructer
	Weapon();
	~Weapon();

	// delete Function
	Weapon(const Weapon& _Other) = delete;
	Weapon(Weapon&& _Other) noexcept = delete;
	Weapon& operator=(const Weapon& _Other) = delete;
	Weapon& operator=(Weapon&& _Other) noexcept = delete;


	bool IsWeaponDone() // �������� �������� 
	{
		return isWeaponDone;
	}

	bool IsFiring() // ���� �߻������� bool��
	{
		return isFire;
	}

	int GetWeaponNumber()
	{
		return WeaponNumber;
	}

    std::string GetWeaponName()
    {
        return WeaponName;
    }

	//virtual void ResetWeapon() {};
protected:

	//
	bool isFire = false;							// �߻������� üũ
	bool isWeaponDone = false;

	// ī�޶� �̵��� ���õ� ��
	float4 PrevCamPos = float4::Zero;
	float4 CurPlayerPos = float4::Zero;
	float4 LerpCamPos = float4::Zero;
	float fLerpRatio = 0.f;
	float fLerpSpeed = 2.f;


    float MinKnockBackPower = 0.0f;
    float MaxKnockBackPower = 0.0f;

	float MoveSpeed = 0.0f;							// ����ӷ�
	float Gravity = 0.0f;							// �߷�

    float Dmg = 0.0f;   //���� ����
    float MinDmg = 0.0f;
	float MaxDmg = 0.0f;								// ���ߵ�����(�Ÿ�������� üũ�ʿ�)
	float Charge = 0.0f;							// ����������

	// GlobalValue���� ������ ��
	float fWindSpeed = 0.f;
	int fWindPhase = 0;

	float4 Dir = float4::Zero;						// ���� ���� ����
	//float4 PlayerPos = float4::Zero;				// �÷��̾� ��ġ ---> �����
	float BombScale = 0.0f;				// ���� ����

	int WeaponNumber = 0;							// ���� �̸��� �ش��ϴ� Number
	std::string WeaponName;							// ���� �̸�

	GameEngineImage* MapCollision = nullptr;		//�浹��


	Player* CurPlayer = nullptr;
	float Timer = 0.0f;								// Ÿ�̸�

	float4 ScreenSize = { 0,0 };
	//GameEngineRender* AimingLine = nullptr;
	virtual void Init() {};
	virtual bool CheckCollision(GameEngineCollision* _Col); // ����ȭ �������� ���� ���

	float4 CheckCollisionSide(GameEngineCollision* _Col);
	float4 GetShootDir();
    
    // Shoot  Key�� ���õ� Ŀ�ǵ�
    bool DownShoot();
	bool PressShoot();
    bool UpShoot();
	float GetChargeTime();

	bool isEndCharging();
	void SetCurPlayer();
	bool AttackPlayer(GameEngineCollision* _Col,bool _AttackSelf = true);
    void AttackPlayerGun(GameEngineCollision* _Col, float _Distance);

    void ExplosionEffectInit(float _CircleDiameter);

	//virtual void Charging();
	virtual void Firing(float _Deltatime) {}; // ���ư��°�, 


    GameEngineRender* ExplosionCircle = nullptr;
    GameEngineRender* ExplosionElipse = nullptr;
    GameEngineRender* PootTextAnimation = nullptr;

	
private:

	bool isRightDir = true;
	float Height = 0.0f;


};

//Ÿ��, ����, ����, �ݸ���, movespeed, �߷°�, �߷� ����, ũ��, 
//�̸�, �ִϸ��̼�, Ÿ�̸�, count, ���� �������, �ٶ���ġ, 
//���߽�����, ������, �÷��̾���ġ, ������

