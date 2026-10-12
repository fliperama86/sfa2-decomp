/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
/* A table of 16 bytes, inferred from the code. */
extern u8 data_80051ea8_slot28[16];
void copy_table_twice_call_state_handler_slot28(Object *obj);

/* What it does (inferred, not an original name): resets state at the start
   of a scene part. It clears three halfwords of the HUD state, adds 1 to a
   fourth, clears a byte of the object (offset 0x4f) and damage, clears a
   16-byte table of the module, sets the HUD state's field_5e to 0xf0, calls
   func_8011eae4 and copy_table_twice_call_state_handler_slot28, and sets the object's active_flag_2
   to 0xff. */
/* The call of func_8011eae4 passes one argument although the callee takes none: the original sets the first argument register before it. Written without the argument, this function differs from the original in 1 instruction slots. */
void func_8001389c_slot28(Object *obj) {
    int i = 0xf;
    u8 *p;

    data_8018f5a0->step_index_2 = 0;
    data_8018f5a0->pass_index = 0;
    data_8018f5a0->field_54 = 0;
    data_8018f5a0->substep_index = data_8018f5a0->substep_index + 1;
    /* The byte at 0x4f is the last byte of vel_x. */
    ((u8 *)&obj->vel_x)[3] = 0;
    obj->damage = 0;
    p = data_80051ea8_slot28 + 15;
    do {
        *p = 0;
        /* The step is taken on the address as an integer, not as p--: behind
           the last pass it leaves the table by one byte, which pointer
           arithmetic of C may not do (it reaches the elements and one past
           the end, not one before the first). This assumes flat addresses:
           a pointer converted to a wide unsigned integer and back is that
           pointer, moved by the bytes subtracted. It is not portable C in
           general. The integer has 64 bits, so that it holds a pointer of
           the PC build whole. The step builds the instruction of p--. */
        p = (u8 *)((unsigned long long)p - 1);
        /* Stand-in, not recovered source: the statement below, which ORs to the counter a term that is 0 for every value of it, reproduces the original's choice of registers. Without it this function differs from the original in 8 instruction slots. */
        i |= ((u32)i << 24) & 0x100;
    } while (--i >= 0);
    data_8018f5a0->field_5e = 0xf0;
    ((void (*)(Object *))func_8011eae4)(obj);
    copy_table_twice_call_state_handler_slot28(obj);
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    /* Without it this function differs from the original in 4 instruction slots. */
    do {
        obj->active_flag_2 = 0xff;
    } while (0);
}
