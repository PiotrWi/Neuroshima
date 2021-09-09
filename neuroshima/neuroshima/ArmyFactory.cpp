#include <ArmyFactory.h>

#include <deque>
#include <map>

const char* headquarter = "headquarter";

std::vector<Attack> createHeadQuarterAttack()
{
    return {
        Attack{Attack::Type::physical, 1, 0, Orientation::TopRight},
        Attack{Attack::Type::physical, 1, 0, Orientation::Right},
        Attack{Attack::Type::physical, 1, 0, Orientation::BottomRight},
        Attack{Attack::Type::physical, 1, 0, Orientation::BottomLeft},
        Attack{Attack::Type::physical, 1, 0, Orientation::Left},
        Attack{Attack::Type::physical, 1, 0, Orientation::TopLeft},
    };
}

std::deque<Coin> createBorgoArmy()
{
    return {Coin{Coin::Type::headquarter, headquarter, 20, createHeadQuarterAttack()}};
}

std::deque<Coin> createMolohArmy()
{
    return {Coin{Coin::Type::headquarter, headquarter, 20, createHeadQuarterAttack()}};
}

Army createArmy(ArmyType type)
{
    std::map<ArmyType, std::deque<Coin>(*)()> createConcreteArmy {
        {ArmyType::Borgo, createBorgoArmy},
        {ArmyType::Moloh, createMolohArmy}
    };
    return Army{type, createConcreteArmy[type]()};
}
