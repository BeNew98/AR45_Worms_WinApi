#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/NumberRenderObject.h>

class PlayerHPUI : public GameEngineActor
{
public:
	// constrcuter destructer
	PlayerHPUI();
	~PlayerHPUI();

	// delete Function
	PlayerHPUI(const PlayerHPUI& _Other) = delete;
	PlayerHPUI(PlayerHPUI&& _Other) noexcept = delete;
	PlayerHPUI& operator=(const PlayerHPUI& _Other) = delete;
	PlayerHPUI& operator=(PlayerHPUI&& _Other) noexcept = delete;

	void SetPlayerHPUI();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	NumberRenderObject PlayerHPNumberRender;
	GameEngineRender* HPBarImageRender = nullptr; // ü�� â �� �̹��� ����
	GameEngineRender* SelectPlayerRender = nullptr; // ȭ��ǥ �̹��� ����

};

