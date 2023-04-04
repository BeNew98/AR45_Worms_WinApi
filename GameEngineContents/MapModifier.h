#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� : ���� ���� ��ȭ�� ���õ� ���
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

	//�������� ���� ��� �Լ�(���� �ʿ�)
	void CreateHole(float4 _Pos, int _Diameter, bool _DrawLine = true);

    //�簢������ ���� ��� �Լ�(���簢�� ���ο� ���� ũ�� �ʿ�)
    void CreateRect(float4 _Pos, int _Horz, int _Vert, bool _DrawLine = true);

	//�浹ü ������ ��� �Լ�
	GameEngineCollision* GetModifierCollision()
	{
		return ModifierCollision;
	}

    void SetModifierColScale(float4 _Scale);
    void SetModifierColPosition(float4 _Pos);
	
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	int Blue	    = RGB(0, 0, 255);
	int Magenta     = RGB(255, 0, 255);
	int LineColor	= RGB(165, 184, 223);

    float4 Scale = { 3840.0f, 1392.0f };


	GameEngineCollision* ModifierCollision = nullptr;				//Modifier Collision
	
	
	//���� �ǹ��� ���� ���� ���м��� �ִ� �Լ�
    void DrawPixel(float4 _Pos, int _Radius);
    //���� �ǹ��� ���� ���� ���м��� �ִ� �Լ�_�簢�� ����
    void DrawPixelRect(float4 _Pos, int _Horz, int _Vert);
};