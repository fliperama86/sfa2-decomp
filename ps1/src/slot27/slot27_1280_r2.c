/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800267a4_slot27[])(Object *);
extern void (*data_800267b4_slot27[])(Object *);

void func_800113d8_slot27(void) {
    int set;
    int i;
    int j;

    if (data_8018db10 == 0) {
        for (set = 0; set < 5; set++) {
            for (i = 0; i < 0x10; i++) {
                for (j = 0; j < 0x20; j++) {
                    data_801a27e4_rows[set][j * 0x10 + i] = 0;
                }
            }
        }
        data_8018f598 = 1;
        data_8018db10 = data_8018db10 + 1;
    }
    func_80137220(0, 0);
    func_80137220(1, 1);
    func_80137220(2, 2);
    func_80137220(3, 3);
    func_80137220(4, 7);
}

void func_800114ac_slot27(Object *object) {
    data_800267a4_slot27[object->field_04](object);
}

void func_800114ec_slot27(Object *object) {
    data_800267b4_slot27[object->field_05](object);
}
