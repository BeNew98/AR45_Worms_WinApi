#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� : ���� ���� ��ȭ�� ���õ� ���
class GameEngineActor;
class MapModifier : public GameEngineActor
{
public:
	static MapModifier* MainModifier;
public:
	// constrcuter destructer
	MapModifier();
	~MapModifier();

	// delete Function
	MapModifier(const MapModifier& _Other) = delete;
	MapModifier(MapModifier&& _Other) noexcept = delete;
	MapModifier& operator=(const MapModifier& _Other) = delete;
	MapModifier& operator=(MapModifier&& _Other) noexcept = delete;

	//�������� ���� ��� �Լ�
	void CreateHole(float4 _Pos, int _Radius);

	//MapModifier�� ���͸� ����� �Լ�
	void CreateMapModifier();

	//�ǹ��� �´��� �����̿� ���� �׸��� �Լ�
	void DrawArc(float4 _Pos, int _Radius);

	//�浹ü �ϳ� �����_�������� �ݸ���->

	GameEngineCollision* GetModifierCollision()
	{
		return ModifierCollision;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

	int White	= RGB(255, 255, 255);
	int Red		= RGB(255, 0, 0);
	int Blue	= RGB(0, 0, 255);
	int Magenta = RGB(255, 0, 255);
	int Cyan	= RGB(0, 255, 255);

	GameEngineCollision* ModifierCollision = nullptr;				//Modifier Collision

	float4 GetStartArcPos(float4 _Pos, int _Radius);
	float4 GetEndArcPos(float4 _Pos, int _Radius);
};