#pragma once
#include <string>
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class Map : public GameEngineActor
{
public:
	static Map* MainMap;
public:
	// constrcuter destructer
	Map();
	~Map();

	// delete Function
	Map(const Map& _Other) = delete;
	Map(Map&& _Other) noexcept = delete;
	Map& operator=(const Map& _Other) = delete;
	Map& operator=(Map&& _Other) noexcept = delete;

	const std::string GetMapName() const
	{
		return MapName;
	}

	const std::string GetColMapName() const
	{
		return ColMapName;
	}

	float4 GetWavePos()	const
	{
		return WaveBackPos;
	}

	HDC GetMapRenderDC() const;


protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	bool IsColMap = false;								//�ȼ� �浹���� ������ ����
	
	float WaveAnimeDiff = 55.0f;						//Wave Animation Render ���� ���� ��
	float FreeSpeed = 900.0f;							//FreeMove ���ǵ�

	float4 WaveBackPos = { 0.0f, 1068.0f };				//Wave BackGround ��ġ
	float4 WaveAnimPos0 = { 0.0f, 696.0f };				//Wave Animation ��ġ	
	float4 WaveAnimPos1 = { 0.0f, 751.0f };
	float4 WaveAnimPos2 = { 0.0f, 806.0f };
	float4 WaveAnimPos3 = { 0.0f, 861.0f };

	float4 WaveBackScale = { 5120.0f, 744.0f };
	float4 WaveScale = { 5120.0f, 110.0f };

	std::string MapName = "MapCity.bmp";
	std::string ColMapName = "MapCity_Ground.bmp";

	GameEngineRender* MapRender = nullptr;

	//ī�޶� �����Ӱ� ������
	bool FreeMoveState(float _DeltaTime);
};

