#pragma once
#include <GameEngineCore/GameEngineActor.h>

enum class PlayerState
{
	IDLE,
	MOVE,
};

// ���� : �÷��̾� Ŭ����
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

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:

	//�÷��̾� ������Ʈ ����
	PlayerState StateValue = PlayerState::IDLE;

	//�÷��̾��� �̵�����
	float4 MoveDir = float4::Zero;

	//�̵� ���� �Լ�
	void MoveCalculation(float _DeltaTime);

	void ChangeState(PlayerState _State);
	void UpdateState(float _Time);
};

