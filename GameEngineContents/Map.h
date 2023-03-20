#pragma once
#include <string>
#include <GameEngineCore/GameEngineActor.h>
#include "MapModifier.h"

// 설명 :
class Map : public GameEngineActor
{
public:
	static Map* MainMap;
	static int MapMode;									//Map 선택
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


	//HDC를 얻어오는 함수
	HDC GetMapDC() const;
	HDC GetColMapDC() const;

	//MapModifier 객체 생성 함수
	void CreateMapModifier();


protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	bool IsColMap = false;								//픽셀 충돌맵의 랜더링 여부
	
	float WaveAnimeDiff = 55.0f;						//Wave Animation Render 간의 높이 차
	float FreeSpeed = 900.0f;							//FreeMove 스피드

	float4 WaveBackPos = { 1920.0f, 1764.0f };			//Wave BackGround 위치
	float4 WaveAnimPos0 = { 1920.0f, 1392.0f };			//Wave Animation 위치	
	float4 WaveAnimPos1 = { 1920.0f, 1447.0f };
	float4 WaveAnimPos2 = { 1920.0f, 1502.0f };
	float4 WaveAnimPos3 = { 1920.0f, 1557.0f };

	float4 WaveBackScale = { 5120.0f, 744.0f };
	float4 WaveScale = { 5120.0f, 110.0f };

	std::string MapName = "MapCity.bmp";
	std::string ColMapName = "MapCity_Ground.bmp";

	std::vector<std::string> Maps = std::vector<std::string>();
	std::vector<std::string> ColMaps = std::vector<std::string>();

	GameEngineRender* MapRender = nullptr;
	GameEngineRender* ColMapRender = nullptr;


	//카메라를 자유롭게 움직임
	bool FreeMoveState(float _DeltaTime);
};

