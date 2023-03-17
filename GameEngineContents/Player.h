#pragma once
#include <GameEngineCore/GameEngineActor.h>

enum class PlayerState
{
	IDLE,
	MOVE,
};

// ���� : �÷��̾� Ŭ����
class GameEngineImage;
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

	void SetIsMyTurn(bool _Value)
	{
		IsMyTurn = _Value;
	}

	bool GetIsMyTurn()
	{
		return IsMyTurn;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	GameEngineRender* AnimationRender = nullptr;  //�ִϸ��̼� ������
	GameEngineCollision* BodyCollision = nullptr; //���� �޾Ҵ��� ���θ� Ȯ���ϱ� ���� �ݸ���

	bool IsMyTurn = false; //�� ������ üũ

	//�÷��̾��� ���⿡ ���� �ٸ� �ִϸ��̼��� �ϰԲ� 
	void DirCheck(const std::string_view& _AnimationName);

	//�̵� ����
	std::string DirString = "Right_"; //����

	float4 MoveDir = float4::Zero; //�÷��̾��� �̵� ����

	float MoveSpeed = 50.0f;
	float Gravity = 300.0f;

	void GravityApplied();
	void MoveCalculation(float _DeltaTime);
	float4 PullUpCharacter(float4 _NextPos, float _DeltaTime); //�÷��̾ colimage�� �Ʒ��� �� �ִٸ�,MoveDir�� ���� ����ø�

	GameEngineImage* ColImage = nullptr; //�̵� � ���� colimage

	//�÷��̾� ������Ʈ ����	
	void ChangeState(PlayerState _State);
	void UpdateState(float _Time);

	//�÷��̾��� ���� ����
	PlayerState StateValue = PlayerState::IDLE;

	void IdleStart();
	void IdleUpdate(float _DeltatTime);
	void IdleEnd();

	void MoveStart();
	void MoveUpdate(float _DeltatTime);
	void MoveEnd();

	//�⺻ ������Ʈ ���� �Լ�
	//void Start();
	//void Update(float _DeltatTime);
	//void End();
};

