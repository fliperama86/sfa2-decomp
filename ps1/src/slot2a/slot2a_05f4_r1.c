/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

typedef void (*SlotFn)(Object *);

extern u8 data_801e25d8_slot2a[];
extern SlotFn table_801e225c_slot2a[];
extern u8 data_801e25dc_slot2a[];
extern u16 *data_801e52c8_slot2a;
void func_801e06a0_slot2a(Object *obj);
void func_801e08f4_slot2a(Object *obj, int a);
void func_801e0958_slot2a(Object *obj, u8 *p);
void func_801e0d94_slot2a(Object *obj, u8 *p, int n);
void func_801e0e48_slot2a(Object *obj);
void func_801e114c_slot2a(Object *obj);

void func_801e05f4_slot2a(Object *obj) {
    u8 t;
    obj->field_05 = 2;
    obj->field_01 = 0;
    obj->field_0e = 0;
    obj->field_09 = 0;
    obj->field_0f = 0;
    obj->field_26 = 0;
    obj->field_04 = obj->field_04 + 1;
    data_801e25d8_slot2a[obj->field_03] = 0;
    obj->pos_y = 0x40;
    if (obj->field_03 != 0) {
        obj->pos_y = 0xa0;
    }
    obj->pos_x = 0x30;
    t = obj->field_45 ^ obj->field_03;
    obj->field_0b = t;
    if (t != 0) {
        obj->pos_x = 0x140;
    }
    func_801e0e48_slot2a(obj);
    func_801e06a0_slot2a(obj);
}

void func_801e06a0_slot2a(Object *obj) {
    table_801e225c_slot2a[obj->field_05](obj);
    if (data_801e25d8_slot2a[obj->field_03] != 0) {
        func_801e0958_slot2a(obj, data_801e25dc_slot2a + obj->field_03 * 0x500 + data_801a27d0 * 0x280);
    }
    func_801e114c_slot2a(obj);
}

void func_801e074c_slot2a(Object *obj) {
    obj->field_05++;
    if (obj->field_45 != (*data_801e52c8_slot2a >> 8)) {
        obj->field_3c = &player_right;
    } else {
        obj->field_3c = &player_left;
    }
    func_801e08f4_slot2a(obj, obj->field_3c->kind);
    func_801e0d94_slot2a(obj, data_801e25dc_slot2a + obj->field_03 * 0x500, 0xb);
    func_801e0d94_slot2a(obj, data_801e25dc_slot2a + 0x280 + obj->field_03 * 0x500, 0xb);
    data_801e25d8_slot2a[obj->field_03] = 1;
}
