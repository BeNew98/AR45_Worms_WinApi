#include "PlayerHPUI.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineLevel.h>

PlayerHPUI::PlayerHPUI() 
{
}

PlayerHPUI::~PlayerHPUI() 
{
}



void PlayerHPUI::Render(float _DeltaTime)
{
	//���� UI ��ġ Ȯ��

	HDC DoubleDC = GameEngineWindow::GetDoubleBufferImage()->GetImageDC();
	float4 ActorPos = GetPos() - GetLevel()->GetCameraPos();;

	Rectangle(DoubleDC,
		ActorPos.ix() - 5,
		ActorPos.iy() - 5,
		ActorPos.ix() + 5,
		ActorPos.iy() + 5
	);
}
