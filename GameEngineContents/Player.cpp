#include "Player.h"

#include<iostream>

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "PlayerHPUI.h"
#include "PlayerGetDamagedUI.h"
#include "ContentsEnums.h"
#include "Weapon.h"

Player::Player() 
{
}

Player::~Player() 
{
}

void Player::Start()
{
	AnimationRender = CreateRender(WormsRenderOrder::Player);
	AnimationRender->SetScale({ 60, 60 });
	AnimationRender->SetPosition({ 0, -10 });
    CreateAnimation();

	//�ݸ���
	{
		BodyCollision = CreateCollision(WormsCollisionOrder::Player);
		BodyCollision->SetMove({ 0, -10 });
		BodyCollision->SetScale({ 30 , 30 });
	}

	//Ű ������ �ȵǾ������� Ű ����( �÷��̾ ������ �� ��츦 ����Ͽ� Ű ������ ������ ������ ) 
	if (GameEngineInput::IsKey("MoveLeft") == false)
	{
		//ĳ���� �̵� �� �ൿ
		GameEngineInput::CreateKey("MoveRight", 'D');
		GameEngineInput::CreateKey("MoveLeft", 'A');
		GameEngineInput::CreateKey("Jump", 'K');

		GameEngineInput::CreateKey("TestButton", 'M');
	}

	SetHPUI("RedNumberRender.bmp", "RedNameTag.bmp", "PlayerSelectArrowRed.bmp");
	ChangeState(PlayerState::IDLE);
}

void Player::SetHPUI(const std::string_view& _HPNumberImage, const std::string_view& _NametagImage, const std::string_view& _ArrowImage) //HP �̹��� ���� �������ִ� �޼���
{
	PlayerHPNumberImageStringView = _HPNumberImage;

	HPUI = GetLevel()->CreateActor<PlayerHPUI>();
	HPUI->SetPlayerHPUI(_HPNumberImage, _NametagImage, _ArrowImage, &PlayerHP);
}

void Player::SetIsMyTurn(bool _Value)
{
	IsMyTurn = _Value;
	HPUI->SetSelectPlayerRender(_Value);
}

void Player::SetColImage(const std::string_view& _Name)
{
	ColImage = GameEngineResources::GetInst().ImageFind(_Name.data());
}

void Player::Test()
{
	if (GameEngineInput::IsDown("TestButton"))
	{
		Damaged(5);
	}
}

void Player::Damaged(int _Damage)
{
	if (DamagedTime >= 0.5f)
	{
		PlayerHP -= _Damage;
		DamagedTime = 0.0f;
		
		PlayerGetDamagedUI* DamagedUI = GetLevel()->CreateActor<PlayerGetDamagedUI>();
		DamagedUI->SetDamagedUI(PlayerHPNumberImageStringView, _Damage);

		float4 DamagedUIPos = { HPUI->GetPos().x, HPUI->GetPos().y - 20.0f };
		DamagedUI->SetPos(DamagedUIPos);
	}
}

void Player::CheckTurn()
{
    if (false == IsMyTurn || nullptr == CurWeapon)
    {
        return;
    }

    if (true == CurWeapon->IsWeaponDone())
    {
        IsMyTurn = false;
    }
}

void Player::Update(float _DeltaTime)
{
	//MoveDir = float4::Zero; //�� �����Ӹ��� MoveDir �ʱ�ȭ
    UpdateState(_DeltaTime);
    GravityApplied(_DeltaTime);
	MoveCalculation(_DeltaTime);
	
    IsGroundCheck();
    CheckTurn();
    Test();

	HPUI->SetPos({GetPos().x , GetPos().y - 50.0f}); //UI �����Ӹ��� ��ġ ����
	DamagedTime += _DeltaTime; //�÷��̾ �ѹ��� �������� �������� �����ʱ� ���� ����
}

bool Player::NextPosWallCheck(float4 _NextPos)
{
    if (RGB(0, 0, 255) == ColImage->GetPixelColor(_NextPos, RGB(0, 0, 0)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Player::GravityApplied(float _DeltaTime)
{
	MoveDir += (float4::Down * Gravity * _DeltaTime);
}

void Player::MoveCalculation(float _DeltaTime)
{
	float4 NextPos = GetPos() + (MoveDir * _DeltaTime);
	NextPos = PullUpCharacter(NextPos, _DeltaTime);

	SetMoveAngle();
	SetMove(MoveDir * _DeltaTime);
}

void Player::IsGroundCheck()
{
	float4 PlayerGroundCheckPos = { GetPos().x , GetPos().y + 1 };

	if (RGB(0, 0, 255) == ColImage->GetPixelColor(PlayerGroundCheckPos, RGB(0, 0, 0)))
	{
		IsGround = true;
	}
	else
	{
		IsGround = false;
	}
}

void Player::SetMoveAngle()
{
	float4 PlayerLeftPos = { GetPos().x - 5, GetPos().y };
	float4 PlayerRightPos = { GetPos().x + 5, GetPos().y };

	if (RGB(0, 0, 255) == ColImage->GetPixelColor(PlayerLeftPos, RGB(0, 0, 0)))
	{
		while (true)
		{
			PlayerLeftPos = { PlayerLeftPos.x, PlayerLeftPos.y - 1 };

			if (RGB(0, 0, 255) != ColImage->GetPixelColor(PlayerLeftPos, RGB(0, 0, 0)))
			{
				break;
			}
		}
	}

	if (RGB(0, 0, 255) == ColImage->GetPixelColor(PlayerRightPos, RGB(0, 0, 0)))
	{
		while (true)
		{
			PlayerRightPos = { PlayerRightPos.x, PlayerRightPos.y - 1 };

			if (RGB(0, 0, 255) != ColImage->GetPixelColor(PlayerRightPos, RGB(0, 0, 0)))
			{
				break;
			}
		}

	}

	float4 LeftAngleVector = GetPos() - PlayerLeftPos;
	LeftMoveAngle = LeftAngleVector.NormalizeReturn().GetAnagleDeg();

	float4 RightAngleVector = GetPos() - PlayerRightPos;
	RightMoveAngle = RightAngleVector.NormalizeReturn().GetAnagleDeg();
}

bool Player::ReturnCanIMove(PlayerAngleDir _Dir)
{
    if (PlayerAngleDir::Left == _Dir)
    {
        if ((LeftMoveAngle <= 1.0f && LeftMoveAngle >= -1.0f) || LeftMoveAngle >= 270 + AngleLimit)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ((RightMoveAngle <= 181.0f && RightMoveAngle >= 179.0f) || RightMoveAngle <= 270 - AngleLimit)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

float4 Player::PullUpCharacter(float4 _NextPos, float _DeltaTime)
{
	if (RGB(0, 0, 255) != ColImage->GetPixelColor(_NextPos, RGB(0, 0, 255)))
	{
		return _NextPos;
	}

	while (true)
	{
		MoveDir.y -= 1;

		_NextPos = GetPos() + MoveDir * _DeltaTime;

		if (RGB(0, 0, 255) == ColImage->GetPixelColor(_NextPos, RGB(0, 0, 255)))
		{
			continue;
		}

		return _NextPos;
	}
}


void Player::DirCheck(const std::string_view& _AnimationName, int _CurIndex)
{
	std::string PrevDirString = DirString;

    if (PlayerState::EQUIPWEAPON == StateValue)
    {
        AnimationRender->ChangeAnimation(DirString + _AnimationName.data(), _CurIndex, true);
    }
    else
    {
        AnimationRender->ChangeAnimation(DirString + _AnimationName.data(), _CurIndex);
    }

	if (GameEngineInput::IsPress("MoveLeft"))
	{
		DirString = "Left_";
	}
	else if (GameEngineInput::IsPress("MoveRight"))
	{
		DirString = "Right_";
	}

	if (PrevDirString != DirString)
	{
		AnimationRender->ChangeAnimation(DirString + _AnimationName.data(), _CurIndex);
	}
}

void Player::CreatePlayerAnimation(const std::string_view& _AnimationName, const std::string_view& _ImageName, int _StartFrame, int _EndFrame, float _InterTime, bool _Loop)
{
	AnimationRender->CreateAnimation({ .AnimationName = _AnimationName, .ImageName = _ImageName, .Start = _StartFrame, .End = _EndFrame, .InterTime = _InterTime, .Loop = _Loop });
}

void Player::ChangePlayerAnimation(const std::string_view& _AnimationName, int _CurIndex)
{
	DirCheck(_AnimationName, _CurIndex);
}

void Player::SetPlayerAnimationFrame(int _Frame)
{
	AnimationRender->SetAnimationFrame(_Frame);
}

void Player::Render(float _DeltaTime)
{
	HDC DoubleDC = GameEngineWindow::GetDoubleBufferImage()->GetImageDC();
	//float4 ActorPos = GetPos() - GetLevel()->GetCameraPos();

	////��ġ Ȯ�ο�
	//Rectangle(DoubleDC,
	//	ActorPos.ix() - 5,
	//	ActorPos.iy() - 5,
	//	ActorPos.ix() + 5,
	//	ActorPos.iy() + 5
	//);

	if (true == IsMyTurn)
	{
		std::string PlayerLeftAngle = "PlayerLeftAngle = ";
		PlayerLeftAngle = PlayerLeftAngle + std::to_string(LeftMoveAngle);
		GameEngineLevel::DebugTextPush(PlayerLeftAngle);

		std::string PlayerRightAngle = "PlayerRightAngle = ";
		PlayerRightAngle = PlayerRightAngle + std::to_string(RightMoveAngle);
		GameEngineLevel::DebugTextPush(PlayerRightAngle);

		std::string PlayerIsGround = "PlayerIsGround = ";
		PlayerIsGround = PlayerIsGround + std::to_string(IsGround);
		GameEngineLevel::DebugTextPush(PlayerIsGround);

        float4 NextPos = (GetPos() + MoveDir * _DeltaTime) - GetLevel()->GetCameraPos();

        Rectangle(DoubleDC,
            NextPos.ix() - 5,
            NextPos.iy() - 5,
            NextPos.ix() + 5,
            NextPos.iy() + 5
        );

        std::string NexPosWallCheck = "NexPosWallCheck = ";
        NexPosWallCheck = NexPosWallCheck + std::to_string(NextPosWallCheck(NextPos));
        GameEngineLevel::DebugTextPush(NexPosWallCheck);

        if (nullptr != CurWeapon)
        {
            std::string PlayerCurWeapon = "PlayerCurWeapon = ";
            PlayerCurWeapon = PlayerCurWeapon + std::to_string(CurWeapon->GetWeaponNumber());
            GameEngineLevel::DebugTextPush(PlayerCurWeapon);
        }

        std::string PlayerStateText = "PlayerState = ";

        switch (StateValue)
        {
        case PlayerState::IDLE:
            PlayerStateText = PlayerStateText + "IDLE ����";
            break;
        case PlayerState::MOVE:
            PlayerStateText = PlayerStateText + "MOVE ����";
            break;
        case PlayerState::JUMP:
            PlayerStateText = PlayerStateText + "JUMP ����";
            break;
        case PlayerState::EQUIPWEAPON:
            PlayerStateText = PlayerStateText + "EQUIPWEAPON ����";
            break;
        default:
            PlayerStateText = PlayerStateText + "UNKNOWN ����";
            break;
        }

        GameEngineLevel::DebugTextPush(PlayerStateText);
	}

}

bool Player::IsPlayerAnimationEnd()
{
	return AnimationRender->IsAnimationEnd();
}