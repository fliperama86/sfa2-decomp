#include "../world.h"
#include <string.h>
static struct intro_world world;
int intro_fade_test(uint8_t *palette,uint8_t *flags,unsigned mode,unsigned *target)
{
    int result;
    memset(&world,0,sizeof world);memcpy(world.video+0xc000,palette,4096);memcpy(ig(&world,0x4f40),flags,3);
    world.fade_mode=mode;world.fade_target=*target;result=intro_fade_tick(&world);
    memcpy(palette,world.video+0xc000,4096);memcpy(flags,ig(&world,0x4f40),3);*target=world.fade_target;
    return result;
}
