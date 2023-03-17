#include "Player.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "ContentsEnums.h"

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
	//�ִϸ��̼�
	{
		//����
		//CreatePlayerAnimation("Left_Idle", "IdleLeft.bmp", 0, 5, 0.1f);

		AnimationRender->CreateAnimation({ .AnimationName = "Left_Idle", .ImageName = "IdleLeft.bmp", .Start = 0, .End = 5, .InterTime = 0.1f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Move", .ImageName = "WalkLeft.bmp", .Start = 0, .End = 14, .InterTime = 0.1f });
	}
	{
		//����
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Idle", .ImageName = "IdleRight.bmp", .Start = 0, .End = 5, .InterTime = 0.1f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Move", .ImageName = "WalkRight.bmp", .Start = 0, .End = 14, .InterTime = 0.1f });

	}

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
	}

	ChangeState(PlayerState::IDLE);
}

void Player::SetColImage(const std::string_view& _Name)
{
	ColImage = GameEngineResources::GetInst().ImageFind(_Name.data());
}

void Player::Update(float _DeltaTime)
{
	MoveDir = float4::Zero; //�� �����Ӹ��� MoveDir �ʱ�ȭ
	
	if (true == IsMyTurn) //�� ���϶��� ������Ʈ ����
	{
		UpdateState(_DeltaTime);
	}
	GravityApplied();
	MoveCalculation(_DeltaTime);

}

void Player::GravityApplied()
{
	MoveDir += (float4::Down * Gravity);
}

void Player::MoveCalculation(float _DeltaTime)
{
	float4 NextPos = GetPos() + (MoveDir * _DeltaTime);
	NextPos = PullUpCharacter(NextPos, _DeltaTime);

	SetMove(MoveDir * _DeltaTime);
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


void Player::DirCheck(const std::string_view& _AnimationName)
{
	std::string PrevDirString = DirString;
	AnimationRender->ChangeAnimation(DirString + _AnimationName.data());

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
		AnimationRender->ChangeAnimation(DirString + _AnimationName.data());
	}
}

void Player::CreatePlayerAnimation(const std::string_view& _AnimationName, const std::string_view& _ImageName, int _StartFrame, int _EndFrame, float _InterTime, bool _Loop)
{
	AnimationRender->CreateAnimation({ .AnimationName = _AnimationName, .ImageName = _ImageName, .Start = _StartFrame, .End = _EndFrame, .InterTime = _InterTime, .Loop = _Loop });
}
void Player::ChangePlayerAnimation(const std::string_view& _AnimationName)
{
	DirCheck(_AnimationName);
}


void Player::Render(float _DeltaTime)
{
	HDC DoubleDC = GameEngineWindow::GetDoubleBufferImage()->GetImageDC();
	float4 ActorPos = GetPos() - GetLevel()->GetCameraPos();

	//��ġ Ȯ�ο�
	Rectangle(DoubleDC,
		ActorPos.ix() - 5,
		ActorPos.iy() - 5,
		ActorPos.ix() + 5,
		ActorPos.iy() + 5
	);
}
