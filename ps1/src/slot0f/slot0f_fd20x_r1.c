/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e004c_slot0f(Object *obj);
void func_800e07b8_slot0f(int a);

void func_800dfd20_slot0f(GameState *state, Menu *menu) {
    u16 buttons;
    func_800e004c_slot0f((Object *)menu);
    buttons = data_801a696a | data_801a6976;
    if ((buttons & 0x5000) != 0) {
        if ((buttons & 0x1000) != 0) {
            menu->field_00 = (s8)menu->field_00 - 1;
            if ((s8)menu->field_00 < 0) {
                menu->field_00 = 5;
            }
            if ((s8)menu->field_02 != 3 && (s8)menu->field_00 == 1) {
                menu->field_00 = (s8)menu->field_00 - 1;
            }
        } else {
            menu->field_00 = menu->field_00 + 1;
            if ((s8)menu->field_00 >= 6) {
                menu->field_00 = 0;
            }
            if ((s8)menu->field_02 != 3 && (s8)menu->field_00 == 1) {
                menu->field_00 = (s8)menu->field_00 + 1;
            }
        }
        func_80120554(0, 0, 0x204);
    }
    if ((s8)menu->field_02 != (s8)menu->field_03) {
        if (((s8)menu->field_02 & 1) == 0) {
            if ((s8)menu->field_00 == 2) {
                menu->field_00 = 0;
            }
        }
        if ((s8)menu->field_02 != 3 && (s8)menu->field_00 == 1) {
            menu->field_00 = 0;
        }
    }
    menu->field_01 = 0;
    if ((buttons & 0x820) != 0) {
        if ((data_801a696a & 0x820) != 0) {
            menu->field_01 = 1;
        } else {
            menu->field_01 = 2;
        }
        data_8018f5a0->field_4a++;
        func_80120554(0, 0, 0x205);
    }
    func_800e07b8_slot0f((s8)menu->field_00);
}
