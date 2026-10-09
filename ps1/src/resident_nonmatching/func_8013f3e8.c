/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size and register use.
 * The exact owner of the bytes in the PS1 build stays the raw bytes of the
 * resident executable; the build does not use this file. The differential
 * test next to it (difftest.py, with func_8013f3e8.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): pushes a 16-bit value of
 * the object (field_150) into a ring of 16-bit words. Two rings exist, one
 * per side (the object's side byte: 0 left, otherwise right). Word 0 of a
 * ring is its head: the value is stored at word [head], then the head
 * becomes (head + 1) & 0xff, or 1 when that is 0 (word 0 is never an
 * entry slot after a wrap), and is written back into word 0.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's side and field_150; word 0 of the ring chosen by
 *     the side.
 *   Writes: the ring word at the old head (a signed halfword index) and
 *     the ring's word 0.
 *   Aliasing: the object is a block apart from the rings.
 *   Inputs excluded: a head outside 0 to 255 (the ring has 256 words, so
 *     such a head would write past the ring; the original has no check and
 *     the game keeps the head in range by this very function).
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8013f3e8(Object *object) {
    u16 *ring;
    u16 head;

    ring = object->side != 0 ? ring_right : ring_left;
    head = ring[0];
    ring[(s16)head] = object->field_150;
    head = (head + 1) & 0xff;
    if (head == 0) {
        head = 1;
    }
    ring[0] = head;
}
