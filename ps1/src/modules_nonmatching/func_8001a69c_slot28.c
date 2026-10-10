/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): sets up a scene of five
 * objects. When data_80190949 is not 0 and the given object's field_f0 is
 * 0, it sets the HUD state's field_60 to 480 and adds 1 to its field_52,
 * calls func_8001a9e8_slot28 (no argument), sets the sequence of the
 * objects held in data_800519c0_slot28 and data_800519c4_slot28 with
 * func_80130768, and positions the object of data_800519c8_slot28. It then
 * asks func_8011f1e0 for two objects; each one it gets is filled in and
 * entered in data_80051998_slot28 and data_8005199c_slot28. Last it calls
 * func_8001a958_slot28 (the given object, 4) and func_80128370.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Returns at once, writing nothing, when data_80190949 is 0 or the
 *     object's field_f0 is not 0.
 *   Reads: data_80190949; the object's field_f0; the HUD state pointer
 *     data_8018f5a0 and its field_52; the three object pointers
 *     data_800519c0_slot28, data_800519c4_slot28, data_800519c8_slot28.
 *   Writes: HUD state field_60 and field_52; pos_y of the objects of
 *     data_800519c4_slot28 and data_800519c8_slot28, field_01 of the latter;
 *     each object that func_8011f1e0 returns; the words
 *     data_80051998_slot28 and data_8005199c_slot28 when it returned one.
 *   Aliasing: the given object, the HUD state, the three table objects and
 *     the two returned objects are distinct blocks.
 *   Callees replaced by recorders (same in both runs): func_8001a9e8_slot28
 *     (no argument), func_80130768 (3 arguments), func_8011f1e0 (no
 *     argument; the setup gives it, per call in turn, a new object or 0),
 *     func_8001a988_slot28 (1), func_8001a958_slot28 (2), func_80128370
 *     (0; it waits for the hardware, read from the original's listing, not
 *     tested). All results are 0 except those of
 *     func_8011f1e0. The log copies, at every call, the given object, the
 *     HUD state, the three table objects and the two returned objects (each
 *     whole, 0x394 bytes) and the five table words, so that a store moved
 *     across a call is seen.
 *   Excluded inputs: none.
 *   Not reached by any input: none.
 * The tree declares func_8011f1e0 as returning a Block172 pointer; the result is cast to Object * here.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
/* Tables of the module, inferred from the code. */
extern SequenceStep *data_80036694_slot28[];
extern SequenceStep *data_8003669c_slot28[];
extern SequenceStep *data_800366a8_slot28[];
extern u8 data_80035430_slot28[];
extern u8 data_800357d8_slot28[];
extern ObjectRef data_800519c0_slot28;
extern Object *data_800519c4_slot28[];
extern ObjectRef data_800519c8_slot28;
extern Object *data_80051998_slot28[];
extern ObjectRef data_8005199c_slot28;
void func_80128370(void);
void func_8001a958_slot28(Object *obj, int arg);
void func_8001a988_slot28(Object *obj);
void func_8001a9e8_slot28(void);

void func_8001a69c_slot28(Object *obj) {
    Object *o;

    if (data_80190949 == 0 || obj->field_f0 != 0) {
        return;
    }
    data_8018f5a0->field_52++;
    data_8018f5a0->field_60 = 0x1e0;
    func_8001a9e8_slot28();

    func_80130768(data_800519c0_slot28.p, 1, data_80036694_slot28);
    o = data_800519c4_slot28[0];
    o->pos_y = -0x60;
    func_80130768(o, 0, data_8003669c_slot28);
    o = data_800519c8_slot28.p;
    o->field_01 = 1;
    o->pos_y = 0x20;

    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001a988_slot28(o);
        o->pos_x = 0xc0;
        o->pos_y = 0x90;
        o->field_46 = 0x40;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 3;
        func_80130768(o, 5, data_800366a8_slot28);
        data_80051998_slot28[0] = o;
    }

    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_02 = 0x33;
        o->field_03 = 2;
        o->field_90 = (void *)0x80060000;
        o->field_98 = data_80035430_slot28;
        o->field_9c = data_800357d8_slot28;
        o->pos_x = 0xb8;
        o->pos_y = 0x64;
        o->field_50 = 0x8000;
        o->field_58 = -0x400;
        o->field_7a = 0x60;
        o->field_09 = 3;
        o->field_00 = 1;
        o->field_01 = 0;
        o->field_46 = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->box_tables = (BoxTables *)data_800366a8_slot28;
        data_8005199c_slot28.p = o;
    }
    func_8001a958_slot28(obj, 4);
    func_80128370();
}
