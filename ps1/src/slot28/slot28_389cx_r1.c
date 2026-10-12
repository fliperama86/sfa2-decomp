/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
/* A table of 16 bytes, inferred from the code. */
extern u8 data_80051ea8_slot28[16];
void func_80013c38_slot28(Object *obj);

/* What it does (inferred, not an original name): resets state at the start
   of a scene part. It clears three halfwords of the HUD state, adds 1 to a
   fourth, clears a byte of the object (offset 0x4f) and field_64, clears a
   16-byte table of the module, sets the HUD state's field_5e to 0xf0, calls
   func_8011eae4 and func_80013c38_slot28, and sets the object's field_ad
   to 0xff. */
/* The call of func_8011eae4 passes one argument although the callee takes none: the original sets the first argument register before it. Written without the argument, this function differs from the original in 1 instruction slots. */
void func_8001389c_slot28(Object *obj) {
    int i = 0xf;
    u8 *p;

    data_8018f5a0->field_50 = 0;
    data_8018f5a0->field_52 = 0;
    data_8018f5a0->field_54 = 0;
    data_8018f5a0->field_4e = data_8018f5a0->field_4e + 1;
    /* The byte at 0x4f is the last byte of vel_x. */
    ((u8 *)&obj->vel_x)[3] = 0;
    obj->field_64 = 0;
    p = data_80051ea8_slot28 + 15;
    do {
        *p = 0;
        p--;
        /* Stand-in, not recovered source: the statement below, which ORs to the counter a term that is 0 for every value of it, reproduces the original's choice of registers. Without it this function differs from the original in 8 instruction slots. */
        i |= ((u32)i << 24) & 0x100;
    } while (--i >= 0);
    data_8018f5a0->field_5e = 0xf0;
    ((void (*)(Object *))func_8011eae4)(obj);
    func_80013c38_slot28(obj);
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    /* Without it this function differs from the original in 4 instruction slots. */
    do {
        obj->field_ad = 0xff;
    } while (0);
}
