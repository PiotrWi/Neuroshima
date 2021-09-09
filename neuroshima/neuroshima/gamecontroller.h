/*
 * At this moment the goal is to write instantion which allow to play for 2 players
 */

#pragma once

#include <array>

#include <army.h>
#include <GameTypes.h>
#include <HexContainer.h>

struct IGameClient
{
    enum class action
    {
        placeHeadquarter,
    };
    virtual void actionRequired(action);
};

struct IGameObserver
{
    virtual void notifyChange();
};

struct PlaceHeadQuarter
{
    int position;
    Orientation orientation;
};

struct CoinInGameDecorator
{
    Orientation orientation;
    int damageTaken;

    Coin coin;
    ~CoinInGameDecorator() = default;
};

struct Field
{
    bool empty = true;
    CoinInGameDecorator coin;
    ~Field() {};
};

class GameController
{
public:
    GameController(std::array<Army, 2>&& armies);
    void start();

    template <typename Action>
    void perform(int player, const Action&);

    void registerClient(int player, IGameClient*);
    void registerObserver(IGameObserver*);
private:
    HexTable<Field, 3> board;

    std::array<std::vector<Coin>, 2> hand;
    std::array<Army, 2> armies_;

    std::vector<IGameObserver*> gameObservers;
    std::array<std::vector<IGameClient*>, 2> clients;

    int activePlayer = 0;
    int roundNumber = 0;
};

template<> void GameController::perform<PlaceHeadQuarter>(int player, const PlaceHeadQuarter&);
