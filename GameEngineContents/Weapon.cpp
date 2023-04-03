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

	if (GameEngineInput::IsPress("WeaponUp") && -5.7 < Height)
	{
		Height -= 0.05f;
	}
	else if (GameEngineInput::IsPress("WeaponDown") && 5.7 > Height)
	{
		Height += 0.05f;
	}

	if (nullptr == CurPlayer)
	{
		MsgAssert("���� �÷��̾ ã�� �� �����ϴ�.");
	}

	float4 PlayerDir = CurPlayer->GetPlayerDir();

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

bool Weapon::DownShoot()
{
    if (GameEngineInput::IsDown("Shoot")) 
    {
        return true;
    }
    return false;
}

bool Weapon::PressShoot()
{
    if (GameEngineInput::IsPress("Shoot")) 
    {
        return true;
    }
	return false;
}

bool Weapon::UpShoot()
{
    if (GameEngineInput::IsUp("Shoot"))
    {
        return true;
    }
    return false;
}

bool Weapon::isEndCharging()
{
	if (GameEngineInput::IsUp("Shoot"))
	{
		return true;
	}

	if (true)
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
		MsgAssert("CurPlayer�� nullptr�Դϴ�.");
	}
    // �ӽ÷� PlyaerState üũ�ϱ� ���� ���� -> ���� ����� �������ִ� ����
    if (nullptr == CurPlayer->GetCurWeapon())
    {
        CurPlayer->SetCurWeapon(this);
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
	else if (RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos(), RGB(0, 0, 255)) && _Col->GetActorPlusPos().y > 0 && _Col->GetActorPlusPos().x > 0) //�ʿ� ������ �����
	{
		return true;
	}
	// �� ������ �������� üũ
    float4 _Pos = _Col->GetPosition();
    if (!(-640 <= _Pos.x && _Pos.x < 4480 && -743 <= _Pos.y && _Pos.y < 1310))
    {
        return true;
    }

	return false;
}

float Weapon::GetChargeTime()
{
	return GameEngineInput::GetPressTime("Shoot");
}

float4 Weapon::CheckCollisionSide(GameEngineCollision* _Col)
{
	float4 ReturnValue = float4::Zero;

    // �� ������ �������� üũ
    float4 _Pos = _Col->GetActorPlusPos(); // �� Ȯ�� �ʿ���.
    if (!(-640 <= _Pos.x && _Pos.x < 4480 && -743 <= _Pos.y && _Pos.y < 1310))
    {
        return ReturnValue;
    }


	std::vector<GameEngineCollision*> CollisionList;
    float CheckScale = _Col->GetScale().hx();

	if (true == _Col->Collision({ .TargetGroup = static_cast<int>(WormsCollisionOrder::Player), .TargetColType = CollisionType::CT_CirCle, .ThisColType = CollisionType::CT_CirCle }, CollisionList))
	{
		for (int i = 0; i < CollisionList.size(); i++)
		{
			Player* ColPlayer = dynamic_cast<Player*>(CollisionList[i]->GetActor());
            if (CurPlayer == ColPlayer)
            {
                continue;
            }
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



	// 30 30 20,40
    if(RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos() + float4{ CheckScale }, RGB(255, 0, 255)) &&
        RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos() + float4{ -CheckScale }, RGB(255, 0, 255)) &&
        RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos() + float4{ 0,CheckScale }, RGB(255, 0, 255)))
    {
        return float4::Up;
    }

    if (RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos() + float4{ CheckScale }, RGB(255, 0, 255)))
	{
		ReturnValue.x += 1;
	}
	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos() + float4{ -CheckScale }, RGB(255, 0, 255)))
	{
		ReturnValue.x -= 1;
	}
	if (RGB(0, 0, 255) == MapCollision->GetPixelColor(_Col->GetActorPlusPos() + float4{ 0,CheckScale }, RGB(255, 0, 255)))
	{
		ReturnValue.y += 1;
	}

	return ReturnValue;
}


void Weapon::AttackPlayer(GameEngineCollision* _Col, bool _AttackSelf) // �� Ȯ�� �ʿ���
{                   // ���� CollisionScale���� �ʿ�                                   :_Col->SetScale({ static_cast<float>(BombScale * 2) });
                    // _Col->GetActorPlusPos()�� ��Ȯ�� ���� ��ġ�� �ǰ� ���� �ʿ�

	if (nullptr == _Col)
	{
		MsgAssert("üũ�� �ݸ����� �����ϴ�.");
	}

	// �÷��̾� üũ
	std::vector<GameEngineCollision*> CollisionList;

    int Radius = BombScale / 2;

	if (_Col != nullptr && true == _Col->Collision({ .TargetGroup = static_cast<int>(WormsCollisionOrder::Player), .TargetColType = CollisionType::CT_CirCle, .ThisColType = CollisionType::CT_CirCle }, CollisionList))
	{
		for (int i = 0; i < CollisionList.size(); i++)
		{
			Player* ColPlayer = dynamic_cast<Player*>(CollisionList[i]->GetActor());
            if (_AttackSelf == false && ColPlayer == CurPlayer)
            {
                continue;
            }
            float4 Distance = ColPlayer->GetPos() - _Col->GetActorPlusPos(); //���� ����

            float Ratio = Distance.Size() / Radius > 1 ? 1 : Distance.Size() / Radius;

            int proportional_dmg = static_cast<int>(MaxDmg * (1 - Distance.Size() / Radius) + MinDmg * (Distance.Size() / Radius));
            float proportional_power = MaxKnockBackPower * (1 - Distance.Size() / Radius) + MinKnockBackPower * (Distance.Size() / Radius);
            Distance.Normalize();

            //              �Ÿ� ��ʵ�����,    ���󰡴� ����,   �Ÿ� ��� ���󰡴� ���� 
            ColPlayer->Damaged(proportional_dmg, Distance, proportional_power);
		    //���⼭ Dmg �� �ִ� ������, KnockBackPower�� �ִ� �˹� �Ŀ��� �̾߱���
        }
	}
}


// Gun�� ����� ���� ��ġ�� �ƴ� �÷��̾ �߻��� �Ÿ� �ݺ�� �������̹Ƿ�
void Weapon::AttackPlayerGun(GameEngineCollision* _Col, float _refDistance)
{                   // ���� CollisionScale���� �ʿ�
                    // _Col->GetActorPlusPos()�� ��Ȯ�� ���� ��ġ�� �ǰ� ���� �ʿ�

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
            //if (ColPlayer == CurPlayer) // Gun�� ���ο��� �������� ���� ����?? �ִ���?
            //{
            //    continue;
            //}
            float4 Distance = _Col->GetActorPlusPos() - GetPos(); //���� ����

            float Ratio = Distance.Size() / _refDistance > 1 ? 1 : Distance.Size() / _refDistance; // ���� ���� �̻����� �Ѿ�� �� ����


            int proportional_dmg = static_cast<int>(MaxDmg * (1 - Ratio) + MinDmg * Ratio); // �������� ���� ������
            float proportional_power = MaxKnockBackPower * (1 - Ratio) + MinKnockBackPower * Ratio;
            Distance.Normalize();

            //              �Ÿ� ��ʵ�����,    ���󰡴� ����,   �Ÿ� ��� ���󰡴� ���� 
            ColPlayer->Damaged(proportional_dmg, Distance, proportional_power);
            //���⼭ Dmg �� �ִ� ������, KnockBackPower�� �ִ� �˹� �Ŀ��� �̾߱���
        }
    }
}


void Weapon::ExplosionEffectInit(float _CircleDiameter)
{
    //����Ʈ
    std::string CircleImageName = "circle25.bmp";
    std::string ElipseImageName = "Elipse50.bmp";

    int CircleFrame = 7;
    int ElipseeFrame = 19;

    //ũ�⿡ ���� ��ȭ
    if (_CircleDiameter >= 100)
    {
        CircleImageName = "circle50.bmp";
        CircleFrame = 8;

        ElipseImageName = "Elipse75.bmp";
        ElipseeFrame = 9;
    }
    else if (_CircleDiameter >= 150)
    {
        CircleImageName = "circle75.bmp";
        CircleFrame = 3;

        ElipseImageName = "Elipse100.bmp";
        ElipseeFrame = 9;
    }
    else if (_CircleDiameter >= 200)
    {
        CircleImageName = "circle100.bmp";
        CircleFrame = 3;

        ElipseImageName = "Elipse100.bmp";
        ElipseeFrame = 9;
    }

    ExplosionCircle = CreateRender(CircleImageName, WormsRenderOrder::Weapon);
    ExplosionCircle->CreateAnimation({ .AnimationName = "Explosion", .ImageName = CircleImageName, .Start = 0, .End = CircleFrame, .InterTime = 0.05f , .Loop = false });
    ExplosionCircle->CreateAnimation({ .AnimationName = "Idle", .ImageName = CircleImageName, .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    ExplosionCircle->SetScale({ _CircleDiameter , _CircleDiameter });

    ExplosionCircle->ChangeAnimation("Idle");
    ExplosionCircle->Off();

    ExplosionElipse = CreateRender(ElipseImageName, WormsRenderOrder::Weapon);
    ExplosionElipse->CreateAnimation({ .AnimationName = "ExplosionElipse", .ImageName = ElipseImageName, .Start = 0, .End = ElipseeFrame, .InterTime = 0.03f , .Loop = false });
    ExplosionElipse->CreateAnimation({ .AnimationName = "Idle", .ImageName = ElipseImageName, .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    ExplosionElipse->SetScale({ _CircleDiameter * 1.5f , _CircleDiameter * 1.5f });

    ExplosionElipse->ChangeAnimation("Idle");
    ExplosionElipse->Off();

    PootTextAnimation = CreateRender("Poot.bmp", WormsRenderOrder::Weapon);
    PootTextAnimation->CreateAnimation({ .AnimationName = "Poot", .ImageName = "Poot.bmp", .Start = 0, .End = 17, .InterTime = 0.02f , .Loop = false });
    PootTextAnimation->CreateAnimation({ .AnimationName = "Idle", .ImageName = "Poot.bmp", .Start = 0, .End = 1, .InterTime = 0.05f , .Loop = false });
    PootTextAnimation->SetScale({ 70 , 70 });

    PootTextAnimation->ChangeAnimation("Idle");
    PootTextAnimation->Off();

}