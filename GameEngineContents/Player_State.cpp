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
	case PlayerState::JUMP:
	{
		JumpStart();
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
	case PlayerState::JUMP:
	{
		JumpEnd();
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
	case PlayerState::JUMP:
	{
		JumpUpdate(_DeltaTime);
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
		if (true == GameEngineInput::IsDown("MoveLeft"))
		{
			ChangeState(PlayerState::MOVE);
			return;
		}

		if (true == GameEngineInput::IsDown("MoveRight"))
		{
			ChangeState(PlayerState::MOVE);
			return;
		}

		if (true == GameEngineInput::IsDown("Jump"))
		{
			ChangeState(PlayerState::JUMP);
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
		if ((LeftMoveAngle <= 1.0f && LeftMoveAngle >= -1.0f) || LeftMoveAngle >= 285.0f)
		{
			MoveDir += float4::Left * MoveSpeed;
		}
		else
		{
			ChangeState(PlayerState::IDLE);
			return;
		}
	}
	
	if (GameEngineInput::IsPress("MoveRight"))
	{
		if ((RightMoveAngle <= 181.0f && RightMoveAngle >= 179.0f) || RightMoveAngle <= 255.0f)
		{
			MoveDir += float4::Right * MoveSpeed;
		}
		else
		{
			ChangeState(PlayerState::IDLE);
			return;
		}
	}
}
void Player::MoveEnd()
{

}

void Player::JumpStart()
{
	AnimationDir = DirString;
	std::string AnimationName = "JumpReady";
	std::string AnimationText = AnimationDir.data() + AnimationName;
	AnimationRender->ChangeAnimation(AnimationText);
}
void Player::JumpUpdate(float _DeltatTime)
{
	if (AnimationRender->IsAnimationEnd())
	{
		std::string AnimationName = "FlyUp";
		std::string AnimationText = AnimationDir.data() + AnimationName;
		AnimationRender->ChangeAnimation(AnimationText);
	}

	if (true == IsGround)
	{
		ChangeState(PlayerState::IDLE);
		return;
	}
}
void Player::JumpEnd()
{

}