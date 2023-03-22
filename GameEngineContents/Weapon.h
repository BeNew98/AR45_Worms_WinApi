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


	float MoveSpeed = 0.0f;							// ����ӷ�
	float Gravity = 0.0f;							// �߷�
	float Dmg = 0.0f;								// ���ߵ�����(�Ÿ�������� üũ�ʿ�)
	float Charge = 0.0f;							// ����������

	// GlobalValue���� ������ ��
	float fWindSpeed = 0.f;
	int fWindPhase = 0;

	float4 Dir = float4::Zero;						// ���� ���� ����
	//float4 PlayerPos = float4::Zero;				// �÷��̾� ��ġ ---> �����
	int BombScale = 0;				// ���� ����

	WeaponNum WeaponNumber;							// ���� �̸��� �ش��ϴ� Number
	std::string WeaponName;							// ���� �̸�

	GameEngineImage* MapCollision = nullptr;		//�浹��

	//GameEngineRender* WeaponRender = nullptr;		//����
	//GameEngineCollision* WeaponCollision = nullptr;	//�ݸ���

	Player* CurPlayer = nullptr;
	float Timer = 0.0f;								// Ÿ�̸�
	float PrevTime = 0.0f;
	float CurTime = 0.0f;
	float TimeCount = 0.0f;		
	float TimeCount_2 = 0.0f;

	//GameEngineRender* AimingLine = nullptr;
	virtual void Init() {};
	virtual bool CheckCollision(GameEngineCollision* _Col); // ����ȭ �������� ���� ���

	float GetChargeTime();
	float4 CheckCollisionSide(GameEngineCollision* _Col);
	float4 GetShootDir();
	bool PressShoot();
	bool isEndCharging();
	void TimeCounting();
	void SetCurPlayer();

	//virtual void Charging();
	virtual void Firing(float _Deltatime) {}; // ���ư��°�, 


private:

	bool isRightDir = true;
	float Height = 0.0f;


};

//Ÿ��, ����, ����, �ݸ���, movespeed, �߷°�, �߷� ����, ũ��, 
//�̸�, �ִϸ��̼�, Ÿ�̸�, count, ���� �������, �ٶ���ġ, 
//���߽�����, ������, �÷��̾���ġ, ������

