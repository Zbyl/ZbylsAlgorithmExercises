
#include <cstdint>
#include <cassert>

const int stringSize = 20;
class string
{
public:
    string()
    {
        for (auto& c : chars)
            c = '0';
    }

    string(const char* str)
    {
        int i;        
        for (i = 0; i < 19; ++i)
        {
            if (!str[i])
                break;
            chars[i] = str[i];
        }

        for (; i < 20; ++i)
        {
            chars[i] = 0;
        }
    }

    string(const string& str)
    {
        for (int i = 0; i < 20; ++i)
            chars[i] = str.chars[i];
    }

    string& operator=(const string& str)
    {
        for (int i = 0; i < 20; ++i)
            chars[i] = str.chars[i];

        return *this;
    }

    bool operator==(const string& str) const
    {
        for (int i = 0; i < 20; ++i)
            if (chars[i] != str.chars[i])
                return false;
        return true;
    }

    char chars[stringSize];
};

template<typename T, int Size>
class allocator_t
{
public:
    allocator_t()
        : numAllocated(0)
    {
    }

    T* allocate()
    {
        assert(numAllocated < Size);
        numAllocated++;
        return &blocks[numAllocated - 1];
    }

    void freeAll()
    {
        numAllocated = 0;
    }

public:
    int numAllocated;
    T blocks[Size];
};

uint32_t hash(const string& str)
{
    auto data = reinterpret_cast<const uint32_t*>(str.chars);
    return (data[0] * 11) ^ (data[1] * 7) ^ (data[2] * 17) ^ (data[3] * 13) ^ (data[4] * 5);
}

/// @brief Hashtable from string to T, with Size buckets.
template<typename T, int Size>
class Hashmap
{
public:
    void init(const T& value)
    {
        for (auto& v : buckets)
            v = value;
    }

    void insert(const string& key, const T& value)
    {
        get(key) = value;
    }

    T& get(const string& key)
    {
        auto h = hash(key);
        auto bucket = h % Size;
        return buckets[bucket];
    }

public:
    T buckets[Size];
};

template<typename T>
class KeyValueNode
{
public:
    KeyValueNode()
        : next(nullptr)
    {}

    KeyValueNode(const string& key, const T& value)
        : key(key)
        , value(value)
        , next(nullptr)
    {}

public:
    string key;
    T value;
    KeyValueNode<T>* next;
};

template<typename T, int Size, typename Allocator>
class MultiMap
{
public:
    MultiMap(Allocator& allocator)
        : allocator(allocator)
    {
    }

    void init()
    {
        map.init(nullptr);
    }

    void insert(const string& key, const T& value)
    {
        auto* kvNode = &map.get(key);
        while (*kvNode)
        {
            kvNode = &(*kvNode)->next;
        }

        *kvNode = allocator.allocate();
        **kvNode = KeyValueNode<T>(key, value);
    }

    int remove(const string& key, const T* value = nullptr)
    {
        int removed = 0;

        auto* kvNode = &map.get(key);
        while (*kvNode)
        {
            if ((*kvNode)->key == key)
            {
                if ( !value || ((*kvNode)->value == *value) )
                {
                    *kvNode = (*kvNode)->next;
                    removed++;
                    continue;
                }
            }
            kvNode = &(*kvNode)->next;
        }

        return removed;
    }

    int count(const string& key, T* firstMatch = nullptr)
    {
        auto count = 0;

        auto* kvNode = &map.get(key);
        while (*kvNode)
        {
            if ((*kvNode)->key == key)
            {
                count++;
                if ( (count == 1) && firstMatch )
                {
                    *firstMatch = (*kvNode)->value;
                }
            }
            kvNode = &(*kvNode)->next;
        }

        return count;
    }

public:
    Allocator& allocator;
    Hashmap<KeyValueNode<T>*, Size> map;
};

const int MAX_ROWS = 10;
const int COLS = 5;
const int MAX_OPS = MAX_ROWS * 5;

using index_allocator = allocator_t<KeyValueNode<int>, MAX_OPS * COLS>;
using index_t = MultiMap<int, MAX_ROWS * 3, index_allocator>;

index_allocator allocator;

index_t indexes[COLS] = { {allocator}, {allocator}, {allocator}, {allocator}, {allocator} };

using row_t = string[COLS];

int numRows = 0;
row_t rows[MAX_ROWS];

void initDB()
{
    allocator.freeAll();
    numRows = 0;

    for(auto& index : indexes)
        index.init();
}

void add(const row_t& row)
{
    for (int i = 0; i < COLS; ++i)
        rows[numRows][i] = row[i];

    numRows++;

    for (int i = 0; i < COLS; ++i)
    {
        indexes[i].insert(row[i], numRows - 1);
    }
}

int search(int col, const string& key, int resultCol, string& value)
{
    int id;
    auto count = indexes[col].count(key, &id);
    if (count != 1)
        return count;

    value = rows[id][resultCol];

    return count;
}

int update(int col, const string& key, int updateCol, const string& value)
{
    auto& index = indexes[col];
    auto& updateIndex = indexes[updateCol];

    if ((col == updateCol) && (key == value))
    {
        auto count = index.count(key);
        return count;
    }

    auto updated = 0;
    while (true)
    {
        int id;
        auto count = index.count(key, &id);
        if (count == 0)
            break;

        auto removed = updateIndex.remove(key, &id);
        assert(removed == 1);
        rows[id][updateCol] = value;
        updateIndex.insert(value, id);

        updated++;
    }

    return updated;
}

void removeId(int id)
{
    auto row = rows[id];
    for (int i = 0; i < COLS; ++i)
    {
        auto& index = indexes[i];
        auto removed = index.remove(row[i], &id);
        assert(removed == 1);
    }
}

int remove(int col, const string& key)
{
    auto removed = 0;
    while (true)
    {
        auto& index = indexes[col];

        int id;
        auto count = index.count(key, &id);
        if (count == 0)
            break;

        removed++;
        removeId(id);
    }

    return removed;
}

int main()
{
    initDB();

    add(row_t{"Ala", "Bozon", "2015-11-29", "alka", "girl"});
    add(row_t{"Ala", "Mozek", "2011-11-29", "moza", "girl"});
    add(row_t{"Marek", "Panek", "2010-11-29", "dent", "boy"});
    add(row_t{"Heniek", "Bozon", "2009-11-29", "boyboy", "boy"});

    string rx;
    auto sx = search(0, "Nope", 1, rx);
    string r0;
    auto s0 = search(0, "Ala", 1, r0);
    string r1;
    auto s1 = search(3, "moza", 2, r1);
    string r2;
    auto s2 = search(4, "boy", 0, r2);
    string r3;
    auto s3 = search(0, "Heniek", 3, r3);

    auto u0 = update(4, "boy", 4, "dude");

    auto x0 = remove(4, "dude");
}