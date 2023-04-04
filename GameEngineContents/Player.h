#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/NumberRenderObject.h>

#include "ContentsEnums.h"

enum class PlayerState
{
	IDLE,
	MOVE,
	JUMP,
    EQUIPWEAPON,

    Dead,
    Win,

    FlyDown,
    FacePlant,

    Damaged,

    FlyAway,
    Sliding,
    StandUp,

};

enum class PlayerAngleDir
{
    Left,
    Right,
};

enum class WallCheckDir
{
    Up,
    Down,
    Left,
    Right,
    LeftUp, //���� ���
    LeftDown, //���� �ϴ�
    RightUp, //���� ���
    RightDown, //���� �ϴ�
    LeftUpBoth,
    LeftDownBoth,
    RightUpBoth,
    RightDownBoth
};

// ���� : �÷��̾� Ŭ����
class Weapon;
class GameEngineImage;
class PlayerHPUI;
class Player : public GameEngineActor
{
public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

	void SetColImage(const std::string_view& _Name);

	void SetIsMyTurn(bool _Value);

    void SetHP(int _Hp)
    {
        PlayerHP = _Hp;
        MaxHP = _Hp;
    }

	bool GetIsMyTurn()
	{
		return IsMyTurn;
	}

    void SetCurWeapon(Weapon* _Weapon);

	Weapon* GetCurWeapon()
	{
		return CurWeapon;
	}

    int GetPlayerHP()
    {
        return PlayerHP;
    }

    void SetPlayerState(PlayerState _State);

    void DisplayDamageUI(float _Damage); //������ UI�� ���� �Լ�
	void CreatePlayerAnimation(const std::string_view& _AnimationName, const std::string_view& _ImageName, int _StartFrame, int _EndFrame, float _InterTime, bool Loop = true); //�ܺ�(����)���� �÷��̾��� �ִϸ��̼��� ���� �� �ְԲ� 
	void ChangePlayerAnimation(const std::string_view& _AnimationName, int _CurIndex = 0); //�÷��̾� �ִϸ��̼� ����
	void SetPlayerAnimationFrame(int _Frame); // ���� ������ �� �������� ����
	void Damaged(int _Damage, float4 _Dir = float4::Zero, float _Power = 0); //�÷��̾ �������� �޴� �Լ�

	//�÷��̾� ���¸� �޾ƿ�
	PlayerState GetPlayerState()
	{
		return StateValue;
	}

	bool IsPlayerAnimationEnd();

	void SetCanIMove(bool _Value)
	{
		CanIMove = _Value;
	}

    bool GetIsAlive()
    {
        return IsAlive;
    }

	// ���⿡�� ���� �÷��̾��� ������ ������ ���� �޼��� 
	float4 GetPlayerDir() 
	{
		if ("Right_" == DirString)
		{
			return float4::Right;
		}
		else
		{
			return float4::Left;
		}
	}

	//�÷��̾� UI�� �̹������� Ư�������ִ� �޼���
	void SetHPUI(const std::string_view& _HPNumberImage, const std::string_view& _NametagImage, const std::string_view& _ArrowImage);

    std::vector<int> GetPlayerWeaponCount()
    {
        return WeaponCount;
    }

    void UsingHealkit(int _Value);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	void Test(); //�׽�Ʈ�� �Լ�
    
    void CreateAnimation();
	GameEngineRender* AnimationRender = nullptr;  //�ִϸ��̼� ������
	GameEngineCollision* BodyCollision = nullptr; //���� �޾Ҵ��� ���θ� Ȯ���ϱ� ���� �ݸ���

    void PlaySoundOnce(const std::string_view& _Text);
    void RandomDamagedSound();

    //�÷��̾� �������ͽ� ���� (HP,������ �� �ִ���,��������)
    bool CanIMove = true;
    int PlayerHP = 100;
    int MaxHP = 100;
    bool IsAlive = true;
    void CheckAlive();
    void TestChangeDeadState();

    void CheckTurn();
    bool IsMyTurn = false; //�� ������ üũ

    Weapon* CurWeapon = nullptr; //���� ����
    WeaponNum CurWeaponNum = WeaponNum::None;

    void PlayerDead(); //�÷��̾� ���� ���� �����ϴ� �Լ�
    void SetGraveObject(const std::string_view& _GraveImage);
    std::string_view PlayerGraveImageStringView;

	//�÷��̾� UI���� (HP, ȭ��ǥ�̹���)
	PlayerHPUI* HPUI = nullptr; //�÷��̾� HP�� ������ �ѹ� ������ 
	float DamagedTime = 0.0f;
	std::string_view PlayerHPNumberImageStringView; //�÷��̾��� HPNumberImage �̸�
	
	//�÷��̾��� ���⿡ ���� �ٸ� �ִϸ��̼��� �ϰԲ� 
	void DirCheck(const std::string_view& _AnimationName, int _CurIndex = 0);

	//�̵� ����
	std::string DirString = "Right_"; //����
	float4 MoveDir = float4::Zero; //�÷��̾��� �̵� ����
	bool IsGround = true; //�÷��̾��� ���� ����
	void IsGroundCheck();
	std::string AnimationDir = "";
    void MoveCalculation(float _DeltaTime);

    bool ReturnCanIMove(PlayerAngleDir _Dir);
	void SetMoveAngle(); 	//�̵��� �÷��̾��� �¿��� �ȼ��� üũ�Ͽ�, �ޱ��� ����
	float LeftMoveAngle = 0.0f; 
	float RightMoveAngle = 0.0f; 
    const float AngleLimit = 7.0f; //���� ���ѿ� ���

    const float SetMoveDirRadius = 5.0f;
    const float SetMoveDIrCenterPos = -4.0f;

    bool GetPlayerWallCheck(WallCheckDir _Dir, float _DeltaTime); //���� �� ������ ���� �´�� �ִ����� ������ 
    void SetMoveDirWithAngle(WallCheckDir _Dir); // ���߿��� ���� ����� ��, MoveDir�� �����ϴ� �Լ�

    void PlayerPixelCheck(float _Deltatime);
    bool LeftPixelCheck = false;
    bool RightPixelCheck = false;
    bool UpPixelCheck = false;
    bool DownPixelCheck = false;
    bool LeftUpPixelCheck = false;
    bool RightUpPixelCheck = false;
    bool LeftDownPixelCheck = false;
    bool RightDownPixelCheck = false;

	float MoveSpeed = 50.0f;
	float Gravity = 500.0f;
    float JumpForce = 200.0f;

	void GravityApplied(float _DeltaTime); //�߷�����

	float4 PullUpCharacter(float4 _NextPos, float _DeltaTime); //�÷��̾ colimage�� �Ʒ��� �� �ִٸ�,MoveDir�� ���� ����ø�

	GameEngineImage* ColImage = nullptr; //�̵� � ���� colimage

    void SetWeaponCount();
    std::vector<int> WeaponCount;

	//�÷��̾� ������Ʈ ����	
	void ChangeState(PlayerState _State);
	void UpdateState(float _Time);

    float StateCalTime = 0.0f; //������Ʈ ������ �ð� ����� �ʿ��� ��
    float StateCalTime2 = 0.0f;
    float StateCalTime3 = 0.0f;
    bool StateCalBool = true;
    bool StateCalBool2 = true;
    bool StateCalBool3 = true;
    float StateCalValue = 0.0f;

	//�÷��̾��� ���� ����
	PlayerState StateValue = PlayerState::IDLE;
    PlayerState PrevStateValue = PlayerState::IDLE;

	void IdleStart();
	void IdleUpdate(float _DeltaTime);
	void IdleEnd();

	void MoveStart();
	void MoveUpdate(float _DeltaTime);
	void MoveEnd();

	void JumpStart();
	void JumpUpdate(float _DeltaTime);
	void JumpEnd();

    void EquipWeaponStart();
    void EquipWeaponUpdate(float _DeltaTime);
    void EquipWeaponEnd();

    void DeadStart();
    void DeadUpdate(float _DeltaTime);
    void DeadEnd();

    void WinStart();
    void WinUpdate(float _DeltaTime);
    void WinEnd();

    void FlyDownStart();
    void FlyDownUpdate(float _DeltaTime);
    void FlyDownEnd();

    void FacePlantStart();
    void FacePlantUpdate(float _DeltaTime);
    void FacePlantEnd();

    void FlyAwayStart();
    void FlyAwayUpdate(float _DeltaTime);
    void FlyAwayEnd();

    void SlidingStart();
    void SlidingUpdate(float _DeltaTime);
    void SlidingEnd();

    void StandUpStart();
    void StandUpUpdate(float _DeltaTime);
    void StandUpEnd();

    void DamagedStart();
    void DamagedUpdate(float _DeltaTime);
    void DamagedEnd();

	//�⺻ ������Ʈ ���� �Լ�
	//void Start();
	//void Update(float _DeltaTime);
	//void End();
};

