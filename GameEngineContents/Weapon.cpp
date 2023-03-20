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

	if (GameEngineInput::IsPress("WeaponUp") && -5.7 < Height) // ������ ��ǥ�� ����
	{
		Height -= 0.05f;
	}
	else if (GameEngineInput::IsPress("WeaponDown") && 5.7 > Height)
	{
		Height += 0.05f;
	}

	float4 PlayerDir = CurPlayer->GetPlayerDir();

	// ���������� ������ ���� ������Ʈ
	
	if (float4::Left == PlayerDir)
	{
		if (isRightDir == true)
		{
			Height = 0.0f;
		}

		isRightDir = false;
	}
	else if (float4::Right == PlayerDir)
	{
		if (isRightDir == false)
		{
			Height = 0.0f;
		}

		isRightDir = true;
	}

	//if (GameEngineInput::IsDown("MoveRight"))
	//{		
	//	if (float4::Left == PlayerDir)
	//	{
	//		Height = 0.0f;
	//	}
	//	isRightDir = true;
	//}
	//else if (GameEngineInput::IsDown("MoveLeft"))
	//{
	//	if (float4::Right == PlayerDir)
	//	{
	//		Height = 0.0f;
	//	}
	//	isRightDir = false;
	//}

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
	TimeCount_2 += (CurTime - PrevTime) / 1000;

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

bool Weapon::CheckCollision(GameEngineCollision* _Col)
{
	if (nullptr == _Col)
	{
		_Col = WeaponCollision;
	}
	std::vector<GameEngineCollision*> CollisionList;

	if (_Col != nullptr && true == _Col->Collision({ .TargetGroup = static_cast<int>(WormsCollisionOrder::Player), .TargetColType = CollisionType::CT_CirCle, .ThisColType = CollisionType::CT_CirCle }, CollisionList))
	{
		for (int i = 0; i < CollisionList.size(); i++)
		{
			Player* ColPlayer = dynamic_cast<Player*>(CollisionList[i]->GetActor());

			if (ColPlayer->GetIsMyTurn() == false)
			{
				return true;
			}
		}
	}
	else if (RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos(), RGB(255, 0, 255))) //�ʿ� ������ �����
	{
		return true;
	}

	return false;
}

float Weapon::GetChargeTime()
{
	return GameEngineInput::GetPressTime("Shoot");
}

float4 Weapon::CheckCollisionSide()
{
	float4 ReturnValue = float4::Zero;
	std::vector<GameEngineCollision*> CollisionList;

	if (true == WeaponCollision->Collision({ .TargetGroup = static_cast<int>(WormsCollisionOrder::Player), .TargetColType = CollisionType::CT_CirCle, .ThisColType = CollisionType::CT_CirCle }, CollisionList))
	{
		for (int i = 0; i < CollisionList.size(); i++)
		{
			Player* ColPlayer = dynamic_cast<Player*>(CollisionList[i]->GetActor());

			if (ColPlayer->GetIsMyTurn() == false)
			{
				float4 Range = (ColPlayer->GetPos() - GetPos());
				if (abs(Range.y) < abs(Range.x))
				{
					ReturnValue.y += 1;
				}
				else if (Range.x > 0)
				{
					ReturnValue.x += 1;
				}
				else
				{
					ReturnValue.x -= 1;
				}
				return ReturnValue;
			}
		}
	}

	// 30 30 20,40
	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(WeaponCollision->GetActorPlusPos() + float4{ 10,0 }, RGB(255, 0, 255)))
	{
		ReturnValue.x += 1;
	}
	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(WeaponCollision->GetActorPlusPos() + float4{ -10,0 }, RGB(255, 0, 255)))
	{
		ReturnValue.x -= 1;
	}
	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(WeaponCollision->GetActorPlusPos() + float4{ 0,2  }, RGB(255, 0, 255)))
	{
		ReturnValue.y += 1;
	}

	return ReturnValue;
}