/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c5410_slot04_06[];

int func_8014693c(Object *object);
void func_80146960(Object *object);
int func_80140cd8(Object *obj, int a, int b);
void func_801b4818_slot04_06(Object *obj);
void func_801b4850_slot04_06(Object *obj);
void func_801b4874_slot04_06(Object *obj, u16 *p);

void func_801b2850_slot04_06(Object *obj) {
    s16 t;
    GameState *g = &game_state;

    func_801b4850_slot04_06(obj);
    if (obj->pos_y < obj->field_70) {
        func_801b4818_slot04_06(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        func_80146960(obj);
        if ((s8)func_8014693c(obj) != 0) {
            t = 0x80;
            func_801b4874_slot04_06(obj, (u16 *)&t);
            ref_other.p->pos_x += t;
            ref_other.p->pos_y -= 0x10;
            if ((s8)func_8014693c(obj) != 0) {
                t = 0x40;
                func_801b4874_slot04_06(obj, (u16 *)&t);
                ref_other.p->pos_x += t;
                ref_other.p->pos_y -= 0x10;
            }
        }
        obj->pos_y = obj->field_70;
        obj->field_45 = 0;
        ((Slot04aObj *)obj)->field_1c2 = 0xc;
        if (obj->field_12a != 4) {
            ((Slot04aObj *)obj)->field_1c2 = 0x1c;
        }
        g->field_63 = 0x1c;
        if ((u8)func_80140cd8(obj, *(s16 *)(data_801c5410_slot04_06 + (obj->field_12a & 0xfe)), 0) != 0 && obj->field_12a != 4) {
            g->field_6b = 4;
            func_80147000(obj);
            func_80120554(obj, obj->side, 0x319);
            ((Slot04aObj *)obj)->field_1c2 = 0x30;
            g->field_63 = 0x20;
            func_801307e0(obj, 0x25);
        } else {
            func_80120554(obj, obj->side, 0x319);
            func_801307e0(obj, 0x25);
        }
    }
}
