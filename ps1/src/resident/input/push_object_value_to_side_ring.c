/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* What it does (inferred, not an original name): pushes a 16-bit value of
   the object (input_word) into a ring of 16-bit words. Two rings exist, one
   per side (the object's side byte: 0 left, otherwise right). Word 0 of a
   ring is its head: the value is stored at word [head], then the head
   becomes (head + 1) & 0xff, or 1 when that is 0 (word 0 is never an
   entry slot after a wrap), and is written back into word 0. The head is
   used as a signed halfword index; the function has no check of it. */
void push_object_value_to_side_ring(Object *object) {
    s16 head;

    /* The read of the head and the store are written once per side: the
       original loads the ring's address in each arm and jumps to one
       shared run of instructions, which is what the compiler makes of two
       arms that end alike. The 16 bytes of stack that the original
       reserves and never uses come with this text too, with no local for
       them. Inferred from the build, not recovered source. */
    if (object->side != 0) {
        head = ring_right[0];
        ring_right[head] = object->input_word;
    } else {
        head = ring_left[0];
        ring_left[head] = object->input_word;
    }
    head = (head + 1) & 0xff;
    if (head == 0) {
        head = 1;
    }
    if (object->side != 0) {
        ring_right[0] = head;
    } else {
        ring_left[0] = head;
    }
}
