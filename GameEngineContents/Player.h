#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/NumberRenderObject.h>

enum class PlayerState
{
	IDLE,
	MOVE,
	JUMP,
    EQUIPWEAPON,
    Dead,
    Win
};

enum class PlayerAngleDir
{
    Left,
    Right,
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

	bool GetIsMyTurn()
	{
		return IsMyTurn;
	}

	void SetCurWeapon(Weapon* _Weapon)
	{
		CurWeapon = _Weapon;
	}

	Weapon* GetCurWeapon()
	{
		return CurWeapon;
	}

    int GetPlayerHP()
    {
        return PlayerHP;
    }

	void CreatePlayerAnimation(const std::string_view& _AnimationName, const std::string_view& _ImageName, int _StartFrame, int _EndFrame, float _InterTime, bool Loop = true); //�ܺ�(����)���� �÷��̾��� �ִϸ��̼��� ���� �� �ְԲ� 
	void ChangePlayerAnimation(const std::string_view& _AnimationName, int _CurIndex = 0); //�÷��̾� �ִϸ��̼� ����
	void SetPlayerAnimationFrame(int _Frame); // ���� ������ �� �������� ����
	void Damaged(int _Damage); //�÷��̾ �������� �޴� �Լ�

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

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	void Test(); //�׽�Ʈ�� �Լ�
	
    void CreateAnimation();
	GameEngineRender* AnimationRender = nullptr;  //�ִϸ��̼� ������
	GameEngineCollision* BodyCollision = nullptr; //���� �޾Ҵ��� ���θ� Ȯ���ϱ� ���� �ݸ���

    //�÷��̾� �������ͽ� ���� (HP,������ �� �ִ���,��������)
    bool CanIMove = true;
    int PlayerHP = 100;
    bool IsAlive = true;
    void CheckAlive();
    void TestChangeDeadState();

    void CheckTurn();
    bool IsMyTurn = false; //�� ������ üũ

    Weapon* CurWeapon = nullptr; //���� ����

    void PlayerDead(); //�÷��̾� ���� ���� �����ϴ� �Լ�
    void SetGraveObject(GameEngineImage* _ColImage, const std::string_view& _GraveImage);
    std::string_view PlayerGraveImageStringView;

	//�÷��̾� UI���� (HP, ȭ��ǥ�̹���)
	PlayerHPUI* HPUI; //�÷��̾� HP�� ������ �ѹ� ������ 
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
    bool NextPosWallCheck(float4 _NextPos);

    bool ReturnCanIMove(PlayerAngleDir _Dir);
	void SetMoveAngle(); 	//�̵��� �÷��̾��� �¿��� �ȼ��� üũ�Ͽ�, �ޱ��� ����
	float LeftMoveAngle = 0.0f; 
	float RightMoveAngle = 0.0f; 
    const float AngleLimit = 5.0f; //���� ���ѿ� ���

    void PlayerPixelCheck();
    bool LeftPixelCheck = false;
    bool RightPixelCheck = false;
    bool UpPixelCheck = false;
    bool DownPixelCheck = false;
    bool LeftUpPixelCheck = false;
    bool RightUpPixelCheck = false;

	float MoveSpeed = 50.0f;
	float Gravity = 500.0f;

	void GravityApplied(float _DeltaTime); //�߷�����

	float4 PullUpCharacter(float4 _NextPos, float _DeltaTime); //�÷��̾ colimage�� �Ʒ��� �� �ִٸ�,MoveDir�� ���� ����ø�

	GameEngineImage* ColImage = nullptr; //�̵� � ���� colimage

	//�÷��̾� ������Ʈ ����	
	void ChangeState(PlayerState _State);
	void UpdateState(float _Time);

    float StateCalTime = 0.0f; //������Ʈ ������ �ð� ����� �ʿ��� ��
    bool StateCalBool = true;
    bool StateCalBool2 = true;

	//�÷��̾��� ���� ����
	PlayerState StateValue = PlayerState::IDLE;

	void IdleStart();
	void IdleUpdate(float _DeltatTime);
	void IdleEnd();

	void MoveStart();
	void MoveUpdate(float _DeltatTime);
	void MoveEnd();

	void JumpStart();
	void JumpUpdate(float _DeltatTime);
	void JumpEnd();

    void EquipWeaponStart();
    void EquipWeaponUpdate(float _DeltatTime);
    void EquipWeaponEnd();

    void DeadStart();
    void DeadUpdate(float _DeltatTime);
    void DeadEnd();

    void WinStart();
    void WinUpdate(float _DeltatTime);
    void WinEnd();

	//�⺻ ������Ʈ ���� �Լ�
	//void Start();
	//void Update(float _DeltatTime);
	//void End();
};

