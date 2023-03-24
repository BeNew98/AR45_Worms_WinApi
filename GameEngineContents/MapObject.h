#pragma once
//#include <vector>
#include <GameEngineCore/GameEngineActor.h>

// ���� : �ʿ� ��ġ�� ������Ʈ�� ���õ� ���
class MapObject : public GameEngineActor
{
public:
	// constrcuter destructer
	MapObject();
	~MapObject();

	// delete Function
	MapObject(const MapObject& _Other) = delete;
	MapObject(MapObject&& _Other) noexcept = delete;
	MapObject& operator=(const MapObject& _Other) = delete;
	MapObject& operator=(MapObject&& _Other) noexcept = delete;

protected:
    void Start() override;
    void Update(float _DeltaTime) override;

private:
    int Blue = RGB(0, 0, 255);
    int Magenta = RGB(255, 0, 255);

    float4 ObjScale = float4::Zero;              //������Ʈ���� �������� �����ϴ� �뵵
    float4 MapScale = { 3840.0f, 1392.0f };      //����ϴ� ���� ũ��


    GameEngineRender* TestRender = nullptr;

    std::vector<float4> GetRandomPos();
    float4 SetObjPosRand();
    void MergeMap();                             //���� �ʰ� �浹�ʿ� ������Ʈ�̹����� �ռ��ϴ� �Լ�

};

