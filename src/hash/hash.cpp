// hashing
// the real crux of the project, in the paper cited in 
// readme.md, there are many listed techniques for implmeneting 
// MPHF, but over large N fingerprinting is the best option

// For more on fingerprinting and the algorithm, read docs
// or the readme.md

// in MPHF, it is assumed that the data structure is already well known
// thus, we dont need traditional hash functions for this because uniqueness
// not the issue, instead we construct the whole structure all at once!

// additionally, MPHF removes the concept of nodes, instead in MPHF with
// fingerprinting we use bit vectors instead

#include "hash.h"

// constructor: in a MPHF, the keys are preknown
// implementation as per BBHash: https://github.com/rizkg/BBHash

void MPHF::build(const std::vector<uint64_t>& keys) {
    if (keys.empty()) return;

    // set levels for MPHF
    std::vector<uint64_t> remaining = keys;
    std::vector<BitVector> levels;          // one array per level, kept to concatenate
    for (unsigned int l = 0; l < MAX_LEVELS && !remaining.empty(); ++l) {
        const size_t m = std::max<size_t>(1, size_t(gamma * remaining.size()) + 1);

        BitVector taken(m), collided(m);

        // Pass 1: mark occupancy, and mark any slot hit more than once.
        for (uint64_t k : remaining) {
            const size_t p = mix(k, seed + l) % m;
            if (taken.get(p)) collided.set(p);
            else              taken.set(p);
        }

        // Pass 2: a key is placed iff its slot was hit exactly once.
        BitVector level(m);
        std::vector<uint64_t> survivors;
        for (uint64_t k : remaining) {
            const size_t p = mix(k, seed + l) % m;
            if (collided.get(p)) survivors.push_back(k);
            else                 level.set(p);
        }

        level_sizes.push_back(m);
        levels.push_back(std::move(level));
        remaining.swap(survivors);
    }

    // Concatenate every level into one bit vector, record offsets
    size_t total = 0;
    for (size_t m : level_sizes) { level_offsets.push_back(total); total += m; }

    bv = BitVector(total);
    for (size_t l = 0; l < levels.size(); ++l)
        for (size_t p = 0; p < level_sizes[l]; ++p)
            if (levels[l].get(p)) bv.set(level_offsets[l] + p);

    bv.build_rank9();

    // Anything still unplaced after MAX_LEVELS gets an explicit entry
    uint64_t next = bv.rank9(total);  
    for (uint64_t k : remaining) fallback[k] = next++;

    if (next != nkeys) return;

}
// invoking the construction with build logic
MPHF::MPHF(const std::vector<uint64_t>& keys, double __gamma, uint64_t __seed) {
    nkeys = keys.size();
    gamma = __gamma;
    seed = __seed;
    build(keys);
}

// mixing, using splitmix64: https://github.com/indiesoftby/defold-splitmix64
uint64_t MPHF::mix(uint64_t x, uint64_t s) {
    // each of these funky looking numbers is a magic hashing number for 
    // fast hashing
    x += s * 0x9E3779B97F4A7C15ULL;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
    return x ^ (x >> 31);
}

// lookup a key
uint64_t MPHF::lookup(uint64_t key) {
    for (size_t l = 0; l < level_sizes.size(); ++l) {
        const size_t p = mix(key, seed + l) % level_sizes[l];
        const size_t g = level_offsets[l] + p;
        if (bv.get(g)) return bv.rank9(g);
    }
    auto it = fallback.find(key);
    return it != fallback.end() ? it->second : 0;
}

// get the total number of keys
size_t MPHF::size() { 
    return nkeys; 
}

// get the number of levels
size_t MPHF::levels() { 
    return level_sizes.size(); 
}

// get bit count
size_t MPHF::bit_count() {
    return bv.getNBits();
}

// return bits per key
double MPHF::bits_per_key() { 
    return double(bit_count()) / nkeys; 
}

// prehashing function, to go from string data to bits
// for this we used FNV1A: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
uint64_t fnv1a_64(const void* data, size_t len) {
    uint64_t hash = 2166136261u;
    const uint8_t* p = (const uint8_t*)data;
    for (size_t i = 0; i < len; i++) {
        hash ^= p[i];
        hash *= 16777619u;
    }
    return hash;
}
