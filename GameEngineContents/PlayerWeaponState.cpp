#include "Player.h"
#include "Weapon.h"
#include "ContentsEnums.h"
#include <GameEnginePlatform/GameEngineInput.h>
void Player::EquipWeaponStart()
{
    int WeaponNumber = CurWeapon->GetWeaponNumber();

    switch (WeaponNumber)
    {
    case static_cast<int>(WeaponNum::Uzi):
    {
        DirCheck("UziOn");
        break;
    }
    default:
        break;
    }
    //���� ���� �ִϸ��̼����� ����
}
void Player::EquipWeaponUpdate(float _DeltatTime)
{

    { // ������ üũ
        if (GameEngineInput::IsPress("MoveLeft") && GameEngineInput::IsPress("MoveRight"))
        {
            return;
        }

        if (true == IsMyTurn && true == CanIMove)
        {
            if (true == GameEngineInput::IsDown("MoveLeft"))
            {
                if (true == ReturnCanIMove(PlayerAngleDir::Left))
                {
                    ChangeState(PlayerState::MOVE);
                    return;
                }
            }

            if (true == GameEngineInput::IsDown("MoveRight"))
            {
                if (true == ReturnCanIMove(PlayerAngleDir::Right))
                {
                    ChangeState(PlayerState::MOVE);
                    return;
                }
            }

            if (true == GameEngineInput::IsDown("Jump"))
            {
                ChangeState(PlayerState::JUMP);
                return;
            }
        }
    }



}
void Player::EquipWeaponEnd()
{
    int WeaponNumber = CurWeapon->GetWeaponNumber();

    switch (WeaponNumber)
    {
    case static_cast<int>(WeaponNum::Uzi):
    {
        DirCheck("UziOff"); // ������ ������Ʈ �ʿ�
        break;
    }
    default:
        break;
    }
    //����� ���� �ִϸ��̼ǿ� ���� ����
}