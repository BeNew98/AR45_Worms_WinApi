#pragma once
#include "MapObject.h"

// ���� :
class Drum : public MapObject
{
public:
	// constrcuter destructer
	Drum();
	~Drum();

	// delete Function
	Drum(const Drum& _Other) = delete;
	Drum(Drum&& _Other) noexcept = delete;
	Drum& operator=(const Drum& _Other) = delete;
	Drum& operator=(Drum&& _Other) noexcept = delete;

protected:
    void Start() override;
    void Update(float _DeltaTime) override;

private:
    bool IsExplosion = false;
    bool IsPetrolEffectEnd = false;

    int Count = 2;
    int Gauge = 150;
    int BombScale = 100;

    //��ġ���� �ʿ�
    float WaitTime = 0.1f;
    float MaxDmg = 50;
    float MinDmg = 20;
    float MinKnockBackPower = 35.0f;
    float MaxKnockBackPower = 75.0f;


    GameEngineRender* ExplosionCircle = nullptr;
    GameEngineRender* ExplosionElipse = nullptr;
    GameEngineRender* PootTextAnimation = nullptr;

    
    void HitWeaponCheck();                          //Weapon�� ���� ����ǿ� ������ �巳�뿡 ���� ����
    void HitPlayerCheck();                          //�÷��̾�� �浹�� Modifier�浹ü�� �޾Ƽ� �ѱ�
    void AttackPlayer(GameEngineCollision* _Col);   //�÷��̾�� ������ ����

    void Explosion();                               //����

    bool CheckCollision(GameEngineCollision* _Col);


    void AnimCheck();                               //�ִϸ��̼� üũ �� ����
    void ExplosionAnimationOff();
    void ExplosionEffectInit();
};

