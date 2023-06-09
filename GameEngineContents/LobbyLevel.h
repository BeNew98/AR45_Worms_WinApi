#pragma once
#include <GameEngineCore/GameEngineLevel.h>

// ���� :
class LobbyLevel : public GameEngineLevel
{
public:
    // constrcuter destructer
    LobbyLevel();
    ~LobbyLevel();

    // delete Function
    LobbyLevel(const LobbyLevel& _Other) = delete;
    LobbyLevel(LobbyLevel&& _Other) noexcept = delete;
    LobbyLevel& operator=(const LobbyLevel& _Other) = delete;
    LobbyLevel& operator=(LobbyLevel&& _Other) noexcept = delete;

protected:
    void Loading() override;
    void Update(float _DeltaTime) override;

    virtual void LevelChangeStart(GameEngineLevel* _PrevLevel) override {}
    virtual void LevelChangeEnd(GameEngineLevel* _NextLevel) override {}


private:
    GameEngineRender* AnimationBodyRender = nullptr;
};


