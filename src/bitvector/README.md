# The bitvector module

While in this project we are using MPHF for more of a hybrid purpose, the main goal of Minimal Perfect Hashing is to reduce the size overhead of each key by packing key representations into a bit vector. This is done through a process called fingerprinting, which is explained in `hash/README.md`. Therefore, the base structure required to build a minimal perfect hash function is a bit vector, so this is built a layer below the minimal perfect hash function.

## The Basics

Packing bits into a vector is actually a fairly complicated process. The most intuitive thing may seem to just create a boolean array of 1s and 0s, but this does not truely pack bits in a size efficient manor into a vector. Instead, we used uint64_t, or a 64 bit integer, and manipulate its binary representation. In addition to packing, we also need to perform operations on the binary representation to make use of it for getting and setting keys, thus we defined 3 functions `set(i)`, `get(i)`, and `clear(i)` to help us do this, using bitshifting operations. I have provided diagrams below on specifically what each function does.

### `set(i)` — turn a bit on

`set` builds a mask with a single 1 at offset `i` and ORs it into the word. OR
leaves a bit alone unless the mask has a 1 there, so nothing else is disturbed.

```text
bv.set(3);                          bits[i >> 6] |= uint64_t(1) << (i & 63);

           0 1 2 3 4 5 6 7
bits[0]:   0 0 0 0 0 0 0 0
mask:      0 0 0 1 0 0 0 0          1 << 3
           ---------------  |=
bits[0]:   0 0 0 1 0 0 0 0
                 ^
                 bit 3 is now on
```

### `clear(i)` — turn a bit off

`clear` builds the same mask, then flips it with `~` so every position *except*
`i` holds a 1. ANDing with that keeps every other bit and forces bit `i` to 0.

```text
bv.clear(3);                        bits[i >> 6] &= ~(uint64_t(1) << (i & 63));

           0 1 2 3 4 5 6 7
bits[0]:   1 0 0 1 0 0 1 0
~mask:     1 1 1 0 1 1 1 1          ~(1 << 3)
           ---------------  &
bits[0]:   1 0 0 0 0 0 1 0
                 ^
                 bit 3 is now off, bits 0 and 6 survived
```

### `get(i)` — read a bit

`get` shifts the word right by `i`, which slides the bit we want down into
position 0, then ANDs with `1` to throw away everything above it. The result is
`0` or `1`, which converts straight to a `bool`.

```text
bv.get(3);                          return (bits[i >> 6] >> (i & 63)) & 1;

           0 1 2 3 4 5 6 7
bits[0]:   1 0 0 1 0 0 1 0
           ---------------  >> 3
shifted:   1 0 0 1 0 0 0 0          bit 3 has moved to position 0
mask:      1 0 0 0 0 0 0 0          1
           ---------------  &
result:    1 0 0 0 0 0 0 0          -> 1, so get returns true
```

Note the shift is drawn moving **left** in these diagrams: because bit 0 sits on
the left, a right shift by 3 pulls every bit three columns toward the front.

## Rank9 (and we its used here)

The operations above tell us about one bit at a time. What a minimal perfect
hash function actually needs is a *rank* query: given an index `i`, how many 1s
appear in the vector before it? That count is what turns a sparse fingerprint
position into a dense, gap-free slot number, that is then used as the key or index to fetch values from our structure, which is the whole point of the
"minimal" in MPHF.

The clear way to answer it is to walk every word up to `i` and popcount each
one. That is correct but linear in the size of the vector, and rank gets called
constantly during lookup, so it needs to be a constant-time operation instead.

Rank9 buys that constant time by precomputing an index once, in `build_rank9`,
and then reading two numbers out of it at query time:

* An **absolute count** stored for every block of 8 words (512 bits) — the total
  number of 1s in the entire vector before that block starts.
* A set of **relative counts** for the 7 words inside the block, measured from
  the start of that block. These are small enough to fit in 9 bits each (a
  512-bit block can hold at most 512 ones), so all 7 pack into a single
  `uint64_t` alongside the absolute count.