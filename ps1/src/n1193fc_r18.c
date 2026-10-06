/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;
extern u16 data_80185fc8;
extern u16 data_80185fcc;
extern int data_80185fd0;
extern int data_80185fd4;
extern s8 data_8016e9a8[];
extern s16 data_8016ea60[];
extern u8 data_8016e9b8[];

/* Residual (not exact): the original selects p with a real if/else and a j
   (gcc here hoists one address before the branch), and keeps the two arms
   that store 4 apart (gcc merges them). Switch gives a tree: not it. */
void func_8012572c(void) {
}

u8 func_80125734(Object *object, u8 a) {
    u8 i;
    if (object->field_cd == 0) {
        for (i = 0; i < 8; i++) {
            if (object->field_130 & 0x100) {
                if ((object->field_130 & data_8016ea60[i]) == data_8016ea60[i]) {
                    return data_8016e9b8[(u8)(object->kind * 8 + i)];
                }
            }
        }
    }
    return a;
}

void func_801257c8(void) {
}
