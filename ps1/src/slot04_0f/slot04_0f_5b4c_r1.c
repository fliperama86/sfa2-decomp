/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190458;
extern Slot04_0fRec6194 data_801c6194_slot04_0f[];

Block172 *func_8011f1e0(void);

void func_801b5b4c_slot04_0f(Object *obj) {
    Object *p;
    u16 t;
    int a;
    int x;
    int y;
    int dx;
    int dy;

    if (((game_state.config->field_33 + data_80190458.p->side) & 3) == 0) {
        t = obj->field_3a;
        a = -3;
        if ((t & 0xff) != 0) {
            if (t & 0x80) {
                a = 3;
            }
            obj->field_48 = (obj->field_48 + a) & 0x3f;
            p = (Object *)func_8011f1e0();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 0x25;
                p->field_3c = data_80190458.p;
                p->field_0c = data_80190458.p->field_0c;
                p->field_7a = 0x60;
                p->field_7c = 0x1e0;
                p->field_0d = obj->field_0d;
                p->field_90 = data_80190458.p->field_90;
                p->field_98 = data_80190458.p->field_98;
                p->field_9c = data_80190458.p->field_9c;
                p->field_08 = 0x20;
                p->field_02 = 4;
                p->field_66 = obj->field_66;
                p->field_0e = data_80190458.p->field_0e;
                p->pos_x = data_80190458.p->pos_x;
                p->pos_y = data_80190458.p->pos_y - 0x3a;
                a = 0x28;
                if (data_80190458.p->field_0b == 0) {
                    a = -0x28;
                }
                p->pos_x += a;
                p->field_09 = 6;
                p->field_1c = data_80190458.p->field_1c;
                p->field_0b = data_80190458.p->field_0b;
                a = obj->field_48 & 0x1f;
                y = data_801c6194_slot04_0f[a].b;
                x = data_801c6194_slot04_0f[a].a;
                y = -y;
                x <<= 8;
                y <<= 8;
                dx = x << 3;
                dy = y << 4;
                if (data_80190458.p->field_0b == 0) {
                    x = -x;
                    dx = -dx;
                }
                p->field_4c = x;
                p->field_50 = y;
                *(s32 *)&p->field_10 += dx;
                *(s32 *)&p->field_14 += dy;
            }
        }
    }
}
