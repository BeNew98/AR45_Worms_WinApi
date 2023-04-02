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
    // �߻���
    float AimIndex = 12;
    int NextAimIndex = 15;

    //�߻� ��
    bool isExplosion = false; // MainBomb ����üũ
    bool isPress = false;
    int MainBombScale = 104;

    //Ŭ������
    float ClusterSpeed = 100.0f; // Ŭ������ ��ź Speed
    bool isClusterFire = false;
    bool isClusterExplosion = false;
    float WaitTime = 0.0f;

    // ���� ���ؼ�
    GameEngineRender* AimingLine = nullptr;
    // ���� �ִϸ��̼�
    GameEngineRender* ChargeAnimation = nullptr;

    // ���� ��ź �̹�������, �ݸ���
    GameEngineRender* WeaponRender = nullptr;		        //����
    GameEngineCollision* WeaponCollision = nullptr;	        //�ݸ���

    //Ŭ������
    std::vector<GameEngineRender*> ClusterRender;           // �ܿ� Ŭ������ ����
    std::vector<GameEngineCollision*> ClusterCollision;     // �ܿ� Ŭ������ �ݸ���
    std::vector<float4> ClusterDir;
    std::vector<float4> ClusterSmokeInterval;


    // ���� �ִϸ��̼� ���� 
    GameEngineRender* ExplosionCircle = nullptr;
    GameEngineRender* ExplosionElipse = nullptr;
    GameEngineRender* BiffTextAnimation = nullptr;
    GameEngineRender* PowTextAnimation = nullptr;

    void WeaponClusterBombInit();   // ���� �� ��ź �̹��� �����Լ�

    void SetAimFrameIndex();        // ���� ������ �´� Frame Indexã�� �Լ�

    void SetCharge();               // ���� ������ ����

    void Aiming(float _DeltaTime);  // ���ؿ� �´� ���� ����

    void Firing(float _DeltaTime);   // �߻翡 �´� ���� ����

    void CheckAnimationDone();          // ���� �ִϸ��̼� ���� ����


    void ClusterFiring(float _DeltaTime);   // Cluster�߻�

    void ClusterOn(float4 _Pos);            // ClusterRender��� On

    void ClusterOff();                      // ClusterRender��� Off

    bool isDone();                          // MainBomb�� ������, ��� Cluster �� ��������

    float4 CulWindMoveVec(float _DeltaTime); //�ٶ� �߰� 

    void MakeSmoke(); // Ŭ������
};

