#pragma once
#include <string>
#include <GameEngineCore/GameEngineActor.h>
#include "MapModifier.h"

// ���� : �ʰ��� ���
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
		return WaveStartPos;
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

    int NumOfObj = 0;                                   //������ �巳 ������Ʈ�� ����

    float WaitTime = 40.0f;
	float WaveAnimeDiff = 55.0f;						//Wave Animation Render ���� ���� ��
	float FreeSpeed = 900.0f;							//FreeMove ���ǵ�

    float4 WaveStartPos = { 1920.0f, 1392.0f };         //Wave BackGround ���� ��ġ

	float4 MountainPos = { 1920.0f, 1264.0f };          //Mountain BackGround ��ġ
	float4 WaveBackPos = { 1920.0f, 1950.0f };			//Wave BackGround ��ġ 


    float4 WaveAnimPos0 = { 1920.0f, 1340.0f };			//Wave Animation ��ġ	
	float4 WaveAnimPos1 = { 1920.0f, 1400.0f };
	float4 WaveAnimPos2 = { 1920.0f, 1447.0f };
	float4 WaveAnimPos3 = { 1920.0f, 1492.0f };
	float4 WaveAnimPos4 = { 1920.0f, 1537.0f };

	float4 BackScale = { 7680.0f, 1116.0f };             //mountain, wave�� ũ��
	float4 WaveScale = { 7680.0f, 165.0f };              //Wave�� ũ��

	std::string MapName = "";
	std::string ColMapName = "";



	std::vector<std::string> Maps = std::vector<std::string>();
	std::vector<std::string> ColMaps = std::vector<std::string>();

	GameEngineRender* MapRender = nullptr;
	GameEngineRender* ColMapRender = nullptr;


	//ī�޶� �����Ӱ� ������
	void FreeMoveState(float _DeltaTime);
};

