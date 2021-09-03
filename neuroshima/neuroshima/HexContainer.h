/*
 * TODO(PiotrWi): To write an iterator to allow to move over hexes.
 * TODO(PiotrWi): To test it on ut.
 * */

#pragma once

/*
 *     | 16| 17| 18|
 *   | 12| 13| 14| 15|
 * | 7 | 8 | 9 | 10| 11|
 *   | 3 | 4 | 5 | 6 |
 *     | 0 | 1 | 2 |
 */

#define NO_COPYABLE_MACRO(NAME) \
    NAME(const NAME&) = delete; \
    NAME(NAME&&) = delete; \
    NAME operator=(const NAME&) = delete; \
    NAME operator=(NAME&&) = delete;

namespace
{
template <unsigned int TEdgeSize>
constexpr unsigned int getSizeOfTable()
{
    static_assert (TEdgeSize != 0ul, "edgeSize size 0 do not create valid hexTable");
    auto size = 0u;
    for (int i = 0; i < TEdgeSize - 1; ++i)
    {
        size += 2 * (TEdgeSize + i);
    }
    size += 2 * TEdgeSize - 1;
    return size;
}
}  // namespace

enum class Dirrection
{
    TopRight = 0u,
    Right = 1u,
    BottomRight = 2u,
    BottomLeft = 3u,
    Left = 4u,
    TopLeft =5u,
};

int to_int(const Dirrection& dir)
{
    return static_cast<int>(dir);
}

template <typename TContent>
struct HexIt;

template <typename TContent>
bool operator==(const HexIt<TContent>& lhs, const HexIt<TContent>& rhs);
template <typename TContent>
bool operator!=(const HexIt<TContent>& lhs, const HexIt<TContent>& rhs);

template <typename TContent>
struct Hex
{
NO_COPYABLE_MACRO(Hex);
friend HexIt<TContent>;
    Hex() {};
public:
    int neigbourIndexes[6];
    Hex* neighbours[6];
    TContent content;
};

template <typename TContent>
struct HexIt
{
    HexIt(Hex<TContent>*);

    void operator++();
    TContent& operator*();

    void next(const Dirrection&);
    void prev(const Dirrection&);

    friend bool operator==<>(const HexIt<TContent>& lhs, const HexIt<TContent>& rhs);
    friend bool operator!=<>(const HexIt<TContent>& lhs, const HexIt<TContent>& rhs);
private:
    Hex<TContent>* current;
};

template <typename TContent>
HexIt<TContent>::HexIt(Hex<TContent>* ptr)
    : current(ptr)
{
}

template <typename TContent>
TContent& HexIt<TContent>::operator*()
{
    return current->content;
}

template <typename TContent>
void HexIt<TContent>::next(const Dirrection& dir)
{
    current = current->neighbours[to_int(dir)];
}

template <typename TContent>
void HexIt<TContent>::prev(const Dirrection& dir)
{
    const auto oposite_direction_int = (to_int(dir)+3) % 6;
    current = current->neighbours[oposite_direction_int];
}

template <typename TContent>
bool operator==(const HexIt<TContent>& lhs, const HexIt<TContent>& rhs)
{
    return lhs.current == rhs.current;
}

template <typename TContent>
bool operator!=(const HexIt<TContent>& lhs, const HexIt<TContent>& rhs)
{
    return lhs.current != rhs.current;
}

template <typename TContent>
void HexIt<TContent>::operator++()
{
    ++current;
}

template <typename TContent, unsigned int TEdgeSize>
class HexTable
{
NO_COPYABLE_MACRO(HexTable);  // TODO(PiotrWi): To be supported later if needed.
public:
    HexTable() {};
    ~HexTable() {};

    int size() const;
    HexIt<TContent> begin();
    HexIt<TContent> end();
    TContent& operator[](int);
    const TContent& operator[](int) const;

private:
    void initHexes();

    static constexpr unsigned int TSize = getSizeOfTable<TEdgeSize>();
    Hex<TContent> data[TSize];
    Hex<TContent> endQuard;

};

template <typename TContent, unsigned int TEdgeSize>
int HexTable<TContent, TEdgeSize>::size() const
{
    return TSize;
}

template <typename TContent, unsigned int TEdgeSize>
HexIt<TContent> HexTable<TContent, TEdgeSize>::begin()
{
    return HexIt<TContent>{data};
}

template <typename TContent, unsigned int TEdgeSize>
HexIt<TContent> HexTable<TContent, TEdgeSize>::end()
{
    return HexIt<TContent>{&endQuard};
}

template <typename TContent, unsigned int TEdgeSize>
TContent& HexTable<TContent, TEdgeSize>::operator[](int i)
{
    return data[i].content;
}

template <typename TContent, unsigned int TEdgeSize>
const TContent& HexTable<TContent, TEdgeSize>::operator[](int i) const
{
    return data[i].content;
}

template <typename TContent, unsigned int TEdgeSize>
void HexTable<TContent, TEdgeSize>::initHexes()
{
    endQuard.neigbourIndexes = {-1, -1, -1, -1, -1, -1};
    endQuard.neighbours = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    const auto longestLineLen = 2*TEdgeSize - 1;
    const auto lineNum = longestLineLen;
    const auto bottomPartEnd = TSize - longestLineLen / 2;

    auto lineLen = TEdgeSize;
    auto line = 0;
    auto indexInLine = 0;
    auto i = 0u;

    for ( ;i < bottomPartEnd; ++i)
    {
        if(indexInLine == lineLen)
        {
            indexInLine = 0;
            ++line;
            lineLen+= 1;
        }
        data[i].neigbourIndexes[to_int(Dirrection::TopRight)] = i + lineLen + 1;
        data[i].neigbourIndexes[to_int(Dirrection::Right)] = ((indexInLine + 1 < lineLen) ? i + 1 : -1);
        data[i].neigbourIndexes[to_int(Dirrection::BottomRight)] = (line > 0 and indexInLine < lineLen - 1) ? (i - lineLen) + 1 : -1;
        data[i].neigbourIndexes[to_int(Dirrection::BottomLeft)] = (line > 0 and indexInLine > 0) ? (i - lineLen) : -1;
        data[i].neigbourIndexes[to_int(Dirrection::Left)] = (indexInLine > 0) ? (i - 1) : -1;
        data[i].neigbourIndexes[to_int(Dirrection::TopLeft)] = i + lineLen - 1;
        ++indexInLine;
    }

    line += 1;
    indexInLine = 0;
    lineLen += 1;
    for (; i < bottomPartEnd + longestLineLen; ++i, indexInLine)
    {
        data[i].neigbourIndexes[to_int(Dirrection::TopRight)] = i + lineLen - 1;
        data[i].neigbourIndexes[to_int(Dirrection::Right)] = ((indexInLine + 1 < lineLen) ? i + 1 : -1);
        data[i].neigbourIndexes[to_int(Dirrection::BottomRight)] = (0 and indexInLine < lineLen - 1) ? (i - lineLen) + 1 : -1;
        data[i].neigbourIndexes[to_int(Dirrection::BottomLeft)] = (indexInLine > 0) ? (i - lineLen) : -1;
        data[i].neigbourIndexes[to_int(Dirrection::Left)] = (indexInLine > 0) ? (i - 1) : -1;
        data[i].neigbourIndexes[to_int(Dirrection::TopLeft)] = (indexInLine > 0) ? i + lineLen- 1  : -1;
    }

    line += 1;
    indexInLine = 0;
    lineLen -= 1;
    for (; i < TSize; ++i, ++indexInLine)
    {
        if(indexInLine == lineLen)
        {
            indexInLine = 0;
            ++line;
            lineLen -= 1;
        }
        data[i].neigbourIndexes[to_int(Dirrection::TopRight)] = (line + 1 != lineNum and indexInLine < lineNum - 1) ? i + lineLen : -1;
        data[i].neigbourIndexes[to_int(Dirrection::Right)] = ((indexInLine + 1 < lineLen) ? i + 1 : -1);
        data[i].neigbourIndexes[to_int(Dirrection::BottomRight)] = (0 and indexInLine < lineLen - 1) ? (i - lineLen) + 1 : -1;
        data[i].neigbourIndexes[to_int(Dirrection::BottomLeft)] = (indexInLine > 0) ? (i - lineLen) : -1;
        data[i].neigbourIndexes[to_int(Dirrection::Left)] = (indexInLine > 0) ? (i - 1) : -1;
        data[i].neigbourIndexes[to_int(Dirrection::TopLeft)] = (indexInLine > 0) ? i + lineLen- 1  : -1;
    }

    for (auto i = 0; i < TSize; ++i)
    {
        for (auto dir_i = 0; dir_i < 6; ++dir_i)
        {
            auto index = data[i].neigbourIndexes[dir_i];
            if (index != -1)
            {
                data[i].neighbours[dir_i] = data + data[i].neigbourIndexes[dir_i];
            }
            else
            {
                data[i].neighbours[dir_i] = &endQuard;
            }
        }
    }
}
