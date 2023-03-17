#include "Weapon.h"
#include "Player.h"
#include "ContentsEnums.h"

#include <time.h>

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>


std::map<std::string, Weapon*> Weapon::AllWeapons;

Weapon::Weapon()
{
}

Weapon::~Weapon()
{
}


float4 Weapon::GetShootDir()
{
	float4 ReturnDir = float4::Zero;
	//if (GameEngineInput::IsKey("WeaponUp") == false)
	//{
	//	// ���� ����
	//	GameEngineInput::CreateKey("WeaponUp", 'W');
	//	GameEngineInput::CreateKey("WeaponDown", 'S');
	//	GameEngineInput::CreateKey("WeaponRight", 'D');
	//	GameEngineInput::CreateKey("WeaponLeft", 'A');

	//}

	if (GameEngineInput::IsPress("WeaponUp") && -1 < Height) // ������ ��ǥ�� ����
	{
		Height -= 0.05f;
	}
	else if (GameEngineInput::IsPress("WeaponDown") && 1 > Height)
	{
		Height += 0.05f;
	}

	// ���������� ������ ���� ������Ʈ
	if (GameEngineInput::IsDown("MoveRight"))
	{
	
		if (false == isRightDir)
		{
			Height = 0.0f;
		}
		isRightDir = true;
	}
	else if (GameEngineInput::IsDown("MoveLeft"))
	{
		if (true == isRightDir)
		{
			Height = 0.0f;
		}
		isRightDir = false;
	}

	ReturnDir.y = Height;
	ReturnDir.x = static_cast<float>(isRightDir ? 1 : -1);

	return ReturnDir.NormalizeReturn();

}


bool Weapon::PressShoot()
{
	/*if (GameEngineInput::IsKey("Shoot") == false)
	{
		GameEngineInput::CreateKey("Shoot", VK_SPACE);
	}*/

	if (GameEngineInput::IsDown("Shoot")) // ����
	{
		return true;
	}

	return false;
}

bool Weapon::isEndCharging() //�����̽��� ������ ��¡�� �ϴٰ� ������ ���� ������ ���۵ǹǷ� ������ ������ ���� �˻簡 �ʿ�
{
	if (GameEngineInput::IsUp("Shoot"))
	{
		return true;
	}

	if (/*�����̽��ٸ� ������ �ʾҴµ�, ���ѽð��� ������ ��*/ true)
	{
		//return true;
	}

	return false;
}

void Weapon::TimeCounting()
{
	CurTime = clock();
	TimeCount += (CurTime - PrevTime) / 1000;
	PrevTime = CurTime;
}


void Weapon::SetCurPlayer()
{
	std::vector<GameEngineActor*> PlayerList = GetLevel()->GetActors(WormsRenderOrder::Player);

	for (int i = 0; i < PlayerList.size(); i++)
	{
		if (true == dynamic_cast<Player*>(PlayerList[i])->GetIsMyTurn())
		{
			CurPlayer = dynamic_cast<Player*>(PlayerList[i]);
			break;
		}
	}
}