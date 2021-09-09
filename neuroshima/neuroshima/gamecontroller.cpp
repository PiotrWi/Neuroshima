#include "gamecontroller.h"

GameController::GameController(std::array<Army, 2>&& armies)
    : armies_(std::move(armies))
{
}

void GameController::start()
{
    for (auto&& army: armies_)
    {
        army.shuffle();
    }
    for (auto&& observer: gameObservers)
    {
        observer->notifyChange();
    }
    for (auto&& client: clients[roundNumber%2])
    {
        client->actionRequired(IGameClient::action::placeHeadquarter);
    }
}
