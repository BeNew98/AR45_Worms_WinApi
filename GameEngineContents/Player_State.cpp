#include "Player.h"
#include <GameEnginePlatform/GameEngineInput.h>

void Player::ChangeState(PlayerState _State)
{
	PlayerState NextState = _State;
	PlayerState PrevState = StateValue;

	StateValue = NextState;

	//���°� ������ ��, ���� �� �ʿ��� �ڵ�, ������ �� �ʿ��� �ڵ带 ����ϱ� ����
	switch (NextState)
	{
	case PlayerState::IDLE:
	{
		IdleStart();
		break;
	}
	case PlayerState::MOVE:
	{
		MoveStart();
		break;
	}
	default:
		break;
	}

	switch (PrevState)
	{
	case PlayerState::IDLE:
	{
		IdleEnd();
		break;
	}
	case PlayerState::MOVE:
	{
		MoveEnd();
		break;
	}
	default:
		break;
	}
}

void Player::UpdateState(float _DeltaTime)
{
	switch (StateValue)
	{
	case PlayerState::IDLE:
	{
		IdleUpdate(_DeltaTime);
		break;
	}
	case PlayerState::MOVE:
	{
		MoveUpdate(_DeltaTime);
		break;
	}
	default:
		break;
	}
}

void Player::IdleStart()
{
	DirCheck("Idle");
}
void Player::IdleUpdate(float _DeltatTime)
{
	if (GameEngineInput::IsPress("MoveLeft") && GameEngineInput::IsPress("MoveRight"))
	{
		return;
	}

	if (true == CanIMove)
	{
		if (true == GameEngineInput::IsPress("MoveLeft"))
		{
			ChangeState(PlayerState::MOVE);
			return;
		}

		if (true == GameEngineInput::IsPress("MoveRight"))
		{
			ChangeState(PlayerState::MOVE);
			return;
		}
	}
}
void Player::IdleEnd()
{

}

void Player::MoveStart()
{
	DirCheck("Move");
}
void Player::MoveUpdate(float _DeltatTime)
{
	//���ÿ� ������ �������� ����
	if (GameEngineInput::IsPress("MoveLeft") && GameEngineInput::IsPress("MoveRight"))
	{
		ChangeState(PlayerState::IDLE);
		return;
	}

	//�̵�Ű ���߿� �ϳ��� �������� ������ Idle���·�
	if (GameEngineInput::IsPress("MoveLeft") == false && GameEngineInput::IsPress("MoveRight") == false)
	{
		ChangeState(PlayerState::IDLE);
		return;
	}

	if (GameEngineInput::IsPress("MoveLeft"))
	{
		MoveDir += float4::Left * MoveSpeed;
	}
	else if (GameEngineInput::IsPress("MoveRight"))
	{
		MoveDir += float4::Right * MoveSpeed;
	}
}
void Player::MoveEnd()
{

}