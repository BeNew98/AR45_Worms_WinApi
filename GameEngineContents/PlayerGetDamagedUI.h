#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/NumberRenderObject.h>

// ���� :
class PlayerGetDamagedUI : public GameEngineActor
{
public:
	// constrcuter destructer
	PlayerGetDamagedUI();
	~PlayerGetDamagedUI();

	// delete Function
	PlayerGetDamagedUI(const PlayerGetDamagedUI& _Other) = delete;
	PlayerGetDamagedUI(PlayerGetDamagedUI&& _Other) noexcept = delete;
	PlayerGetDamagedUI& operator=(const PlayerGetDamagedUI& _Other) = delete;
	PlayerGetDamagedUI& operator=(PlayerGetDamagedUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	
private:
	NumberRenderObject HpNumberRender; // ���� ü���� ������ �ѹ� ����
	GameEngineRender* HPImageRender = nullptr; // ���� ü�� �� ��׶��� �̹��� ����
};

