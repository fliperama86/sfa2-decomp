/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): sets up a scene of eleven
 * objects. When the given object's field_f0 is 0 it sets the HUD state's
 * field_60 to 600 and adds 1 to its field_52, calls func_80022bf0_slot28
 * (no argument), and prepares the object held in data_80051bd0_slot28
 * (position y -0x60, field_01 1, sequence set with func_80130768). Then it
 * asks func_8011f1e0 for nine objects; each one it gets is filled in and
 * entered in the table data_80051ba4_slot28 at index 1 to 9. It asks for
 * one more, which it sets up with func_80022c6c_slot28 and func_80130768 and
 * enters at index 0. It calls func_80128370, func_8014f4d4 (1, 0x504) and
 * func_80022c3c_slot28 (the given object, 4).
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Returns at once, writing nothing, when the object's field_f0 is not 0.
 *   Reads: the object's field_f0; the HUD state pointer data_8018f5a0 and
 *     its field_52; the object pointer in data_80051bd0_slot28.
 *   Writes (when field_f0 is 0): HUD state field_60 and field_52; the
 *     object of data_80051bd0_slot28 (pos_y, field_01); each object that
 *     func_8011f1e0 returns (for each of the first nine: field_00 to
 *     field_03, field_09, field_0d, field_7a, field_7c, box_tables,
 *     field_90, field_98, field_9c; for the tenth: pos_x, pos_y, field_09
 *     (set to 2 before the func_80130768 call and to 3 after it),
 *     field_0d, field_7a, field_7c); the table words at index 0 to 9.
 *   Aliasing: the given object, the HUD state, the object of
 *     data_80051bd0_slot28 and the ten objects func_8011f1e0 returns are
 *     distinct blocks; the table is another block.
 *   Callees replaced by recorders (same in both runs): func_80022bf0_slot28
 *     (no argument), func_80130768 (3 arguments), func_8011f1e0 (no
 *     argument; the setup gives it, per call in turn, a new object or 0),
 *     func_80022c6c_slot28 (1), func_80128370 (0; it waits for the
 *     hardware, read from the original's listing, not tested),
 *     func_8014f4d4 (2; it reaches the library, read from the original's
 *     listing, not tested), func_80022c3c_slot28 (2). All results are 0
 *     except those of func_8011f1e0 and the one of func_80022bf0_slot28,
 *     which is random per case (the C does not use it). The log copies, at
 *     every call, the given object, the HUD state, the object of
 *     data_80051bd0_slot28, the ten returned objects (each whole, 0x394
 *     bytes) and the table words at index 0 to 9 and the pointer after
 *     them, so that a store moved across a call is seen.
 *   Excluded inputs: none.
 *   Not reached by any input: none.
 * The tree declares func_8011f1e0 as returning a Block172 pointer; the result is cast to Object * here.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
/* Tables of the module, inferred from the code. */
extern Object *data_80051ba4_slot28[];
extern Object *data_80051bd0_slot28[];
extern u8 data_800469ec_slot28[];
extern u8 data_80046d34_slot28[];
extern SequenceStep *data_80047bdc_slot28[];
extern SequenceStep *data_80047be4_slot28[];
void func_80128370(void);
void func_80022bf0_slot28(void);
void func_80022c3c_slot28(Object *obj, int arg);
void func_80022c6c_slot28(Object *obj);

void func_80022944_slot28(Object *obj) {
    Object *o;
    int i;

    if (obj->field_f0 != 0) {
        return;
    }
    data_8018f5a0->field_60 = 0x258;
    data_8018f5a0->field_52++;
    func_80022bf0_slot28();

    o = data_80051bd0_slot28[0];
    o->pos_y = -0x60;
    o->field_01 = 1;
    func_80130768(o, 0, data_80047bdc_slot28);

    for (i = 1; i < 10; i++) {
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_02 = 0x65;
            o->field_09 = 2;
            o->field_7a = 0x60;
            o->field_7c = 0x1e0;
            o->field_90 = (void *)0x80060000;
            o->field_98 = data_800469ec_slot28;
            o->field_9c = data_80046d34_slot28;
            o->field_00 = 1;
            o->field_03 = i;
            o->field_01 = 1;
            o->field_0d = 0;
            o->box_tables = (BoxTables *)data_80047be4_slot28;
            data_80051ba4_slot28[i] = o;
        }
    }

    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80022c6c_slot28(o);
        o->pos_x = 0xc8;
        o->pos_y = 0xa0;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 2;
        func_80130768(o, 2, data_80047be4_slot28);
        data_80051ba4_slot28[0] = o;
        o->field_09 = 3;
    }
    func_80128370();
    func_8014f4d4(1, 0x504);
    func_80022c3c_slot28(obj, 4);
}
