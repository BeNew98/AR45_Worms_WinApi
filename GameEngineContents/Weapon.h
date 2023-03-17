#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineCollision.h>

class GameEngineImage;
class Weapon :public GameEngineActor
{
public:
	// constrcuter destructer
	Weapon() ;
	~Weapon();

	// delete Function
	Weapon(const Weapon& _Other) = delete;
	Weapon(Weapon&& _Other) noexcept = delete;
	Weapon& operator=(const Weapon& _Other) = delete;
	Weapon& operator=(Weapon&& _Other) noexcept = delete;

	float4 GetShootDir();
	bool PressShoot();


protected:

	//
	bool EffectGravity = true;						// �߷¿���
	bool isAnimation = false;						// �ִϸ��̼�
	bool isBlocked = false;							// ���� �������
	bool isTarget = false;							// Ÿ�� ���� ����

	float MoveSpeed = 0.0f;							// ����ӷ�
	float Gravity = 0.0f;							// �߷�
	float GravityAccel = 0.0f;						// �߷°��ӵ�
	float Timer = 0.0f;								// Ÿ�̸�
	float WindPower = 0.0f;							// �ٶ�����
	float Dmg = 0.0f;								// ���ߵ�����(�Ÿ�������� üũ�ʿ�)
	float Charge = 0.0f;							// ����������

	float4 Dir = float4::Zero;						// ���� ���� ����
	float4 PlayerPos = float4::Zero;				// �÷��̾� ��ġ
	float4 BombScale = float4::Zero;				// ���� ����
	//float4 Scale = float4::Zero;					// ����������

	std::string WeaponName;							// ���� �̸�

	GameEngineImage* MapCollision = nullptr;		//�浹��

	GameEngineRender* WeaponRender = nullptr;		//����
	GameEngineCollision* WeaponCollision = nullptr;	//�ݸ���

private:
	bool isRightDir = true;
	float Height = 0.0f;
		
};

//Ÿ��, ����, ����, �ݸ���, movespeed, �߷°�, �߷� ����, ũ��, 
//�̸�, �ִϸ��̼�, Ÿ�̸�, count, ���� �������, �ٶ���ġ, 
//���߽�����, ������, �÷��̾���ġ, ������

