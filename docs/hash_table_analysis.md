# Hash Table Analysis Laboratory

In this laboratory, I implemented a hash table in C++ that stores string keys and integer values. My implementation uses open addressing with quadratic probing to handle collisions. It supports inserting, searching, retrieving, updating, and removing entries. It also uses tombstones to preserve correct searching after deletion and rehashing to increase the table's capacity when needed.


- insertion with collision handling;
The insert function adds a new key-value pair to the hash table. It uses the hash function to find the starting index and quadratic probing to find another available position when a collision occurs. Before inserting, it checks whether the key already exists to prevent duplicates. It can also reuse deleted positions and resize the table when more space is needed.

- lookup and retrieval;
The contains function checks whether a key exists in the hash table, while the get function retrieves the value associated with a key. Both functions use quadratic probing to search for the key. If get cannot find the key, then it throws an out_of_range exception.

- update of existing entries;
The update function searches for an existing key and changes its associated value. If the key is found, then it updates the value and returns true. If the key does not exist, it returns false. Updating an entry does not change the size of the hash table.

- deletion while preserving probe correctness;
The remove function deletes an entry by marking it as deleted instead of making its position empty. This creates a tombstone, which allows future searches to continue through that position. The size of the hash table also decreases after a successful removal.

- capacity growth and rehashing;
The hash table increases its capacity when it reaches its allowed number of entries. The rehash function creates a larger table with a prime capacity and reinserts all active entries using the new capacity. Deleted entries will not be copied into the new table.

- hash function design;
My implementation uses an FNV-style hash function to convert string keys into table indexes. It processes each character using XOR and multiplication, then uses the modulo operator to keep the resulting index within the table capacity.

- complex-key scenarios with many collisions.
The hash table stores string keys and uses quadratic probing to handle collisions. I tested collision handling with multiple strings, including three keys in a table with a capacity of five. The implementation successfully stored and retrieved the entries without creating duplicates.

## Required analysis

- average-case insertion, lookup, and removal;
Insertion, lookup, and removal take O(1) time when the hash function distributes keys well and the load factor remains reasonable. This is because most operations only need to check a small number of positions.

- best-case and worst-case behavior;
The best case is O(1), which is when the correct position is available immediately or the key is found on the first attempt. The worst case is O(n), which is when many collisions occur and the operation must examine many positions in the table.

- collision resolution strategies;
Collision resolution strategies are linear probing, quadratic probing, and separate chaining. Linear probing checks consecutive positions, while quadratic probing uses squared increments to find another position. Separate chaining stores multiple entries in a collection at the same index.

- load factor and rehashing costs;
The load factor measures how full a hash table is by dividing the number of stored entries by its capacity. As the load factor increases, collisions become more likely and operations can take longer. Rehashing increases the capacity and reinserts existing entries, which takes O(n) time on average.

- open addressing vs. chaining;
Open addressing stores all entries directly inside the hash table and uses probing to find available positions. It can use memory efficiently, but performance decreases when the table becomes crowded. Chaining stores multiple entries in separate collections at each index. It handles collisions more flexibly but requires additional memory for those collections.

- how clustering can affect performance.
Clustering occurs when several entries occupy nearby positions, causing more collisions during insertion and searching. Quadratic probing reduces primary clustering compared to linear probing, but it can still experience secondary clustering when keys have the same starting index. As clustering increases, operations will take longer.

## Big-O expectations

- insert(): Average O(1) because a position is usually found after a small number of probes. Worst-case O(n) when many collisions occur.

- remove(): Average O(1) because the key is usually found quickly. Worst-case O(n) when many positions must be checked.

- get(): Average O(1) because retrieving a value usually requires only a few probes. Worst-case O(n) when collisions make searching slower.

- update(): Average O(1) because an existing key is usually found quickly. Worst-case O(n) when many positions must be examined.

- rehash(): Average O(n) because it scans the old table and reinserts its active entries into a larger table. Severe collisions during reinsertion can increase the cost to O(n²).

- contains(): Average O(1) because the key is usually found or determined to be missing after a small number of probes. Worst-case O(n) when many positions must be checked.

Hash tables provide efficient average-case performance when keys are distributed well and the load factor is controlled. However, collisions and clustering can increase the time required for operations. Expected average-case is O(1) for insert, lookup, and update under a reasonable load factor; worst case can degrade to O(n) when many collisions occur or the table becomes badly loaded.
