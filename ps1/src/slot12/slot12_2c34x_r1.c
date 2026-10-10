/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019054_slot12[];
extern u8 data_80019548_slot12[];
extern SequenceStep *data_8001d4ec_slot12[];

/* The margin is read into a local after the first group of stores and used
   for pos_x at the end. Read in place at that statement, this function
   differs from the original in 32 instruction slots; read one statement
   earlier, in 26; one statement later, in 13; at the declaration, in 35. */
void func_80012c34_slot12(Object *obj) {
    u16 margin;
    obj->field_01 = 1;
    obj->field_09 = 0;
    obj->field_0b = 0;
    obj->field_0c = 0;
    obj->field_0d = 0;
    obj->field_04++;
    margin = box_margin[0];
    obj->pos_y = 0x78;
    obj->field_98 = data_80019054_slot12;
    obj->field_9c = data_80019548_slot12;
    obj->field_90 = (void *)0x80055598;
    obj->field_7a = 0;
    obj->field_7c = 0x1f1;
    obj->pos_x = margin + 0xc0;
    func_80130768(obj, 0, data_8001d4ec_slot12);
    func_80131094(obj);
}
