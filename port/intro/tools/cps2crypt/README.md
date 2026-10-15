# Import-only CPS2 data decryption

`cps2crypt.cpp` is third-party MAME source under BSD-3-Clause, not game source
reconstructed by this project. Copyright notices and the license are retained.
It is never linked into the native game, used as an instruction interpreter,
or shipped with a key or ROM. The importer uses it only to extract the inline
data operands consumed by project-authored C. Whole program images are not
retained in the resulting sparse data pack.

Pinned upstream: MAME commit `32f48ebb75f4fefcc7bcf0482289c0afa57d9973`,
[`src/mame/capcom/cps2crypt.cpp`](https://github.com/mamedev/mame/blob/32f48ebb75f4fefcc7bcf0482289c0afa57d9973/src/mame/capcom/cps2crypt.cpp).
Original file SHA-256:
`c0f9ef55059fc48c5507a5f0ab1122826b44c98210adfe197e9b356d86a9a5e5`.

Adaptation: remove MAME-specific includes, the machine argument and UI progress
block; supply the small bit-permutation compatibility header. Cipher tables
and arithmetic are otherwise unchanged. `decrypt.cpp` is the project-authored
bounded byte-array/key bridge. The user supplies the archive, including its key;
input and decrypted-plane digests must match the examined revision.
