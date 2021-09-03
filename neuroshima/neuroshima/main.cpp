#include <iostream>

#include "HexContainer.h"

using namespace std;

int main()
{
    HexTable<int, 3> table;

    for (auto i = 0; i < table.size(); ++i)
    {
        table[i] = i;
    }

    for (auto it = table.begin(); it != table.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    return 0;
}
