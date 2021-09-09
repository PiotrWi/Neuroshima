#pragma once

#include <deque>
#include <stack>
#include <vector>

#include <GameTypes.h>

struct Attack
{
    enum class Type {
        physical,
    } type;

    int power;
    int initiative;
    Orientation orientation;
};

struct Coin
{
    enum class Type {
        headquarter,
        unit,
        module
    } type;
    const char* name;
    int hitPoints;

    std::vector<Attack> AttackList;
    ~Coin() = default;
};

struct Memorial
{
    struct Change
    {
        Coin c;
        int position;

        Change(const Coin& coin, int pos)
            : c(coin)
            , position(pos) {}
    };
    std::stack<Change> changes;
};

enum class ArmyType
{
    Borgo,
    Hegemony,
    Moloh,
    Post
};

struct RandomGetResults
{
    std::vector<Coin> coins;
    Memorial memorial;
};

class Army
{
public:
    Army(const ArmyType&, std::deque<Coin>&&);
    ArmyType getArmyType() const;
    void shuffle();
    std::vector<Coin> getNexts(unsigned int n);
    RandomGetResults getRandomNexts(unsigned int n);
private:
    ArmyType armyType;
    std::deque<Coin> coins;
};
