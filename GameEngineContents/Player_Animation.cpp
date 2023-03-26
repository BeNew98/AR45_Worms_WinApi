#include "Player.h"

void Player::CreateAnimation()
{
    //�ִϸ��̼�
    {
        //����
        AnimationRender->CreateAnimation({ .AnimationName = "Left_Idle", .ImageName = "IdleLeft.bmp", .Start = 0, .End = 5, .InterTime = 0.1f });
        AnimationRender->CreateAnimation({ .AnimationName = "Left_Move", .ImageName = "WalkLeft.bmp", .Start = 0, .End = 14, .InterTime = 0.03f });
        AnimationRender->CreateAnimation({ .AnimationName = "Left_JumpReady", .ImageName = "JumpReadyLeft.bmp", .Start = 0, .End = 9, .InterTime = 0.03f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Left_FlyUp", .ImageName = "FlyupLeft.bmp", .Start = 0, .End = 1, .InterTime = 0.1f });
        AnimationRender->CreateAnimation({ .AnimationName = "Left_FlyDown", .ImageName = "FlydownLeft.bmp", .Start = 0, .End = 1, .InterTime = 0.1f });
        AnimationRender->CreateAnimation({ .AnimationName = "Left_Die", .ImageName = "DieLeft.bmp", .Start = 0, .End = 59, .InterTime = 0.05f });
        AnimationRender->CreateAnimation({ .AnimationName = "Left_Win", .ImageName = "WinLeft.bmp", .Start = 0, .End = 13, .InterTime = 0.05f });
    }
    {
        //����
        AnimationRender->CreateAnimation({ .AnimationName = "Right_Idle", .ImageName = "IdleRight.bmp", .Start = 0, .End = 5, .InterTime = 0.1f });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_Move", .ImageName = "WalkRight.bmp", .Start = 0, .End = 14, .InterTime = 0.03f });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_JumpReady", .ImageName = "JumpReadyRight.bmp", .Start = 0, .End = 9, .InterTime = 0.03f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_FlyUp", .ImageName = "FlyupRight.bmp", .Start = 0, .End = 1, .InterTime = 0.1f });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_FlyDown", .ImageName = "FlydownRight.bmp", .Start = 0, .End = 1, .InterTime = 0.1f });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_Die", .ImageName = "DieRight.bmp", .Start = 0, .End = 59, .InterTime = 0.05f });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_Win", .ImageName = "WinRight.bmp", .Start = 0, .End = 13, .InterTime = 0.05f });
    }


    //����ī                            
    {  
        AnimationRender->CreateAnimation({ .AnimationName = "Left_bazAim", .ImageName = "bazAimLeft.bmp",.Start = 0, .End = 31, .InterTime = 1000.0f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_bazAim", .ImageName = "bazAimRight.bmp",.Start = 0, .End = 31, .InterTime = 1000.0f, .Loop = false });

        AnimationRender->CreateAnimation({ .AnimationName = "Left_bazOff", .ImageName = "bazOffLeft.bmp",.Start = 0, .End = 6, .InterTime = 0.05f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_bazOff", .ImageName = "bazOffRight.bmp", .Start = 0, .End = 6, .InterTime = 0.05f, .Loop = false });

        AnimationRender->CreateAnimation({ .AnimationName = "Left_bazOn", .ImageName = "bazOnLeft.bmp", .Start = 0, .End = 7, .InterTime = 0.05f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_bazOn", .ImageName = "bazOnRight.bmp", .Start = 0, .End = 7, .InterTime = 0.05f, .Loop = false });
    }

    //��
    {
        AnimationRender->CreateAnimation({ .AnimationName = "Left_SheepOff", .ImageName = "SheepOffLeft.bmp",.Start = 0, .End = 9, .InterTime = 0.025f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_SheepOff", .ImageName = "SheepOffRight.bmp",.Start = 0, .End = 9, .InterTime = 0.025f, .Loop = false });

        AnimationRender->CreateAnimation({ .AnimationName = "Left_SheepOn", .ImageName = "SheepOnLeft.bmp",.Start = 0, .End = 9, .InterTime = 0.025f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_SheepOn", .ImageName = "SheepOnRight.bmp", .Start = 0, .End = 9, .InterTime = 0.025f, .Loop = false });
    }

    //���Ʈ����ũ
    {
        AnimationRender->CreateAnimation({ .AnimationName = "Left_AirStrikeOff", .ImageName = "AirStrikeOffLeft.bmp",.Start = 0, .End = 9, .InterTime = 0.1f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_AirStrikeOff", .ImageName = "AirStrikeOffRight.bmp",.Start = 0, .End = 9, .InterTime = 0.1f, .Loop = false });

        AnimationRender->CreateAnimation({ .AnimationName = "Left_AirStrikeOn", .ImageName = "AirStrikeOnLeft.bmp",.Start = 0, .End = 9, .InterTime = 0.05f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_AirStrikeOn", .ImageName = "AirStrikeOnRight.bmp", .Start = 0, .End = 9, .InterTime = 0.05f, .Loop = false });
    }

    //ȣ�ֹ̻���
    {
        AnimationRender->CreateAnimation({ .AnimationName = "Left_HomingAim", .ImageName = "HomingAimLeft.bmp",.Start = 0, .End = 31, .InterTime = 1000.0f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_HomingAim", .ImageName = "HomingAimRight.bmp",.Start = 0, .End = 31, .InterTime = 1000.0f, .Loop = false });

        AnimationRender->CreateAnimation({ .AnimationName = "Left_HomingOff", .ImageName = "HomingOffLeft.bmp",.Start = 0, .End = 6, .InterTime = 0.1f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_HomingOff", .ImageName = "HomingOffRight.bmp",.Start = 0, .End = 6, .InterTime = 0.1f, .Loop = false });

        AnimationRender->CreateAnimation({ .AnimationName = "Left_HomingOn", .ImageName = "HomingOnLeft.bmp",.Start = 0, .End = 6, .InterTime = 0.05f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_HomingOn", .ImageName = "HomingOnRight.bmp", .Start = 0, .End = 6, .InterTime = 0.05f, .Loop = false });
    }

    //�׷����̵� ( ����ź )
    {
        AnimationRender->CreateAnimation({ .AnimationName = "Left_GrenadeAim", .ImageName = "GrenadeAimLeft.bmp",.Start = 0, .End = 31, .InterTime = 0.1f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_GrenadeAim", .ImageName = "GrenadeAimRight.bmp",.Start = 0, .End = 31, .InterTime = 0.1f, .Loop = false });

        AnimationRender->CreateAnimation({ .AnimationName = "Left_GrenadeOff", .ImageName = "GrenadeOffLeft.bmp",.Start = 0, .End = 9, .InterTime = 0.1f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_GrenadeOff", .ImageName = "GrenadeOffRight.bmp",.Start = 0, .End = 9, .InterTime = 0.1f, .Loop = false });

        AnimationRender->CreateAnimation({ .AnimationName = "Left_GrenadeOn", .ImageName = "GrenadeOnLeft.bmp",.Start = 0, .End = 9, .InterTime = 0.05f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_GrenadeOn", .ImageName = "GrenadeOnRight.bmp", .Start = 0, .End = 9, .InterTime = 0.05f, .Loop = false });
    }

    //��ġ
    {
        AnimationRender->CreateAnimation({ .AnimationName = "Left_TorchFire", .ImageName = "TorchFireLeft.bmp",.Start = 0, .End = 14, .InterTime = 0.1f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_TorchFire", .ImageName = "TorchFireRight.bmp",.Start = 0, .End =14, .InterTime = 0.1f, .Loop = false });

        AnimationRender->CreateAnimation({ .AnimationName = "Left_TorchOff", .ImageName = "TorchOffLeft.bmp",.Start = 0, .End = 14, .InterTime = 0.1f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_TorchOff", .ImageName = "TorchOffRight.bmp",.Start = 0, .End = 14, .InterTime = 0.1f, .Loop = false });

        AnimationRender->CreateAnimation({ .AnimationName = "Left_TorchOn", .ImageName = "TorchOnLeft.bmp",.Start = 0, .End = 14, .InterTime = 0.05f, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_TorchOn", .ImageName = "TorchOnRight.bmp", .Start = 0, .End = 14, .InterTime = 0.05f, .Loop = false });
    }

    //����
    {
        AnimationRender->CreateAnimation({.AnimationName = "Left_UziFire", .ImageName = "UziFireLeft.bmp",.Start = 0, .End = 31, .InterTime = 0.1f, .Loop = false });
        AnimationRender->CreateAnimation({.AnimationName = "Right_UziFire", .ImageName = "UziFireRight.bmp",.Start = 0, .End = 31, .InterTime = 0.1f, .Loop = false });
       
        AnimationRender->CreateAnimation({ .AnimationName = "Left_UziAim", .ImageName = "UziAimLeft.bmp",.Start = 0, .End = 31, .InterTime = 1000, .Loop = false });
        AnimationRender->CreateAnimation({ .AnimationName = "Right_UziAim", .ImageName = "UziAimRight.bmp",.Start = 0, .End = 31, .InterTime = 1000, .Loop = false });
       
        AnimationRender->CreateAnimation({.AnimationName = "Left_UziOff", .ImageName = "UziOffLeft.bmp",.Start = 0, .End = 9, .InterTime = 0.1f, .Loop = false });
        AnimationRender->CreateAnimation({.AnimationName = "Right_UziOff", .ImageName = "UziOffRight.bmp",.Start = 0, .End = 9, .InterTime = 0.1f, .Loop = false });

        AnimationRender->CreateAnimation({.AnimationName = "Left_UziOn", .ImageName = "UziOnLeft.bmp",.Start = 0, .End = 9, .InterTime = 0.05f, .Loop = false });
        AnimationRender->CreateAnimation({.AnimationName = "Right_UziOn", .ImageName = "UziOnRight.bmp", .Start = 0, .End = 9, .InterTime = 0.05f, .Loop = false });
    }
}