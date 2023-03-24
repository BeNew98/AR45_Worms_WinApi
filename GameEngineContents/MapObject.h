#pragma once
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
    GameEngineRender* MapObjRender = nullptr;
};

