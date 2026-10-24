/* Project-authored INI subset and atomic save. No assets, GPU or game dependency. */
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif
#include "settings.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define OPEN_NEW(p) _open(p,_O_WRONLY|_O_CREAT|_O_EXCL|_O_BINARY,_S_IREAD|_S_IWRITE)
#define FDOPEN _fdopen
#define CLOSE _close
#define UNLINK _unlink
#else
#include <unistd.h>
#define OPEN_NEW(p) open(p,O_WRONLY|O_CREAT|O_EXCL,0600)
#define FDOPEN fdopen
#define CLOSE close
#define UNLINK unlink
#endif
struct port_settings port_settings;
struct port_settings port_settings_file;
const char *port_settings_path = "settings.ini";

/* The defaults, by the owner's decisions of 2026-10-23 and 2026-10-24 ("turn the arcade intro and pc settings default on"): every
 * setting on but the arcade assets (arcade_assets), which need the player's arcade set and are work in progress, and widescreen
 * (widescreen.md), which is work in progress. The arcade intro needs a pack that the player makes; without one the PS1 opening
 * plays, with a line (main.c).
 * The graphics settings need PsyZ's patched backend: a build without PsyZ (the controls, which open no window) keeps them off, and
 * its filter is nearest, so that its defaults never ask for what it cannot do. The build tools compile this file with PORT_HAVE_PSYZ
 * when they link PsyZ. */
#ifdef PORT_HAVE_PSYZ
#define GRAPHICS_DEFAULT 1
#define FILTER_DEFAULT PORT_FILTER_SHARP_BILINEAR
#else
#define GRAPHICS_DEFAULT 0
#define FILTER_DEFAULT PORT_FILTER_NEAREST
#endif
void port_settings_defaults(struct port_settings *out)
{
    out->pixel_perfect_ui=GRAPHICS_DEFAULT;out->arcade_video=GRAPHICS_DEFAULT;out->center_ui=GRAPHICS_DEFAULT;
    out->fast_loads=1;out->memory_card=1;out->arcade_intro=GRAPHICS_DEFAULT;out->arcade_assets=0;out->settings_menu=GRAPHICS_DEFAULT;out->widescreen=0;
    out->screen=PORT_SCREEN_WINDOW;out->window_w=PORT_WINDOW_DEFAULT_W;out->window_h=PORT_WINDOW_DEFAULT_H;
    out->fullscreen_w=0;out->fullscreen_h=0;out->filter=FILTER_DEFAULT;
}

static const char *const screen_names[]={"window","fullscreen","borderless"};
static const char *const filter_names[]={"nearest","bilinear","sharp_bilinear"};
const char *port_screen_name(int screen){ return screen>=0 && screen<(int)(sizeof screen_names/sizeof *screen_names)?screen_names[screen]:NULL; }
const char *port_filter_name(int filter){ return filter>=0 && filter<(int)(sizeof filter_names/sizeof *filter_names)?filter_names[filter]:NULL; }

static int failure(char *err,size_t size,unsigned line,const char *why)
{
    if(err && size) {
        if(line) snprintf(err,size,"settings line %u: %s",line,why);
        else snprintf(err,size,"settings: %s",why);
    }
    return -1;
}
static char *trim(char *s)
{
    char *end;
    while(isspace((unsigned char)*s))s++;
    end=s+strlen(s);while(end>s && isspace((unsigned char)end[-1]))*--end=0;
    return s;
}
/* The keys: where each belongs and how its value reads. The id is also the key's bit in `seen`. */
enum { SECTION_NONE, SECTION_EXPERIMENTAL, SECTION_DISPLAY };
enum { K_PIXEL_PERFECT_UI, K_ARCADE_VIDEO, K_CENTER_UI, K_FAST_LOADS, K_MEMORY_CARD, K_ARCADE_INTRO, K_ARCADE_ASSETS, K_SETTINGS_MENU, K_WIDESCREEN,
       K_SHARP_BILINEAR /* the legacy spelling of filter, read only */, K_SCREEN, K_WINDOW_SIZE, K_FULLSCREEN_SIZE, K_FILTER };
static const struct { const char *name; int section, id; } keys[]={
    {"pixel_perfect_ui",SECTION_EXPERIMENTAL,K_PIXEL_PERFECT_UI},{"arcade_video",SECTION_EXPERIMENTAL,K_ARCADE_VIDEO},
    {"center_ui",SECTION_EXPERIMENTAL,K_CENTER_UI},{"fast_loads",SECTION_EXPERIMENTAL,K_FAST_LOADS},
    {"memory_card",SECTION_EXPERIMENTAL,K_MEMORY_CARD},{"arcade_intro",SECTION_EXPERIMENTAL,K_ARCADE_INTRO},
    {"arcade_assets",SECTION_EXPERIMENTAL,K_ARCADE_ASSETS},{"settings_menu",SECTION_EXPERIMENTAL,K_SETTINGS_MENU},
    {"widescreen",SECTION_EXPERIMENTAL,K_WIDESCREEN},
    {"sharp_bilinear",SECTION_EXPERIMENTAL,K_SHARP_BILINEAR},
    {"screen",SECTION_DISPLAY,K_SCREEN},{"window_size",SECTION_DISPLAY,K_WINDOW_SIZE},
    {"fullscreen_size",SECTION_DISPLAY,K_FULLSCREEN_SIZE},{"filter",SECTION_DISPLAY,K_FILTER}};

static int size_ok(int w,int h){ return w>=PORT_SIZE_MIN && w<=PORT_SIZE_MAX && h>=PORT_SIZE_MIN && h<=PORT_SIZE_MAX; }

/* One side of a size: decimal digits, no leading zero, at most five of them. Returns the pointer after them, or NULL. */
static const char *side(const char *p,int *out)
{
    int n=0,v=0;
    if(*p<'1' || *p>'9')return NULL;
    while(*p>='0' && *p<='9') { if(++n>5)return NULL;v=v*10+(*p++-'0'); }
    *out=v;return p;
}

/* "WxH": a lowercase x, no spaces inside, each side PORT_SIZE_MIN to PORT_SIZE_MAX. 0 and the sizes, or -1 for a bad form or range. */
static int size_read(const char *value,int *w,int *h)
{
    const char *p=side(value,w);
    if(!p || *p!='x')return -1;
    p=side(p+1,h);
    if(!p || *p || !size_ok(*w,*h))return -1;
    return 0;
}

static int name_index(const char *const *names,int count,const char *value)
{
    int i;
    for(i=0;i<count;i++)if(!strcmp(value,names[i]))return i;
    return -1;
}

static int line_read(char *text,unsigned row,int *section,unsigned *seen,
                     struct port_settings *next,char *err,size_t size)
{
    char *s=text,*comment,*equal,*key,*value;int id=-1,b=0,v,w,h;size_t k;
    if(row==1 && strlen(s)>=3 && (unsigned char)s[0]==0xef &&
       (unsigned char)s[1]==0xbb && (unsigned char)s[2]==0xbf)s+=3;
    comment=strpbrk(s,"#;");if(comment)*comment=0;
    s=trim(s);if(!*s)return 0;
    if(*s=='[') {
        if(!strcmp(s,"[experimental]"))*section=SECTION_EXPERIMENTAL;
        else if(!strcmp(s,"[display]"))*section=SECTION_DISPLAY;
        else return failure(err,size,row,"unknown section");
        return 0;
    }
    if(!*section)return failure(err,size,row,"key outside a section");
    equal=strchr(s,'=');if(!equal)return failure(err,size,row,"expected key = value");
    *equal=0;key=trim(s);value=trim(equal+1);
    for(k=0;k<sizeof keys/sizeof *keys;k++)if(!strcmp(key,keys[k].name))break;
    if(k==sizeof keys/sizeof *keys)return failure(err,size,row,"unknown key");
    if(keys[k].section!=*section)return failure(err,size,row,"key in the wrong section");
    id=keys[k].id;
    if(*seen&(1u<<id))return failure(err,size,row,"duplicate key");
    if((id==K_SHARP_BILINEAR && (*seen&(1u<<K_FILTER))) || (id==K_FILTER && (*seen&(1u<<K_SHARP_BILINEAR))))
        return failure(err,size,row,"sharp_bilinear and filter both given");
    if(id<=K_SHARP_BILINEAR) {
        if(!strcmp(value,"true"))b=1;
        else if(!strcmp(value,"false"))b=0;
        else return failure(err,size,row,"expected true or false");
        switch(id) {
        case K_PIXEL_PERFECT_UI:next->pixel_perfect_ui=b;break;
        case K_ARCADE_VIDEO:next->arcade_video=b;break;
        case K_CENTER_UI:next->center_ui=b;break;
        case K_FAST_LOADS:next->fast_loads=b;break;
        case K_MEMORY_CARD:next->memory_card=b;break;
        case K_ARCADE_INTRO:next->arcade_intro=b;break;
        case K_ARCADE_ASSETS:next->arcade_assets=b;break;
        case K_SETTINGS_MENU:next->settings_menu=b;break;
        case K_WIDESCREEN:next->widescreen=b;break;
        default:next->filter=b?PORT_FILTER_SHARP_BILINEAR:PORT_FILTER_NEAREST;break;   /* sharp_bilinear, the legacy key */
        }
    } else if(id==K_SCREEN) {
        v=name_index(screen_names,(int)(sizeof screen_names/sizeof *screen_names),value);
        if(v<0)return failure(err,size,row,"expected window, fullscreen or borderless");
        next->screen=v;
    } else if(id==K_FILTER) {
        v=name_index(filter_names,(int)(sizeof filter_names/sizeof *filter_names),value);
        if(v<0)return failure(err,size,row,"expected nearest, bilinear or sharp_bilinear");
        next->filter=v;
    } else if(id==K_WINDOW_SIZE) {
        if(size_read(value,&w,&h))return failure(err,size,row,"expected WxH, each side 320 to 16384");
        next->window_w=w;next->window_h=h;
    } else {
        if(!strcmp(value,"desktop"))w=h=0;
        else if(size_read(value,&w,&h))return failure(err,size,row,"expected desktop or WxH, each side 320 to 16384");
        next->fullscreen_w=w;next->fullscreen_h=h;
    }
    *seen|=1u<<id;
    return 0;
}
int port_settings_read(FILE *file,struct port_settings *out,char *err,size_t size)
{
    char line[512];size_t n=0;unsigned row=1,seen=0;int ch,section=0;
    struct port_settings next;port_settings_defaults(&next);
    while((ch=fgetc(file))!=EOF) {
        if(ch==0)return failure(err,size,row,"NUL byte in text");
        if(ch=='\n') {
            line[n]=0;if(line_read(line,row,&section,&seen,&next,err,size))return -1;
            n=0;row++;
        } else {
            if(n+1>=sizeof line)return failure(err,size,row,"line too long");
            line[n++]=(char)ch;
        }
    }
    if(ferror(file))return failure(err,size,row,"read failed");
    if(n) { line[n]=0;if(line_read(line,row,&section,&seen,&next,err,size))return -1; }
    *out=next;return 0;
}
int port_settings_load(const char *path,struct port_settings *out,int missing_ok,char *err,size_t size)
{
    struct port_settings next;FILE *file=fopen(path,"rb");int rc;
    if(!file) {
        if(missing_ok && errno==ENOENT) { port_settings_defaults(out);return 0; }
        return failure(err,size,0,"cannot open file");
    }
    rc=port_settings_read(file,&next,err,size);
    if(fclose(file) && !rc)rc=failure(err,size,0,"close failed");
    if(!rc)*out=next;
    return rc;
}
static int flag_ok(int v){ return v==0 || v==1; }

static int valid(const struct port_settings *v)
{
    return flag_ok(v->pixel_perfect_ui) && flag_ok(v->arcade_video) && flag_ok(v->center_ui) && flag_ok(v->fast_loads) &&
           flag_ok(v->memory_card) && flag_ok(v->arcade_intro) && flag_ok(v->arcade_assets) && flag_ok(v->settings_menu) && flag_ok(v->widescreen) &&
           port_screen_name(v->screen) && port_filter_name(v->filter) && size_ok(v->window_w,v->window_h) &&
           ((v->fullscreen_w==0 && v->fullscreen_h==0) || size_ok(v->fullscreen_w,v->fullscreen_h));
}

/* The file's form of *value, both sections, every key (what save writes and --show-settings prints). 0, or -1 on a write error. */
int port_settings_write(FILE *file,const struct port_settings *value)
{
#define TF(x) ((x)?"true":"false")
    if(fprintf(file,"[experimental]\npixel_perfect_ui = %s\narcade_video = %s\ncenter_ui = %s\nfast_loads = %s\nmemory_card = %s\n"
                    "arcade_intro = %s\narcade_assets = %s\nsettings_menu = %s\nwidescreen = %s\n\n[display]\nscreen = %s\nwindow_size = %dx%d\n",
       TF(value->pixel_perfect_ui),TF(value->arcade_video),TF(value->center_ui),TF(value->fast_loads),TF(value->memory_card),
       TF(value->arcade_intro),TF(value->arcade_assets),TF(value->settings_menu),TF(value->widescreen),port_screen_name(value->screen),value->window_w,value->window_h)<0)return -1;
#undef TF
    if((value->fullscreen_w?fprintf(file,"fullscreen_size = %dx%d\n",value->fullscreen_w,value->fullscreen_h):fprintf(file,"fullscreen_size = desktop\n"))<0)return -1;
    if(fprintf(file,"filter = %s\n",port_filter_name(value->filter))<0)return -1;
    return 0;
}

int port_settings_save(const char *path,const struct port_settings *value,char *err,size_t size)
{
    char *tmp;size_t n=strlen(path);int fd,bad=0;FILE *file;
    if(!valid(value))return failure(err,size,0,"invalid value for save");
    tmp=malloc(n+5);if(!tmp)return failure(err,size,0,"out of memory");
    memcpy(tmp,path,n);memcpy(tmp+n,".tmp",5);
    fd=OPEN_NEW(tmp);
    if(fd<0) { free(tmp);return failure(err,size,0,"cannot create exclusive temporary file"); }
    file=FDOPEN(fd,"wb");
    if(!file) { CLOSE(fd);UNLINK(tmp);free(tmp);return failure(err,size,0,"cannot open temporary stream"); }
    if(port_settings_write(file,value))bad=1;
    if(fclose(file))bad=1;
    if(!bad) {
#ifdef _WIN32
        if(!MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))bad=1;
#else
        if(rename(tmp,path))bad=1;
#endif
    }
    if(bad)UNLINK(tmp);
    free(tmp);return bad?failure(err,size,0,"save failed; prior file kept") : 0;
}
