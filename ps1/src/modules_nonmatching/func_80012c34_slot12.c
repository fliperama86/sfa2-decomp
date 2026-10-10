/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code orders its stores differently. The exact owner of the bytes in
 * the PS1 build stays the raw bytes of the module image; the build does not
 * use this file. The differential test next to it (difftest.py, with
 * func_80012c34_slot12.py) compares the behavior of this C with the original
 * code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): initializes an object of
 * this module: sets its state bytes, counts one more use in field_04, places
 * it at y 0x78 and at x = box_margin + 0xc0, gives it two data tables and a
 * graphics pointer, then calls two resident routines on it (the second sets
 * up the object's sequence from the table data_8001d4ec_slot12).
 *
 * Contract:
 *   Argument: a0 = pointer to an object (0x394 bytes in the test).
 *   No return value.
 *   Reads: the object's field_04 (byte, incremented, any value), the
 *     halfword box_margin.
 *   Writes: the object's fields 01, 04, 09, 0b, 0c, 0d (bytes), 16, 12, 7a,
 *     7c (halfwords) and 90, 98, 9c (words). Nothing else.
 *   Callees, both replaced by recorders (they run object code of the
 *     resident image on state the contract does not build): func_80130768
 *     (3 arguments: object, 0, the table) and func_80131094 (1 argument:
 *     the object). Their results are unused. The log watches the whole
 *     object at every call, so a field written after a call that the
 *     original writes before it is a difference.
 *   Aliasing: the object is a block of its own.
 *   Not reached: nothing is excluded; every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019054_slot12[];
extern u8 data_80019548_slot12[];
extern SequenceStep *data_8001d4ec_slot12[];
extern u8 data_80055598[];

void func_80012c34_slot12(Object *obj) {
    obj->field_01 = 1;
    obj->field_09 = 0;
    obj->field_0b = 0;
    obj->field_0c = 0;
    obj->field_0d = 0;
    obj->field_04++;
    obj->pos_y = 0x78;
    obj->field_98 = data_80019054_slot12;
    obj->field_9c = data_80019548_slot12;
    obj->field_90 = data_80055598;
    obj->field_7a = 0;
    obj->field_7c = 0x1f1;
    obj->pos_x = box_margin[0] + 0xc0;
    func_80130768(obj, 0, data_8001d4ec_slot12);
    func_80131094(obj);
}
