
#include <functional>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "WeaponInterFace.h"
#include "ContentsEnums.h"
#include "PlayLevel.h"
#include "GlobalValue.h"
#include "MouseObject.h"
#include "Player.h"
#include "Cusor.h"
#include "WeaponBazooka.h"
#include "WeaponGrenade.h"
#include "WeaponClusterBomb.h"
#include "WeaponShotgun.h"
#include "HomingMissile.h"
#include "WeaponHandgun.h"
#include "WeaponUzi.h"
#include "WeaponMinigun.h"
#include "WeaponTorch.h"
#include "WeaponSheep.h"
#include "WeaponAirStrike.h"

WeaponInterFace*WeaponInterFace::Interface;
GameEngineLevel* WeaponInterFace::Value;

//void asas(Button* a) // ���� �� �ڵ�
void asas(Button*a, int _Enum) // int�� �޴� �Լ��� ������ �ְ� ���� ���� ����
{
    WeaponNum eNum = static_cast<WeaponNum>(_Enum);

	Player* CurPlayer = GlobalValue::gValue.GetPlayer();
	//if (CurPlayer->GetCurWeapon()->GetName() == "B")
	//{
	//	return;
	//}
	if (CurPlayer->GetCurWeapon() != nullptr)
	{
		CurPlayer->GetCurWeapon()->Death();

	}

    switch (eNum)
    {
    case WeaponNum::None:
        break;
    case WeaponNum::Bazooka: 
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<WeaponBazooka>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;
    }
       
    case WeaponNum::HomingMissile:
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<HomingMissile>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;
    }
    
    case WeaponNum::Grenade: 
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<WeaponGrenade>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;
    }
  
    case WeaponNum::ClusterBomb:
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<WeaponClusterBomb>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;
    }
   
    case WeaponNum::Shotgun:
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<WeaponShotgun>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;
    }
  
    case WeaponNum::Handgun:
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<WeaponHandgun>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;

      
    }
 
    case WeaponNum::Uzi:
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<WeaponUzi>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;
    }
   
    case WeaponNum::Minigun:
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<WeaponMinigun>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;
    }
  
    case WeaponNum::Sheep:
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<WeaponSheep>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;
    }

    case WeaponNum::AirStrike:
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<WeaponAirStrike>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;
    }

    case WeaponNum::Torch:
    {
        Weapon* NewWeapon = WeaponInterFace::Value->CreateActor<WeaponTorch>();
        CurPlayer->SetCurWeapon(NewWeapon);
        break;
    }

    default:
        break;
    }
        
	/*if (CurWeapon == nullptr)
	{
		GameEngineActor* Weapon = WeaponInterFace::Value->CreateActor<WeaponBazooka>();
	}
	Gmsf::dsds()
	int a = 1;*/
	
}


WeaponInterFace::WeaponInterFace()
{
}


WeaponInterFace::~WeaponInterFace()
{
}



void WeaponInterFace::Start()
{
    Cusor* Actor = GetLevel()->CreateActor<Cusor>();
	
	
	if (false == GameEngineInput::IsKey("RightClock"))
	{
		GameEngineInput::CreateKey("RightClock", VK_RBUTTON);
		
	}
	WeaponInterFace::Value = GetLevel();
	MouseObject* MouseObjectInst = GetLevel()->CreateActor<MouseObject>();
	
	// 1���� �������̽� 
	
	// ����ī 
	{
        Bazooka = GetLevel()->CreateActor<Button>();
		Bazooka->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,586 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
        Bazooka->SetEnum(WeaponNum::Bazooka); //SetEnum���� ���θ��� ButtonCallbackEnum�� Enum���õǰԲ������.
        Bazooka->SetClickCallBackEnum(asas);
        Bazooka->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        ButtonManager.push_back(Bazooka);
	}
    // ȣ�ֹ̻��� 
	{
        HomingMissile = GetLevel()->CreateActor<Button>();
        HomingMissile->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,586 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
        HomingMissile->SetEnum(WeaponNum::HomingMissile);
        HomingMissile->SetClickCallBackEnum(asas);
        HomingMissile->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
     	ButtonManager.push_back(HomingMissile);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,586 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,586 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        ButtonManager.push_back(button);   
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,586 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        button->SetEnum(WeaponNum::Shotgun);
        ButtonManager.push_back(button);
        button->SetClickCallBackEnum(asas);
	}

	// 2���� �������̽� 

	// �ʷ���ź 
	{
		Grenade = GetLevel()->CreateActor<Button>();
		Grenade->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,615 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		Grenade->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		Grenade->SetEnum(WeaponNum::Grenade);

		ButtonManager.push_back(Grenade);
        Grenade->SetClickCallBackEnum(asas);
	}
	// ������ź 
	{
		ClusterBomb = GetLevel()->CreateActor<Button>();
        ClusterBomb->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,615 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);

        ClusterBomb->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        ClusterBomb->SetEnum(WeaponNum::ClusterBomb);
        ClusterBomb->SetClickCallBackEnum(asas);
		ButtonManager.push_back(ClusterBomb);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,615 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
	    //button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,615 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
	    //button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,615 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}

	//3��° �������̽� 

	// ���� 
	{
        Shotgun = GetLevel()->CreateActor<Button>();
        Shotgun->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,644 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
        Shotgun->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        Shotgun->SetEnum(WeaponNum::Shotgun);
        Shotgun->SetClickCallBackEnum(asas);
	
		ButtonManager.push_back(Shotgun);
	}

    //�ڵ�� 
	{
        Handgun = GetLevel()->CreateActor<Button>();
        Handgun->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,644 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
        Handgun->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        Handgun->SetEnum(WeaponNum::Handgun);
        Handgun->SetClickCallBackEnum(asas);

		ButtonManager.push_back(Handgun);
	}
    //����
	{
        Uzi = GetLevel()->CreateActor<Button>();
        Uzi->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,644 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
        Uzi->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        Uzi->SetEnum(WeaponNum::Uzi);
        Uzi->SetClickCallBackEnum(asas);

		ButtonManager.push_back(Uzi);
	}
    // �ӽŰ� 
	{
        Minigun = GetLevel()->CreateActor<Button>();
        Minigun->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,644 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
        Minigun->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        Minigun->SetEnum(WeaponNum::Minigun);
        Minigun->SetClickCallBackEnum(asas);
		ButtonManager.push_back(Minigun);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,644 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	// 4���� �������̽� 

	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,673 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(asas);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,673 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(avvv);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,673 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,673 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,673 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	// 5���� �������̽� 

	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,702 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(asas);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,702 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(avvv);
		ButtonManager.push_back(button);
	}
    // �� 
	{
        Sheep = GetLevel()->CreateActor<Button>();
        Sheep->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,702 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
        Sheep->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        Sheep->SetEnum(WeaponNum::Sheep);
        Sheep->SetClickCallBackEnum(asas);
		ButtonManager.push_back(Sheep);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,702 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,702 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}

	// 6���� �������̽� 

    //���� ��Ʈ����ũ 
	{
        AirStrike = GetLevel()->CreateActor<Button>();
        AirStrike->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,731 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
        AirStrike->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        AirStrike->SetEnum(WeaponNum::AirStrike);
        AirStrike->SetClickCallBackEnum(asas);
		ButtonManager.push_back(AirStrike);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,731 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(avvv);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,731 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,731 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,731 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}

	// 7���� �������̽� 

    //��ġ 
	{
        Torch = GetLevel()->CreateActor<Button>();
        Torch->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,760 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
        Torch->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
        Torch->SetEnum(WeaponNum::Torch);
        Torch->SetClickCallBackEnum(asas);
        ButtonManager.push_back(Torch);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,760 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(avvv);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,760 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,760 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,760 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	// 8���� �������̽� 

	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,789 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(asas);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,789 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(avvv);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,789 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,789 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,789 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	// 9���� �������̽� 
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,818 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(asas);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,818 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(avvv);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,818 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,818 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,818 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	// 10���� �������̽� 
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,847 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(asas);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,847 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(avvv);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,847 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,847 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,847 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	// 11���� �������̽� 
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,876 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(asas);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,876 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(avvv);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,876 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,876 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,876 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	// 12���� �������̽� 
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1327,905 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(asas);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1357,905 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack(avvv);
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1387,905 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1415,905 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}
	{
		Button* button = GetLevel()->CreateActor<Button>();
		button->setting("WeaponIcon.bmp", "2020.bmp", "2020.bmp", { 1444,905 }, { 26,27 }, static_cast<int>(WormsRenderOrder::WeaPonInterFace), false);
		button->SetTargetCollisionGroup(static_cast<int>(WormsCollisionOrder::WeaPonInterFace));
		//button->SetClickCallBack();
		ButtonManager.push_back(button);
	}



	{
		WaeponInterface = CreateRender(WormsRenderOrder::UI);
		WaeponInterface->SetImage("weaponSheet.Bmp");
		WaeponInterface->SetPosition({ 1370,740 });
		WaeponInterface->SetScale({ 180, 400 });
		WaeponInterface->EffectCameraOff();
		WaeponInterface->On();
	}

	{
		bazooka = CreateRender(WormsRenderOrder::WeaPonUI);
		bazooka->SetImage("bazookaUI.Bmp");
		bazooka->SetPosition({ 1327,586 });
		bazooka->SetScale({ 32, 32 });
		bazooka->EffectCameraOff();
		bazooka->On();
	}
	{
		cluster = CreateRender(WormsRenderOrder::WeaPonUI);
		cluster->SetImage("clusterUI.Bmp");
		cluster->SetPosition({ 1357,615 });
		cluster->SetScale({ 32, 32 });
		cluster->EffectCameraOff();
		cluster->On();
	}
	{
		grenade = CreateRender(WormsRenderOrder::WeaPonUI);
		grenade->SetImage("grenadeUI.Bmp");
		grenade->SetPosition({ 1327,615 });
		grenade->SetScale({ 32, 32 });
		grenade->EffectCameraOff();
		grenade->On();
	}
	{
		shotgun = CreateRender(WormsRenderOrder::WeaPonUI);
		shotgun->SetImage("shotgunUI.Bmp");
		shotgun->SetPosition({ 1327,644 });
		shotgun->SetScale({ 32, 32 });
		shotgun->EffectCameraOff();
		shotgun->On();
	}
   
    {
        HomingMissileUI = CreateRender(WormsRenderOrder::WeaPonUI);
        HomingMissileUI->SetImage("hmissileUI.Bmp");
        HomingMissileUI->SetPosition({ 1357, 586 });
        HomingMissileUI->SetScale({ 32, 32 });
        HomingMissileUI->EffectCameraOff();
        HomingMissileUI->On();
    }

    {
        HandgunUI = CreateRender(WormsRenderOrder::WeaPonUI);
        HandgunUI->SetImage("handgunUI.Bmp");
        HandgunUI->SetPosition({ 1357,644 });
        HandgunUI->SetScale({ 32, 32 });
        HandgunUI->EffectCameraOff();
        HandgunUI->On();
    }

    {
        UziUI = CreateRender(WormsRenderOrder::WeaPonUI);
        UziUI->SetImage("uziUI.Bmp");
        UziUI->SetPosition({ 1387,644 });
        UziUI->SetScale({ 32, 32 });
        UziUI->EffectCameraOff();
        UziUI->On();
    }

    {
        MinigunUI = CreateRender(WormsRenderOrder::WeaPonUI);
        MinigunUI->SetImage("minigunUI.Bmp");
        MinigunUI->SetPosition({ 1415,644 });
        MinigunUI->SetScale({ 32, 32 });
        MinigunUI->EffectCameraOff();
        MinigunUI->On();
    }

    {
        SheepUI = CreateRender(WormsRenderOrder::WeaPonUI);
        SheepUI->SetImage("sheepUI.Bmp");
        SheepUI->SetPosition({ 1387,698 });
        SheepUI->SetScale({ 32, 32 });
        SheepUI->EffectCameraOff();
        SheepUI->On();
    }
  
    {
        AirStrikeUI = CreateRender(WormsRenderOrder::WeaPonUI);
        AirStrikeUI->SetImage("airstrkeUI.Bmp");
        AirStrikeUI->SetPosition({ 1327,731 });
        AirStrikeUI->SetScale({ 28, 28 });
        AirStrikeUI->EffectCameraOff();
        AirStrikeUI->On();
    }
    {
        TorchUI = CreateRender(WormsRenderOrder::WeaPonUI);
        TorchUI->SetImage("torchUI.Bmp");
        TorchUI->SetPosition({ 1327,760 });
        TorchUI->SetScale({ 28, 28 });
        TorchUI->EffectCameraOff();
        TorchUI->On();
    }
    {
        X = CreateRender(WormsRenderOrder::WeaPonUI);
        X->SetImage("X.Bmp");
        X->SetPosition({ 1440,930 });
        X->SetScale({ 16,17 });
        X->EffectCameraOff();
        X->Off();
    }


    HomingMissileNumber.SetOwner(this);
    HomingMissileNumber.SetImage("TimerNum.bmp", { 12,20 }, static_cast<int>(WormsRenderOrder::WeaPonUI), RGB(255, 0, 255));
    HomingMissileNumber.SetRenderPos({ 1450,930 });
    HomingMissileNumber.SetCameraEffect(false);
    HomingMissileNumber.SetAlign(Align::Center);
    HomingMissileNumber.SetValue(static_cast<int>(HomingMissileCount));
    HomingMissileNumber.Off();

    GrenadeNummber.SetOwner(this);
    GrenadeNummber.SetImage("TimerNum.bmp", { 12,20 }, static_cast<int>(WormsRenderOrder::WeaPonUI), RGB(255, 0, 255));
    GrenadeNummber.SetRenderPos({ 1450,930 });
    GrenadeNummber.SetCameraEffect(false);
    GrenadeNummber.SetAlign(Align::Center);
    GrenadeNummber.SetValue(static_cast<int>(GrenadeCount));
    GrenadeNummber.Off();

   
    ClusterBombNumber.SetOwner(this);
    ClusterBombNumber.SetImage("TimerNum.bmp", { 12,20 }, static_cast<int>(WormsRenderOrder::WeaPonUI), RGB(255, 0, 255));
    ClusterBombNumber.SetRenderPos({ 1450,930 });
    ClusterBombNumber.SetCameraEffect(false);
    ClusterBombNumber.SetAlign(Align::Center);
    ClusterBombNumber.SetValue(static_cast<int>(ClusterBombCount));
    ClusterBombNumber.Off();



    ShotgunNumber.SetOwner(this);
    ShotgunNumber.SetImage("TimerNum.bmp", { 12,20 }, static_cast<int>(WormsRenderOrder::WeaPonUI), RGB(255, 0, 255));
    ShotgunNumber.SetRenderPos({ 1450,930 });
    ShotgunNumber.SetCameraEffect(false);
    ShotgunNumber.SetAlign(Align::Center);
    ShotgunNumber.SetValue(static_cast<int>(ShotgunCount));
    ShotgunNumber.Off();

    HandgunNumber.SetOwner(this);
    HandgunNumber.SetImage("TimerNum.bmp", { 12,20 }, static_cast<int>(WormsRenderOrder::WeaPonUI), RGB(255, 0, 255));
    HandgunNumber.SetRenderPos({ 1450,930 });
    HandgunNumber.SetCameraEffect(false);
    HandgunNumber.SetAlign(Align::Center);
    HandgunNumber.SetValue(static_cast<int>(HandgunCount));
    HandgunNumber.Off();


    UziNummber.SetOwner(this);
    UziNummber.SetImage("TimerNum.bmp", { 12,20 }, static_cast<int>(WormsRenderOrder::WeaPonUI), RGB(255, 0, 255));
    UziNummber.SetRenderPos({ 1450,930 });
    UziNummber.SetCameraEffect(false);
    UziNummber.SetAlign(Align::Center);
    UziNummber.SetValue(static_cast<int>(UziCountCount));
    UziNummber.Off();



    MinigunNumber.SetOwner(this);
    MinigunNumber.SetImage("TimerNum.bmp", { 12,20 }, static_cast<int>(WormsRenderOrder::WeaPonUI), RGB(255, 0, 255));
    MinigunNumber.SetRenderPos({ 1450,930 });
    MinigunNumber.SetCameraEffect(false);
    MinigunNumber.SetAlign(Align::Center);
    MinigunNumber.SetValue(static_cast<int>(MinigunCount));
    MinigunNumber.Off();

    SheepNumber.SetOwner(this);
    SheepNumber.SetImage("TimerNum.bmp", { 12,20 }, static_cast<int>(WormsRenderOrder::WeaPonUI), RGB(255, 0, 255));
    SheepNumber.SetRenderPos({ 1450,930 });
    SheepNumber.SetCameraEffect(false);
    SheepNumber.SetAlign(Align::Center);
    SheepNumber.SetValue(static_cast<int>(SheepCount));
    SheepNumber.Off();

    AirStrikeNumber.SetOwner(this);
    AirStrikeNumber.SetImage("TimerNum.bmp", { 12,20 }, static_cast<int>(WormsRenderOrder::WeaPonUI), RGB(255, 0, 255));
    AirStrikeNumber.SetRenderPos({ 1450,930 });
    AirStrikeNumber.SetCameraEffect(false);
    AirStrikeNumber.SetAlign(Align::Center);
    AirStrikeNumber.SetValue(static_cast<int>(AirStrikeCount));
    AirStrikeNumber.Off();

   TorchNumber.SetOwner(this);
   TorchNumber.SetImage("TimerNum.bmp", { 12,20 }, static_cast<int>(WormsRenderOrder::WeaPonUI), RGB(255, 0, 255));
   TorchNumber.SetRenderPos({ 1450,930 });
   TorchNumber.SetCameraEffect(false);
   TorchNumber.SetAlign(Align::Center);
   TorchNumber.SetValue(static_cast<int>(TorchCount));
   TorchNumber.Off();



	
}

void WeaponInterFace::Update(float _DeltaTime)
{
    

	if(true == GameEngineInput::IsDown("RightClock") && InterfaceMove ==true)
	{
		MoveDir = float4::Left * 300;
	}

	if (true == GameEngineInput::IsDown("RightClock") && InterfaceMove == false)
	{
		MoveDir = float4::Right * 300;
	}

   


	if (GetPos().x < -180 && InterfaceMove == true)
	{
		MoveDir = { 0,0 };
		InterfaceMove = false;
	}
	if (GetPos().x > 0 && InterfaceMove == false)
	{
		MoveDir = { 0,0 };
		InterfaceMove = true;
	}

	for (size_t i = 0; i < ButtonManager.size(); i++)
	{
		ButtonManager[i]->SetMove(MoveDir * _DeltaTime);
	}

    NumberManager();
	SetMove(MoveDir*_DeltaTime);

}

void WeaponInterFace::NumberManager()
{
    X->Off();
    HomingMissileNumber.Off();
    GrenadeNummber.Off();
    ClusterBombNumber.Off();
    ShotgunNumber.Off();
    HandgunNumber.Off();
    UziNummber.Off();
    MinigunNumber.Off();
    SheepNumber.Off();
    AirStrikeNumber.Off();
    TorchNumber.Off();



    // ȣ�ֹ̻���
    if (HomingMissile->GetHover() == true)
    {
        X->On();
        HomingMissileNumber.On();
    }
    if (HomingMissile->GetTest() == true && GameEngineInput::IsDown("LeftClock"))
    {
        --HomingMissileCount;
    }
   
    if (HomingMissileCount == 0)
    {
        HomingMissileUI->Off();
        HomingMissile->Off();
    }
    if (HomingMissileCount > 0)
    {
        HomingMissileUI->On();
        HomingMissile->On();
    }

    // �ʷ���ź
    if (Grenade->GetHover() == true)
    {
        X->On();
        GrenadeNummber.On();
    }
    if (Grenade->GetTest() == true && GameEngineInput::IsDown("LeftClock"))
    {
        --GrenadeCount;
    }

    if (GrenadeCount <= 0)
    {
        grenade->Off();
        Grenade->Off();
    }
    if (GrenadeCount > 0)
    {
        grenade->On();
        Grenade->On();
    }

    // ������ź

    if (ClusterBomb->GetHover() == true)
    {
        X->On();
        ClusterBombNumber.On();
    }
    if (ClusterBomb->GetTest() == true && GameEngineInput::IsDown("LeftClock"))
    {
        --ClusterBombCount;
    }

    if (ClusterBombCount == 0)
    {
        cluster->Off();
        ClusterBomb->Off();
    }
    if (ClusterBombCount > 0)
    {
        cluster->On();
        ClusterBomb->On();
    }

    // ���� 

    if (Shotgun->GetHover() == true)
    {
        X->On();
        ShotgunNumber.On();
    }
    if (Shotgun->GetTest() == true && GameEngineInput::IsDown("LeftClock"))
    {
        --ShotgunCount;
    }

    if (ShotgunCount == 0)
    {
        shotgun->Off();
        Shotgun->Off();
    }
    if (ShotgunCount > 0)
    {
        shotgun->On();
        Shotgun->On();
    }

    // �ڵ�� 

    if (Handgun->GetHover() == true)
    {
        X->On();
        HandgunNumber.On();
    }
    if (Handgun->GetTest() == true && GameEngineInput::IsDown("LeftClock"))
    {
        --HandgunCount;
    }

    if (HandgunCount == 0)
    {
        HandgunUI->Off();
        Handgun->Off();
    }
    if (HandgunCount > 0)
    {
        HandgunUI->On();
        Handgun->On();
    }

    // ���� 

    if (Uzi->GetHover() == true)
    {
        X->On();
        UziNummber.On();
    }
    if (Uzi->GetTest() == true && GameEngineInput::IsDown("LeftClock"))
    {
        --UziCountCount;
    }

    if (UziCountCount == 0)
    {
        UziUI->Off();
        Uzi->Off();
    }
    if (UziCountCount > 0)
    {
        UziUI->On();
        Uzi->On();
    }

    // �̴ϰ� 

    if (Minigun->GetHover() == true)
    {
        X->On();
        MinigunNumber.On();
    }
    if (Minigun->GetTest() == true && GameEngineInput::IsDown("LeftClock"))
    {
        --MinigunCount;
    }

    if (MinigunCount == 0)
    {
        MinigunUI->Off();
        Minigun->Off();
    }
    if (MinigunCount > 0)
    {
        MinigunUI->On();
        Minigun->On();
    }

    // �� 

    if (Sheep->GetHover() == true)
    {
        X->On();
        SheepNumber.On();
    }
    if (Sheep->GetTest() == true && GameEngineInput::IsDown("LeftClock"))
    {
        --SheepCount;
    }

    if (SheepCount == 0)
    {
        SheepUI->Off();
        Sheep->Off();
    }
    if (SheepCount > 0)
    {
        SheepUI->On();
        Sheep->On();
    }

    // ���� ��Ʈ����ũ 


    if (AirStrike->GetHover() == true)
    {
        X->On();
        AirStrikeNumber.On();
    }
    if (AirStrike->GetTest() == true && GameEngineInput::IsDown("LeftClock"))
    {
        --AirStrikeCount;
    }

    if (AirStrikeCount == 0)
    {
        AirStrikeUI->Off();
        AirStrike->Off();
    }
    if (AirStrikeCount > 0)
    {
        AirStrikeUI->On();
        AirStrike->On();
    }






    if (HomingMissileCount >= 0)
    {
        HomingMissileNumber.SetValue(static_cast<int>(HomingMissileCount));
    }
    if (GrenadeCount >= 0)
    {
        GrenadeNummber.SetValue(static_cast<int>(GrenadeCount));
    }
    if (ClusterBombCount >= 0)
    {
        ClusterBombNumber.SetValue(static_cast<int>(ClusterBombCount));
    }
    if (ShotgunCount > 0)
    {
        ShotgunNumber.SetValue(static_cast<int>(ShotgunCount));
    }
    if (HandgunCount > 0)
    {
        HandgunNumber.SetValue(static_cast<int>(HandgunCount));
    }
    if (UziCountCount > 0)
    {
        UziNummber.SetValue(static_cast<int>(UziCountCount));
    }

    if (MinigunCount > 0)
    {
        MinigunNumber.SetValue(static_cast<int>(MinigunCount));
    }

    if (SheepCount > 0)
    {
        SheepNumber.SetValue(static_cast<int>(SheepCount));
    }

    if (AirStrikeCount > 0)
    {
        AirStrikeNumber.SetValue(static_cast<int>(AirStrikeCount));
    }
   /* if (TorchCount > 0)
    {
        TorchNumber.SetValue(static_cast<int>(TorchCount));
    }*/



}

void WeaponInterFace::Render(float _Time)
{
	
}

