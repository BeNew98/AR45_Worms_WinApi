#include "Player.h"

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

	}
	case PlayerState::MOVE:
	{

	}
	default:
		break;
	}

	switch (PrevState)
	{
	case PlayerState::IDLE:
	{

	}
	case PlayerState::MOVE:
	{

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

	}
	case PlayerState::MOVE:
	{

	}
	default:
		break;
	}
}