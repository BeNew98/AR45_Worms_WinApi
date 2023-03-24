#include "Weapon.h"
#include "Player.h"
#include "ContentsEnums.h"
#include "GlobalValue.h"
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

	if (GameEngineInput::IsPress("WeaponUp") && -5.7 < Height) // ?????? ????? ????
	{
		Height -= 0.05f;
	}
	else if (GameEngineInput::IsPress("WeaponDown") && 5.7 > Height)
	{
		Height += 0.05f;
	}

	if (nullptr == CurPlayer)
	{
		MsgAssert("???? ???? ?��???? ???????.");
	}
	float4 PlayerDir = CurPlayer->GetPlayerDir();

	// ?????????? ?????? ???? ???????

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

	ReturnDir.y = Height;
	ReturnDir.x = static_cast<float>(isRightDir ? 1 : -1);

	return ReturnDir.NormalizeReturn();

}


bool Weapon::PressShoot()
{
	if (GameEngineInput::IsPress("Shoot")) // ����
	{
		return true;
	}

	return false;
}

bool Weapon::isEndCharging() //????????? ?????? ??��?? ???? ?????? ???? ?????? ??????? ?????? ?????? ???? ??�� ???
{
	if (GameEngineInput::IsUp("Shoot"))
	{
		return true;
	}

	if (/*?????????? ?????? ?????, ????��??? ?????? ??*/ true)
	{
		//return true;
	}

	return false;
}



void Weapon::SetCurPlayer() // �����Ϸ�
{
	Player* tmpPlayer = GlobalValue::gValue.GetPlayer();
	if (tmpPlayer != CurPlayer)
	{
		CurPlayer = tmpPlayer;
        GetShootDir();
	}

	if (nullptr == CurPlayer)
	{
		MsgAssert("???? ???? ?��???? ???????.");
	}
}

bool Weapon::CheckCollision(GameEngineCollision* _Col)
{
	if (nullptr == _Col)
	{
		MsgAssert("üũ�� �ݸ����� �����ϴ�.");
	}

	// �÷��̾� üũ
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

	// ��üũ
	else if (RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos(), RGB(0, 0, 255)) && _Col->GetActorPlusPos().y > 0) //�ʿ� ������ �����
	{
		return true;
	}
	// �� ������ �������� üũ

	return false;
}

float Weapon::GetChargeTime()
{
	return GameEngineInput::GetPressTime("Shoot");
}

float4 Weapon::CheckCollisionSide(GameEngineCollision* _Col)
{
	float4 ReturnValue = float4::Zero;
	std::vector<GameEngineCollision*> CollisionList;

	if (true == _Col->Collision({ .TargetGroup = static_cast<int>(WormsCollisionOrder::Player), .TargetColType = CollisionType::CT_CirCle, .ThisColType = CollisionType::CT_CirCle }, CollisionList))
	{
		for (int i = 0; i < CollisionList.size(); i++)
		{
			Player* ColPlayer = dynamic_cast<Player*>(CollisionList[i]->GetActor());

			//if (ColPlayer->GetIsMyTurn() == false)
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
	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos() + float4{ 10,0 }, RGB(255, 0, 255)))
	{
		ReturnValue.x += 1;
	}
	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos() + float4{ -10,0 }, RGB(255, 0, 255)))
	{
		ReturnValue.x -= 1;
	}
	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos() + float4{ 0,2  }, RGB(255, 0, 255)))
	{
		ReturnValue.y += 1;
	}

	return ReturnValue;
}


void Weapon::AttackPlayer(GameEngineCollision* _Col) // �����ʿ�
{

	if (nullptr == _Col)
	{
		MsgAssert("üũ�� �ݸ����� �����ϴ�.");
	}

	// �÷��̾� üũ
	std::vector<GameEngineCollision*> CollisionList;

	if (_Col != nullptr && true == _Col->Collision({ .TargetGroup = static_cast<int>(WormsCollisionOrder::Player), .TargetColType = CollisionType::CT_CirCle, .ThisColType = CollisionType::CT_CirCle }, CollisionList))
	{
		for (int i = 0; i < CollisionList.size(); i++)
		{
			Player* ColPlayer = dynamic_cast<Player*>(CollisionList[i]->GetActor());
			//ColPlayer���� �������� ���󰡴� ����, ���󰡴� ���� �ֱ�

		}
	}
}
