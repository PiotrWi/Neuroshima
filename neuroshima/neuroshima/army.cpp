#include "army.h"

#include <algorithm>
#include <random>

static std::random_device rd;
static std::mt19937 g(rd());


Army::Army(const ArmyType& type, std::deque<Coin>&& c)
    : armyType(type)
    , coins(std::move(c))
{
}

ArmyType Army::getArmyType() const
{
    return armyType;
}

void Army::shuffle()
{
    constexpr auto headquarterOffset = 1;
    std::shuffle(coins.begin() + headquarterOffset, coins.end(), g);
}

std::vector<Coin> Army::getNexts(unsigned int n)
{
    std::vector<Coin> coinsToReturn;

    for (auto i = 0u; i < n && !coins.empty(); ++i)
    {
        coinsToReturn.emplace_back(coins.front());
        coins.pop_front();
    }

    return coinsToReturn;
}

RandomGetResults Army::getRandomNexts(unsigned int n)
{
    RandomGetResults result;

    for (auto i = 0u; i < n && !coins.empty(); ++i)
    {
        auto position = g() % coins.size();
        auto coin = coins[position];
        result.coins.emplace_back(coin);
        coins.erase(coins.begin() + position);

        result.memorial.changes.emplace(coin, position);
    }

    return result;
}
