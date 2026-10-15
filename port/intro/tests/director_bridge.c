/* Isolated director harness. Actor execution and presentation are dependencies,
 * not part of this comparison. Inputs/resources supplied by the test caller. */
#include "../world.h"
#include <string.h>
static struct intro_world world;
static const uint8_t *planes[2];
static size_t sizes[2];
static const uint8_t *read_table(void *context,unsigned plane,uint32_t address,size_t n)
{
    (void)context;
    return plane<2 && address<=sizes[plane] && n<=sizes[plane]-address ? planes[plane]+address : NULL;
}
static int actor(struct intro_world *w,unsigned pool,uint16_t id) {(void)w;(void)pool;(void)id;return 0;}
static int present(struct intro_world *w) {(void)w;return 0;}
void intro_director_test_inputs(const uint8_t *data,size_t data_size,const uint8_t *inlined,size_t inline_size)
{planes[0]=data;sizes[0]=data_size;planes[1]=inlined;sizes[1]=inline_size;}
const char *intro_director_test_run(uint8_t *ram,uint8_t *video,unsigned reset)
{
    memset(&world,0,sizeof world);memcpy(world.ram,ram,sizeof world.ram);memcpy(world.video,video,sizeof world.video);
    world.read=read_table;world.actor_step=actor;world.present=present;world.title=present;
    if(reset) intro_world_reset_actors(&world);else intro_director_step(&world);
    memcpy(ram,world.ram,sizeof world.ram);memcpy(video,world.video,sizeof world.video);
    return world.error;
}
#include "../actors.h"
const char *intro_actor_test_run(uint8_t *ram,uint8_t *video,unsigned pool,uint16_t id)
{
    memset(&world,0,sizeof world);memcpy(world.ram,ram,sizeof world.ram);memcpy(world.video,video,sizeof world.video);
    world.read=read_table;intro_actor_step_native(&world,pool,id);
    memcpy(ram,world.ram,sizeof world.ram);memcpy(video,world.video,sizeof world.video);
    return world.error;
}
static int core_actor(struct intro_world *w,unsigned pool,uint16_t id)
{return intro_actor_step_native(w,pool,id);}
#ifdef INTRO_TEST_PRESENTATION
#include "../presentation.h"
static struct intro_presentation display;
static int core_present(struct intro_world *w)
{
    *ig(w,0x95)=0; /* Stand-in for the display boundary, as in the oracle. */
    return intro_present_objects(w,&display)<0 ? -1 : 0;
}
size_t intro_core_test_objects(uint8_t *out,size_t capacity)
{
    size_t n=display.count*8+4;
    if(capacity<n) return 0;
    memcpy(out,display.objects,n);return n;
}
#else
static int core_present(struct intro_world *w)
{intro_world_reset_draw_lists(w);return 0;}
#endif
void intro_core_test_start(const uint8_t *ram,const uint8_t *video)
{
    memset(&world,0,sizeof world);memcpy(world.ram,ram,sizeof world.ram);memcpy(world.video,video,sizeof world.video);
    world.read=read_table;world.actor_step=core_actor;world.present=core_present;world.title=intro_title_write;
}
const char *intro_core_test_tick(uint8_t *ram,uint8_t *video)
{
    intro_director_step(&world);
#ifdef INTRO_TEST_PRESENTATION
    intro_present_clear(&world,&display);
#endif
    if(!gw(&world,IG_EXIT)) {intro_fade_tick(&world);intro_world_random(&world);}
    memcpy(ram,world.ram,sizeof world.ram);memcpy(video,world.video,sizeof world.video);return world.error;
}
