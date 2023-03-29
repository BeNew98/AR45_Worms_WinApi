#pragma once
//#include <vector>
#include <GameEngineCore/GameEngineActor.h>

// ���� : �ʿ� ��ġ�� ������Ʈ�� ���õ� ���
class GameEngineImage;
class MapObject : public GameEngineActor
{
public:
	// constrcuter destructer
	MapObject();
	virtual ~MapObject() = 0;

	// delete Function
	MapObject(const MapObject& _Other) = delete;
	MapObject(MapObject&& _Other) noexcept = delete;
	MapObject& operator=(const MapObject& _Other) = delete;
	MapObject& operator=(MapObject&& _Other) noexcept = delete;

    float4 GetMapObjPos();

    void SetColImage();

protected:
    int Blue = RGB(0, 0, 255);
    int Magenta = RGB(255, 0, 255);

    float MoveSpeed = 50.0f;                                //�ӷ�
    float Gravity = 500.0f;                                 //�߷� ũ��

    float4 ObjScale = { 60.0f, 60.0f };
    float4 MoveDir = float4::Zero;                          //������Ʈ �̵��� ����

    GameEngineRender* MapObjRender = nullptr;               //object Render
    GameEngineCollision* MapObjCol = nullptr;				//Map Object Collision

    GameEngineImage* ColImage = nullptr;

   

    void GravityApplied(float _DeltaTime);                  //�߷�����
    float4 PullUp(float4 _NextPos, float _DeltaTime);       //�߷¿� ���� �ϰ��ϴ� ��ġ���� ������ ����ø�
    bool IsGroundCheck();                                   //���� �ִ� ��ġ�� ������ Ȯ���ϴ� �Լ�
    bool IsUnderWaterCheck();
private:
   
};

