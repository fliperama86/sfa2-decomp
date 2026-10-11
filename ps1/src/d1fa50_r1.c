/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/*
 * Makes a record the object's current sequence, copies the record's header
 * word into the object (high half to step_timer, low half to step_flags), then
 * copies the record's data block into a grid of cells at a place derived from
 * the object's position: pos_x selects a column (bits 9 to 11 and bits 5 to
 * 8) and pos_y / 32 a row, in the buffer that cam_f50 points to (inferred,
 * not an original description).
 */

/* The buffer pointer is read as the member at offset 0x50 of the camera block cam_obj; a plain read of cam_f50 differs in 4 instruction lines (the compiler then moves its load below the store of the sequence). */
typedef struct {
    u8 pad[0x50];
    u8 *map;
} CamBlock;

void func_8011fa50(Object *object, SeqRec *rec) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[2];
    int offset, col, bits;
    u16 pos_x;
    int half;
    int word;
    u8 *base = ((CamBlock *)cam_obj)->map;

    object->sequence = (SequenceStep *)rec;
    pos_x = object->pos_x;
    half = rec->header;
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    do {
        word = half;
        half = word >> 16;
        object->step_timer = half;
        object->step_flags = word;
        bits = pos_x & 0xe00;
        /* The original adds 31 to the masked value before dividing by 32 for a negative value; the value (pos_x & 0x1e0) is never negative, so that slot is never reached. Without the (s16) cast the unit differs in 26 instruction lines. */
        col = (s16)(pos_x & 0x1e0) / 32 * 4;
        offset = col + bits + (((object->pos_y / 32) << 22) >> 16);
    } while (0);
    func_8011fb70(rec->data, (u16 *)(base + ((offset << 15) >> 15)));
}
