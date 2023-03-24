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
    case PlayerState::EQUIPWEAPON:
    {
        EquipWeaponStart();
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
    case PlayerState::EQUIPWEAPON:
    {
        EquipWeaponEnd();
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
    case PlayerState::EQUIPWEAPON:
    {
        EquipWeaponUpdate(_DeltaTime);
        break;
    }
	default:
		break;
	}
}

void Player::IdleStart()
{
	DirCheck("Idle");
    StateCalTime = 0.0f;
    MoveDir = float4::Zero;


}
void Player::IdleUpdate(float _DeltatTime)
{
    StateCalTime += _DeltatTime;

	if (GameEngineInput::IsPress("MoveLeft") && GameEngineInput::IsPress("MoveRight"))
	{
		return;
	}

	if (true == IsMyTurn && true == CanIMove)
	{
		if (true == GameEngineInput::IsDown("MoveLeft"))
		{
            if (true == ReturnCanIMove(PlayerAngleDir::Left))
            {
                ChangeState(PlayerState::MOVE);
                return;
            }
		}

		if (true == GameEngineInput::IsDown("MoveRight"))
		{
            if (true == ReturnCanIMove(PlayerAngleDir::Right))
            {
                ChangeState(PlayerState::MOVE);
                return;
            }
		}

		if (true == GameEngineInput::IsDown("Jump"))
		{
			ChangeState(PlayerState::JUMP);
			return;
		}
	}

    if (CurWeapon != nullptr && true == IsMyTurn && StateCalTime >= 2.0f)
    {
        ChangeState(PlayerState::EQUIPWEAPON);
        return;
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

    if (false == IsGround)
    {
        ChangeState(PlayerState::IDLE);
        return;
    }



    if (GameEngineInput::IsPress("MoveLeft"))
    {
        if (true == ReturnCanIMove(PlayerAngleDir::Left))
        {
            MoveDir = float4::Left * MoveSpeed;
            MoveDir += float4::Down * Gravity;
        }
        else
        {
            ChangeState(PlayerState::IDLE);
            return;
        }
    }

    if (GameEngineInput::IsPress("MoveRight"))
    {
        if (true == ReturnCanIMove(PlayerAngleDir::Right))
        {
            MoveDir = float4::Right * MoveSpeed;
            MoveDir += float4::Down * Gravity;
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

    StateCalBool = true;
}
void Player::JumpUpdate(float _DeltatTime)
{
    if (true == IsGround && StateCalBool == false)
    {
        ChangeState(PlayerState::IDLE);
        return;
    }

    float4 NextPos = MoveDir * _DeltatTime;

    if (true == NextPosWallCheck(NextPos))
    {
        MoveDir = { -MoveDir.x, MoveDir.y };
    }

	if (true == AnimationRender->IsAnimationEnd() && true == StateCalBool)
	{
        if ("Right_" == DirString)
        {
            MoveDir += (float4::Up + float4::Right) * 150.0f;
        }
        else
        {
            MoveDir += (float4::Up + float4::Left) * 150.0f;
        }
       
		std::string AnimationName = "FlyUp";
		std::string AnimationText = AnimationDir.data() + AnimationName;
		AnimationRender->ChangeAnimation(AnimationText);

        StateCalBool = false;
	}

}
void Player::JumpEnd()
{

}

//void Player::EquipWeaponStart()
//{
//    //���� ���� �ִϸ��̼����� ����
//}
//void Player::EquipWeaponUpdate(float _DeltatTime)
//{
//
//}
//void Player::EquipWeaponEnd()
//{
//    //����� ���� �ִϸ��̼ǿ� ���� ����
//}