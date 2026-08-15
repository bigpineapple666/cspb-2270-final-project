# The "hash" OR MPHF module

This is where the code lives for all of the underlying logic for our minimal perfect hash function. The main goals of minimal perfect hash function are 2-fold. One is it is collision free, meaning because it requires all keys to be known at build time, it is not possible to have collisions (in practice) for minimal perfect hash functions because all keys are transmuted into the bitvector at construction. The second and more impressive goal is reducing memory overhead. Minimal perfect hashing scales very well and is therefore great to use on very large sets of data where the keys are preknown. This particular implementation uses BBHash algorithms and at gamma equals 2, can get down to a mere *3 bits a key at scale*. For comparison a 5 letter word encoded in ACSII is 40 bits, so at scale this structure is incredibly memory efficient.

## The Basics

This data structure is best explained by breaking down the constructor. When creating a minimal perfect hash function, The goal is to pack and reduce the memory overhead of keys.  

### Prehashing
First, we prehash each key from a string to an int64 value. The reason this structure requires prehashing is because in construction we technically hash keys multiple times because to handle collisions, we must pack keys in multiple levels, and then combine them into one bitvector once *collisions are resolved at build time*. 

### Levels and bitvector creation

Once we prehash each key, then break each integer key down further into levels, which are layers of bitvectors. This is where the logic gets very advanced, and most of this is taken from BBHash directly. For all bitvector levels, a constant gamma is set which is a measure of how dense a level should be. A gamma of 2 means each level must contain twice as many slots as keys. Levels in this algorithm are the collision reduction mechanism. 

Each key is transcribed into a bitvector using set and our mix function to rehash each key once per level to find a slot for that respective key, and collisions are detected via get. If a collision occurs, then we mark the key as collided and attempt to added it at the next level. This process continues until each key is transcribed into a bitvector level. Once all keys are populated into each level, then the levels are combined and this forms our final bitvector for key lookup.

## Key Lookup

Looking a key up is really just repeating what we already did at build time. We
prehash the key the same way, then walk the levels in order, using mix with the
same seed and level number and the same slot count we used during construction.
Because nothing about that math has changed, each level hands back the exact same
slot it did when we built the structure.

At each level we check that slot with get. If the bit is clear, this key collided
here during the build and got pushed down, so we move on to the next level. If
the bit is set, we are done, because a slot only ever gets set when exactly one
key claims it. 

The last step is turning that position into something useful. Our bitvector is
much bigger than the number of keys we actually have, so the position itself is
full of gaps and would waste most of an array. Calling rank9 on it counts the set
bits before it, which renumbers every key into a tight range from 0 to the number
of keys. That number is what we return, and Tensor Map uses this number as the index to fetch its given value.

