#include "PlayerHPUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/NumberRenderObject.h>

#include "ContentsEnums.h"

PlayerHPUI::PlayerHPUI() 
{
}

PlayerHPUI::~PlayerHPUI() 
{
}

void PlayerHPUI::Start()
{
	SelectPlayerRender = CreateRender(WormsRenderOrder::UI);
	HPBarImageRender = CreateRender(WormsRenderOrder::UI);
	IDRender = CreateRender(WormsRenderOrder::UI);
}

void PlayerHPUI::Update(float _DeltaTime)
{
	if (this->IsUpdate())
	{
		if (CurrentValue > *HPValue)
		{
			--CurrentValue;
		}

		if (CurrentValue < 0)
		{
			CurrentValue = 0;
		}

		PlayerHPNumberRender.SetValue(CurrentValue);
	}

}

void PlayerHPUI::SetPlayerHPUI(const std::string_view& _HPNumberImage, const std::string_view& _NametagImage, const std::string_view& _ArrowImage, int *_PlayerHP)
{

	PlayerHPNumberRender.SetOwner(this);
	PlayerHPNumberRender.SetCameraEffect(true);
	PlayerHPNumberRender.SetImage(_HPNumberImage, { 10, 10 }, 10, RGB(255, 0, 255));
	PlayerHPNumberRender.SetAlign(Align::Center);
	
	HPValue = _PlayerHP;
	CurrentValue = *_PlayerHP;
	
	PlayerHPNumberRender.SetValue(CurrentValue);

	IDRender->SetImage(_NametagImage);
	IDRender->SetScaleToImage();
	IDRender->SetPosition({ 0, -17 });

	HPBarImageRender->SetImage("PlayerHPBackground.bmp");
	HPBarImageRender->SetScaleToImage();

	SelectPlayerRender->SetOwner(this);
	SelectPlayerRender->SetPosition({ 0, -55 });
	SelectPlayerRender->SetScale({ 60, 60 });
	SelectPlayerRender->CreateAnimation({ .AnimationName = "ArrowAnimation", .ImageName = _ArrowImage, .Start = 0, .End = 29, .InterTime = 0.05f });
	SelectPlayerRender->ChangeAnimation("ArrowAnimation");

	SelectPlayerRender->Off();
}

void PlayerHPUI::SetSelectPlayerRender(bool _Value)
{
	if (true == _Value)
	{
		SelectPlayerRender->On();
	}
	else
	{
		SelectPlayerRender->Off();
	}
}

void PlayerHPUI::Render(float _DeltaTime)
{
	//���� UI ��ġ Ȯ��

	HDC DoubleDC = GameEngineWindow::GetDoubleBufferImage()->GetImageDC();
	float4 ActorPos = GetPos() - GetLevel()->GetCameraPos();

	Rectangle(DoubleDC,
		ActorPos.ix() - 5,
		ActorPos.iy() - 5,
		ActorPos.ix() + 5,
		ActorPos.iy() + 5
	);
}
