#pragma once
#include <string>
#include <GameEngineCore/GameEngineActor.h>
#include "MapModifier.h"

// ���� :
class Map : public GameEngineActor
{
public:
	static Map* MainMap;
	static int MapMode;									//Map ����
public:
	// constrcuter destructer
	Map();
	~Map();

	// delete Function
	Map(const Map& _Other) = delete;
	Map(Map&& _Other) noexcept = delete;
	Map& operator=(const Map& _Other) = delete;
	Map& operator=(Map&& _Other) noexcept = delete;

	const std::string& GetMapName() const
	{
		return MapName;
	}

	const std::string& GetColMapName() const
	{
		return ColMapName;
	}

	float4 GetWavePos()	const
	{
		return WaveBackPos;
	}

	//HDC�� ������ �Լ�
	HDC GetMapDC() const;
	HDC GetColMapDC() const;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	bool IsColMap = false;								//�ȼ� �浹���� ������ ����
	
	float WaveAnimeDiff = 55.0f;						//Wave Animation Render ���� ���� ��
	float FreeSpeed = 900.0f;							//FreeMove ���ǵ�

	float4 WaveBackPos = { 1920.0f, 1764.0f };			//Wave BackGround ��ġ
	float4 WaveAnimPos0 = { 1920.0f, 1392.0f };			//Wave Animation ��ġ	
	float4 WaveAnimPos1 = { 1920.0f, 1437.0f };
	float4 WaveAnimPos2 = { 1920.0f, 1492.0f };
	float4 WaveAnimPos3 = { 1920.0f, 1547.0f };

	float4 WaveBackScale = { 5120.0f, 744.0f };
	float4 WaveScale = { 5120.0f, 110.0f };

	std::string MapName = "";
	std::string ColMapName = "";

	std::vector<std::string> Maps = std::vector<std::string>();
	std::vector<std::string> ColMaps = std::vector<std::string>();

	GameEngineRender* MapRender = nullptr;
	GameEngineRender* ColMapRender = nullptr;


	//ī�޶� �����Ӱ� ������
	bool FreeMoveState(float _DeltaTime);
};

