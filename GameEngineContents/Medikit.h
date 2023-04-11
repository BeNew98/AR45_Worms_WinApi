#pragma once
#include "MapObject.h"

// ���� :
class Medikit : public MapObject
{
public:
	// constrcuter destructer
	Medikit();
	~Medikit();

	// delete Function
	Medikit(const Medikit& _Other) = delete;
	Medikit(Medikit&& _Other) noexcept = delete;
	Medikit& operator=(const Medikit& _Other) = delete;
	Medikit& operator=(Medikit&& _Other) noexcept = delete;

protected:
    void Start() override;
    void Update(float _DeltaTime) override;

private:
    bool IsChangedAnim = false;                                //ó�� �����Ǽ� ���ϻ����� �ϰ��ϰ� ���� ��Ҵٸ� �� ���ķδ� �ִϸ��̼��� �������� ����
    bool IsPlaySound = false;

    int Recovery = 50;

    std::string DirString = "Right_";                          //������ �� ����

    void DirCheck(const std::string_view& _AnimationName);     //�ִϸ��̼� ���� üũ �� ����

    void RecoveryCheck();                                      //Player�� �浹�ϸ� ü�� ȸ��

    bool IsWindApplied();                                      //���߿� �ִٸ� �ٶ��� ������ ����

};

