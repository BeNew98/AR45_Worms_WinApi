#include "PlayLevel.h"
#include "WeaponBazooka.h"
#include "Map.h"

#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>

#include "Player.h"
PlayLevel::PlayLevel() 
{
}

PlayLevel::~PlayLevel() 
{
}

void PlayLevel::SoundLoad()
{

}

void PlayLevel::ImageLoad()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Image");
	Dir.Move("Play");

	{
		Dir.Move("Map");

		GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("MapCity.bmp"));
		GameEngineImage* ColImage = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("MapCity_Ground.bmp"));

		Dir.MoveParent();
	}
	{
		Dir.Move("Weapon");

		GameEngineImage* Weapon1 = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Weapon1.bmp"));
		GameEngineImage* Weapon2 = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Weapon2.bmp"));

		Dir.MoveParent();
	}
	{
		Dir.Move("Player");
		Dir.Move("Image");
		{
			//Left 플레이어 이미지
			GameEngineImage* IdleLeft = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("IdleLeft.bmp"));
			IdleLeft->Cut(1, 6);
		}
		{
			//Right 플레이어 이미지
			GameEngineImage* IdleRight = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("IdleRight.bmp"));
			IdleRight->Cut(1, 6);
		}
	}

}

void PlayLevel::Loading()
{
	SoundLoad();
	ImageLoad();
	if (false == GameEngineInput::IsKey("ChangePlayer"))
	{
		GameEngineInput::CreateKey("ChangePlayer", 'n');
	}

	{
		Map* Actor = CreateActor<Map>();
	}
	{
		ScreenSize = GameEngineWindow::GetScreenSize();
		for (size_t i = 0; i < 8; i++)
		{
			int iRandxPos = GameEngineRandom::MainRandom.RandomInt(0, 399);

			vecAllPlayer.push_back(CreateActor<Player>());
			vecAllPlayer[i]->SetColImage("MapCity_Ground.bmp");

			float4 StartPos = float4{ 350,50 };
			StartPos.x *= i+1;
			StartPos.x += iRandxPos;
			vecAllPlayer[i]->SetPos(StartPos);
		}

		iPlayerNumber = 0;
		pCurPlayer = vecAllPlayer[iPlayerNumber];
	}

	CreateActor<WeaponBazooka>();
}

void PlayLevel::Update(float _DeltaTime)
{
	if (-1 == iPlayerNumber)
	{
		MsgAssert("PlayerNumber가 오류났습니다.");
	}
	if (GameEngineInput::IsDown("ChangePlayer"))
	{
		++iPlayerNumber;
		if (8 == iPlayerNumber)
		{
			iPlayerNumber = 0;
		}
		pCurPlayer = vecAllPlayer[iPlayerNumber];
	}
	if (GameEngineInput::IsDown("FreeMoveSwitch"))
	{
		bFreeCamMove = !bFreeCamMove;
	}

	if (false == bFreeCamMove)
	{
		float4 CurPlayerPos = pCurPlayer->GetPos();
		SetCameraPos(CurPlayerPos - ScreenSize.half());
	}
}

