#include "project/hash_table.h"

#include <cmath>
#include <stdexcept>
#include <utility>

HashTable::HashTable(std::size_t capacity)
    : table_(capacity), size_(0)
{
    if (capacity == 0)
    {
        throw std::invalid_argument("Hash table capacity must be greater than zero");
    }
    for (std::size_t i = 0; i < table_.size(); ++i)
    {
        table_[i] = HashEntry();
    }
}

HashTable::~HashTable() = default;

std::size_t HashTable::hashFunction(const std::string &key) const
{
    std::size_t hash = 2166136261u;
    for (unsigned char ch : key)
    {
        hash ^= ch;
        hash *= 16777619u;
    }
    return hash % table_.size();
}

std::size_t HashTable::probe(const std::string &key) const
{
    std::size_t index = hashFunction(key);
    std::size_t original = index;
    std::size_t step = 1;

    while (table_[index].occupied && table_[index].key != key)
    {
        index = (original + step * step) % table_.size();
        ++step;
    }
    return index;
}

bool HashTable::insert(const std::string &key, int value)
{
    std::size_t original = hashFunction(key);
    std::size_t capacity = table_.size();

    std::size_t firstDeleted = capacity;
    for (std::size_t i = 0; i < capacity; ++i)
    {
        std::size_t index = (original + i * i) % capacity;
        // Stop if reaching an empty slot.
        if (!table_[index].occupied)
        {
            break;
        }
        // Stop if the key already exists.
        if (!table_[index].deleted && table_[index].key == key)
        {
            return false;
        }
        // Record the first deleted slot.
        if (table_[index].deleted && firstDeleted == capacity)
        {
            firstDeleted = index;
        }
    }
    // Use the first deleted slot if no empty slot was found.
    std::size_t target = firstDeleted;
    if (target == capacity)
    {
        // Search for an empty slot using quadratic probing.
        for (std::size_t i = 0; i < capacity; ++i)
        {
            std::size_t index = (original + i * i) % capacity;
            if (!table_[index].occupied)
            {
                target = index;
                break;
            }
        }
    }
    // Rehash and retry if no slot was found or the table is half full.
    if (target == capacity || size_ >= (capacity + 1) / 2)
    {
        rehash();
        return insert(key, value);
    }
    // store the new key and value.
    table_[target].key = key;
    table_[target].value = value;
    table_[target].occupied = true;
    table_[target].deleted = false;
    ++size_;
    return true;
}

bool HashTable::remove(const std::string &key)
{
    std::size_t original = hashFunction(key);
    std::size_t capacity = table_.size();
    for (std::size_t i = 0; i < capacity; ++i)
    {
        std::size_t index = (original + i * i) % capacity;
        if (!table_[index].occupied)
        {
            return false;
        }
        if (!table_[index].deleted && table_[index].key == key)
        {
            table_[index].deleted = true;
            --size_;
            return true;
        }
    }
    return false;
}

bool HashTable::contains(const std::string &key) const
{
    std::size_t original = hashFunction(key);
    std::size_t capacity = table_.size();
    for (std::size_t i = 0; i < capacity; ++i)
    {
        std::size_t index = (original + i * i) % capacity;

        // If the slot is not occupied, the key is not in the table.
        if (!table_[index].occupied)
        {
            return false;
        }
        // If the slot is occupied and not deleted, check if the key matches.
        if (!table_[index].deleted && table_[index].key == key)
        {
            return true;
        }
    }
    return false;
}

int HashTable::get(const std::string &key) const
{
    std::size_t original = hashFunction(key);
    std::size_t capacity = table_.size();
    for (std::size_t i = 0; i < capacity; ++i)
    {
        std::size_t index = (original + i * i) % capacity;
        // Stop if reaching an empty slot.
        if (!table_[index].occupied)
        {
            throw std::out_of_range("Key not found");
        }
        // return the value if the key matches.
        if (!table_[index].deleted && table_[index].key == key)
        {
            return table_[index].value;
        }
    }
    throw std::out_of_range("Key not found");
}

bool HashTable::update(const std::string &key, int newValue)
{
    std::size_t original = hashFunction(key);
    std::size_t capacity = table_.size();
    for (std::size_t i = 0; i < capacity; ++i)
    {
        std::size_t index = (original + i * i) % capacity;
        if (!table_[index].occupied)
        {
            return false;
        }
        if (!table_[index].deleted && table_[index].key == key)
        {
            table_[index].value = newValue;
            return true;
        }
    }
    return false;
}

std::size_t HashTable::size() const noexcept
{
    return size_;
}

std::size_t HashTable::capacity() const noexcept
{
    return table_.size();
}

bool HashTable::empty() const noexcept
{
    return size_ == 0;
}

double HashTable::loadFactor() const noexcept
{
    return static_cast<double>(size_) / static_cast<double>(table_.size());
}

std::vector<std::string> HashTable::keys() const
{
    std::vector<std::string> result;
    for (const auto &entry : table_)
    {
        if (entry.occupied && !entry.deleted)
        {
            result.push_back(entry.key);
        }
    }
    return result;
}

std::vector<int> HashTable::values() const
{
    std::vector<int> result;
    for (const auto &entry : table_)
    {
        if (entry.occupied && !entry.deleted)
        {
            result.push_back(entry.value);
        }
    }
    return result;
}

void HashTable::rehash()
{
    // Save the current table before rehashing.
    std::vector<HashEntry> oldTable = table_;
    // Calculate the new capacity and initialize the new table.
    std::size_t newCapacity = nextPrime(oldTable.size() * 2);
    // Create a new table with the calculated capacity.
    table_.assign(newCapacity, HashEntry{});
    size_ = 0;
    // Reinsert all active entries into the new table.
    for (const auto &entry : oldTable)
    {
        if (entry.occupied && !entry.deleted)
        {
            std::size_t original = hashFunction(entry.key);
            for (std::size_t i = 0; i < newCapacity; ++i)
            {
                std::size_t index = (original + i * i) % newCapacity;
                if (!table_[index].occupied)
                {
                    table_[index].key = entry.key;
                    table_[index].value = entry.value;
                    table_[index].occupied = true;
                    table_[index].deleted = false;
                    ++size_;
                    break;
                }
            }
        }
    }
}

bool HashTable::isPrime(std::size_t value) const
{
    if (value < 2)
    {
        return false;
    }
    if (value == 2)
    {
        return true;
    }
    if (value % 2 == 0)
    {
        return false;
    }
    for (std::size_t i = 3; i * i <= value; i += 2)
    {
        if (value % i == 0)
        {
            return false;
        }
    }
    return true;
}

std::size_t HashTable::nextPrime(std::size_t value) const
{
    if (value <= 2)
    {
        return 2;
    }
    std::size_t candidate = value % 2 == 0 ? value + 1 : value;
    while (!isPrime(candidate))
    {
        candidate += 2;
    }
    return candidate;
}
