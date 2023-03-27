#include "Player.h"
#include "Weapon.h"
#include "ContentsEnums.h"
#include <GameEnginePlatform/GameEngineInput.h>
void Player::EquipWeaponStart()
{
    if (nullptr == CurWeapon || CurWeapon->GetWeaponNumber() == static_cast<int>(WeaponNum::None))
    {
        return;
    }
    std::string _Name = CurWeapon->GetWeaponName() + "On";
    DirCheck(_Name);
    //switch (WeaponNumber)
    //{
    //case static_cast<int>(WeaponNum::Uzi):
    //{
    //    DirCheck("UziOn");
    //    break;
    //}
    //case static_cast<int>(WeaponNum::Grenade):
    //{
    //    DirCheck("GrenadeOn");
    //    break;
    //}
    //case static_cast<int>(WeaponNum::ClusterBomb):
    //{
    //    DirCheck("ClusterBombOn"); // ������ ������Ʈ �ʿ�
    //    break;
    //}
    //default:
    //    break;
    //}
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
    if (true == CurWeapon->IsWeaponDone())
    {
        ChangeState(PlayerState::IDLE);
        return;
    }


}
void Player::EquipWeaponEnd()
{
    std::string _Name = CurWeapon->GetWeaponName() + "Off";
    DirCheck(_Name);
    //int WeaponNumber = CurWeapon->GetWeaponNumber();

    //switch (WeaponNumber)
    //{
    //case static_cast<int>(WeaponNum::Uzi):
    //{
    //    DirCheck("UziOff"); // ������ ������Ʈ �ʿ�
    //    break;
    //}
    //case static_cast<int>(WeaponNum::Grenade):
    //{
    //    DirCheck("GrenadeOff"); // ������ ������Ʈ �ʿ�
    //    break;
    //}
    //case static_cast<int>(WeaponNum::ClusterBomb):
    //{
    //    DirCheck("ClusterBombOff"); // ������ ������Ʈ �ʿ�
    //    break;
    //}
    //default:
    //    break;
    //}
    //����� ���� �ִϸ��̼ǿ� ���� ����
}