#include <iostream>

#include <array>

#include <ArmyFactory.h>
#include <gamecontroller.h>

using namespace std;

int main()
{
    std::array<Army, 2> armies {
        createArmy(ArmyType::Borgo),
        createArmy(ArmyType::Moloh)
    };
    GameController game(std::move(armies));

    return 0;
}
