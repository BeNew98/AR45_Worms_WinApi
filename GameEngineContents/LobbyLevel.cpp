#include "LobbyLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include "Lobby.h"
LobbyLevel::LobbyLevel()
{
}

LobbyLevel::~LobbyLevel()
{
}

void LobbyLevel::Loading()
{
    GameEngineDirectory Dir;
    Dir.MoveParentToDirectory("ContentsResources");
    Dir.Move("ContentsResources");
    Dir.Move("Image");
    Dir.Move("Title");

    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("MapSelect.BMP"));
    }

    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Teams.BMP"));
    }

    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Barracks.BMP"));
    }

    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Option.BMP"));
    }

    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("StartGame.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Exit.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("8436.BMP"));
    }
    
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("24226.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("5618.BMP"));
    }

    // �� 
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("LobbyMap_MapBooks.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("LobbyMap_MapCars.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("LobbyMap_MapCity.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("LobbyMap_MapTrain.BMP"));
    }
    // �ɼ�
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("weaponoptions.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("gameoptions.BMP"));
    }   
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("11280.BMP"));
    }
    // �÷��̾� 
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("1Up.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("2Up.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("3Up.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("4Up.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("5Up.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("6Up.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("1.BMP"));
    }
    // �Ͻð� 
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("000000.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("000001.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("000002.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("000003.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("000004.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("000005.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("000006.BMP"));
    }
    // ���� �ð� 
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Round0.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Round1.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Round2.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Round3.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Round4.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Round5.BMP"));
    }
    {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Round6.BMP"));
    }
    // �̱�� Ƚ�� 
    {
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Wins0.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Wins1.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Wins2.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Wins3.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Wins4.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Wins5.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Wins6.BMP"));
        }

    }
    // �� ����
    {
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WormSelectOFF.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WormSelectON.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WormSelectRANDOM.BMP"));
        }
    }
    // �� ü�� 
    {
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Hp00.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Hp100.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Hp150.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Hp200.BMP"));
        }
    }
    // �ڷ���Ʈ �¿��� 
    {
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("TeleportinOFF.BMP"));
        }
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("TeleportinON.BMP"));
        }
    }

    //Select
    {
        {
            GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("6868.BMP"));
        }
    }
    //Cusor

   /* {
        GameEngineImage* Image = GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Cursor.BMP"));
    }*/

    CreateActor<Lobby>();
}

void LobbyLevel::Update(float _DeltaTime)
{
}
