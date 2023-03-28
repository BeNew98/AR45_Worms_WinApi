#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <vector>

// ���� :
class GameEngineRender;
class AllPlayerHpUI : public GameEngineActor
{
public:
    static AllPlayerHpUI* AllHpUI;
	// constrcuter destructer
	AllPlayerHpUI();
	~AllPlayerHpUI();

	// delete Function
	AllPlayerHpUI(const AllPlayerHpUI& _Other) = delete;
	AllPlayerHpUI(AllPlayerHpUI&& _Other) noexcept = delete;
	AllPlayerHpUI& operator=(const AllPlayerHpUI& _Other) = delete;
	AllPlayerHpUI& operator=(AllPlayerHpUI&& _Other) noexcept = delete;

    void SetAllPlayerHP();

protected:
    void Start() override;
    void Update(float _DeltaTime) override;


    // ���� �ʿ�. �������Ϳ� HP���͸� ������ HP�񱳸� ���� HP������ ������ �����ϸ� �������� ������ �����ϰ� �����ؼ�
    // ������ ���� ��ġ���� �����ؾ���. ��ġ���� �ʱ������� y�� 850.f �� �ΰ� �ݺ��������� 17.f�� ����
    // Scale�� �پ��°� ���� ���������� �پ�����Ѵ�(?)

private:
    std::vector<int> vecMixNum;
    std::vector<GameEngineRender*> vecPlayerHpBar;
    std::vector<GameEngineRender*> vecPlayerName;
    std::vector<int> vecPlayerCurHp;
    std::vector<float4> vecLastPos;
    float4 StartPos = { 640.f,850.f };

    float fLerpRatio = 0.f;

    bool bSetHP = false;
    bool bSort = false;

};

