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


	//�浹ü ������ ��� �Լ�
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
	int LineColor	= RGB(165, 184, 223);

	float4 StartArcPos = float4::Zero;
	float4 EndArcPos = float4::Zero;

	GameEngineCollision* ModifierCollision = nullptr;				//Modifier Collision
	
	
	//���� �ǹ��� ���� ���� ���� �׸�
	void DrawPixel(float4 _Pos, int _Radius);
};