#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� : �ʿ� ��ġ�� ��ĵ�� ���õ� ���
class MapDecoration : public GameEngineActor
{
public:
	// constrcuter destructer
	MapDecoration();
	~MapDecoration();

	// delete Function
	MapDecoration(const MapDecoration& _Other) = delete;
	MapDecoration(MapDecoration&& _Other) noexcept = delete;
	MapDecoration& operator=(const MapDecoration& _Other) = delete;
	MapDecoration& operator=(MapDecoration&& _Other) noexcept = delete;

    float4 GetRandomPos();
    void MergeMap();                             //���� �ʰ� �浹�ʿ� ������Ʈ�̹����� �ռ��ϴ� �Լ�

protected:
    void Start() override;
    void Update(float _DeltaTime) override;

private:
    int Blue = RGB(0, 0, 255);
    int Magenta = RGB(255, 0, 255);

    int RandIdx = 0;

    float4 DecoScale = float4::Zero;              //�ٹ̱�� ������Ʈ���� �������� �����ϴ� �뵵

    std::vector<float4> RandPosSets = std::vector<float4>();

    std::vector<std::string> Objects = std::vector<std::string>();
    std::vector<std::string> ColObjects = std::vector<std::string>();

    void MakeRandomPosSets();                    //������ ��ġ�� üũ�ؼ� ���Ϳ� �ִ� �Լ�
};