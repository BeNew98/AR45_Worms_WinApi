#include "Weapon.h"

#include <GameEnginePlatform/GameEngineInput.h>

Weapon::Weapon()
{
}

Weapon::~Weapon()
{
}


float4 Weapon::GetShootDir()
{
	float4 ReturnDir = float4::Zero;
	if (GameEngineInput::IsKey("WeaponUp") == false)
	{
		// ���� ����
		GameEngineInput::CreateKey("WeaponUp", 'W');
		GameEngineInput::CreateKey("WeaponDown", 'S');
		GameEngineInput::CreateKey("WeaponRight", 'D');
		GameEngineInput::CreateKey("WeaponLeft", 'A');

	}

	if (GameEngineInput::IsPress("WeaponUp") && -1 < Height) // ������ ��ǥ�� ����
	{
		Height -= 0.05f;
	}
	else if (GameEngineInput::IsPress("WeaponDown") && 1 > Height)
	{
		Height += 0.05f;
	}

	// ���������� ������ ���� ������Ʈ
	if (GameEngineInput::IsPress("WeaponRight"))
	{
	
		if (false == isRightDir)
		{
			Height = 0.0f;
		}
		isRightDir = true;
	}
	else if (GameEngineInput::IsPress("WeaponLeft"))
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
	if (GameEngineInput::IsKey("Shoot") == false)
	{
		GameEngineInput::CreateKey("Shoot", VK_SPACE);
	}

	if (GameEngineInput::IsPress("Shoot")) // ����
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