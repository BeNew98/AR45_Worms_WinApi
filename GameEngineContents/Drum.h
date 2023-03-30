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

    int Gauge = 150;
    int Dmg = 25;
    int BombScale = 50;


    GameEngineRender* ExplosionCircle = nullptr;
    GameEngineRender* ExplosionElipse = nullptr;
    GameEngineRender* PootTextAnimation = nullptr;
    
    void HitWeaponCheck();                      //Weapon�� ���� ����ǿ� ������ �巳�뿡 ���� ����
    void HitPlayerCheck();                      //�÷��̾�� ������ ����
    void AnimCheck();                           //�ִϸ��̼� üũ �� ����
    void Explosion();                           //����

    bool CheckCollision(GameEngineCollision* _Col);

    void CreateSpark();

    void ExplosionAnimationOff();
    void ExplosionEffectInit();
};

