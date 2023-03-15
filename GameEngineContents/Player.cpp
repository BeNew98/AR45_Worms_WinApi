#include "Player.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>

Player::Player() 
{
}

Player::~Player() 
{
}

void Player::Start()
{
	SetMove(GameEngineWindow::GetScreenSize().half());

	//Ű ������ �ȵǾ������� Ű ����( �÷��̾ ������ �� ��츦 ����Ͽ� Ű ������ ������ ������ ) 
	if (GameEngineInput::IsKey("MoveLeft") == false)
	{
		//ĳ���� �̵� �� �ൿ
		GameEngineInput::CreateKey("MoveLeft", 'A');
		GameEngineInput::CreateKey("MoveRight", 'D');
	}

	ChangeState(PlayerState::IDLE);
}

void Player::Update(float _DeltaTime)
{
	MoveDir = float4::Zero; //�� �����Ӹ��� MoveDir �ʱ�ȭ

	MoveCalculation(_DeltaTime);
	UpdateState(_DeltaTime);
}

void Player::MoveCalculation(float _DeltaTime)
{
	SetMove(MoveDir * _DeltaTime);
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