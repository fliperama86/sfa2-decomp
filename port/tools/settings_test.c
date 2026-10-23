/* Isolated generated configuration fixtures. No game, graphics or private inputs.
 * usage: settings_test SCRATCH_FILE [TEMPLATE]; with TEMPLATE (port/settings.ini) it must load as the build's defaults. */
#include "settings.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
/* the defaults (settings.c): the graphics settings are on in a build with PsyZ, and the memory cards always */
#ifdef PORT_HAVE_PSYZ
#define G 1
#define FILTER PORT_FILTER_SHARP_BILINEAR
#else
#define G 0
#define FILTER PORT_FILTER_NEAREST
#endif
/* A value that no read may produce; a refused read must leave it as it was. */
static struct port_settings sentinel(void)
{
    struct port_settings s;
    s.pixel_perfect_ui=7;s.arcade_video=9;s.center_ui=11;s.fast_loads=13;s.memory_card=15;s.arcade_intro=17;s.arcade_assets=19;s.settings_menu=21;
    s.screen=23;s.window_w=25;s.window_h=27;s.fullscreen_w=29;s.fullscreen_h=31;s.filter=33;
    return s;
}
static struct port_settings dflt(void){ struct port_settings s;port_settings_defaults(&s);return s; }
static int same(const struct port_settings *a,const struct port_settings *b){ return !memcmp(a,b,sizeof *a); }
static int read_text(const char *text,size_t bytes,struct port_settings *s,char *err,size_t size)
{
    FILE *f=tmpfile();int rc;
    assert(f);assert(fwrite(text,1,bytes,f)==bytes);rewind(f);
    rc=port_settings_read(f,s,err,size);fclose(f);return rc;
}
/* text reads to exactly `want` */
static void ok_case(const char *text,size_t bytes,struct port_settings want)
{
    struct port_settings s=sentinel();char err[128];
    assert(!read_text(text,bytes,&s,err,sizeof err));
    assert(same(&s,&want));
}
#define OK(text,want) ok_case(text,strlen(text),want)
/* text is refused, with `why` in the message, and the output is untouched */
static void bad_case(const char *text,size_t bytes,const char *why)
{
    struct port_settings s=sentinel(),keep=s;char err[128]="";
    if(!read_text(text,bytes,&s,err,sizeof err)) { fprintf(stderr,"accepted: %s\n",text);abort(); }
    assert(same(&s,&keep));assert(err[0]);
    if(why && !strstr(err,why)) { fprintf(stderr,"refused with \"%s\", wanted \"%s\" in: %s\n",err,why,text);abort(); }
}
#define BAD(text,why) bad_case(text,strlen(text),why)
static struct port_settings with_filter(int f){ struct port_settings s=dflt();s.filter=f;return s; }
static struct port_settings with_window(int w,int h){ struct port_settings s=dflt();s.window_w=w;s.window_h=h;return s; }
static struct port_settings with_fullscreen(int w,int h){ struct port_settings s=dflt();s.fullscreen_w=w;s.fullscreen_h=h;return s; }
static char *slurp(const char *path,char *buf,size_t size)
{
    FILE *f=fopen(path,"rb");size_t n;
    assert(f);n=fread(buf,1,size-1,f);buf[n]=0;fclose(f);return buf;
}
int main(int argc,char **argv)
{
    struct port_settings s,d,w;char err[128],longline[700],text[1024],golden[512];FILE *f;int a,i,j,k;
    const char *invalid[]={"x=true","[other]\nx=true","[experimental]\nnope=true","[experimental]\narcade_video=yes",
      "[experimental]\narcade_video=true\narcade_video=false","[experimental]\npixel_perfect_ui=true\ninvalid=false","[experimental]\ncenter_ui=yes","[experimental]\ncenter_ui=true\ncenter_ui=false",
      "[experimental]\nfast_loads=1","[experimental]\nfast_loads=true\nfast_loads=false",
      "[experimental]\nmemory_card=1","[experimental]\nmemory_card=true\nmemory_card=false",
      "[experimental]\narcade_intro=yes","[experimental]\narcade_intro=true\narcade_intro=false",
      "[experimental]\nsharp_bilinear=1","[experimental]\nsharp_bilinear=true\nsharp_bilinear=false",
      "[experimental]\narcade_assets=on","[experimental]\narcade_assets=false\narcade_assets=false",
      "[experimental]\nsettings_menu=on","[experimental]\nsettings_menu=1","[experimental]\nsettings_menu=true\nsettings_menu=false",
      "[experimental]\nsettings_menu","[experimental]\n=true","[Experimental]\nfast_loads=true","[experimental\nfast_loads=true","[]\n",
      "[display]\nscreen=Window","[display]\nscreen=full","[display]\nscreen=","[display]\nscreen=window\nscreen=window",
      "[display]\nfilter=linear","[display]\nfilter=Nearest","[display]\nfilter=sharp-bilinear","[display]\nfilter=true","[display]\nfilter=nearest\nfilter=bilinear",
      "[display]\nwindow_size=1280X960","[display]\nwindow_size=01280x960","[display]\nwindow_size=1280x0960","[display]\nwindow_size=1280x",
      "[display]\nwindow_size=x960","[display]\nwindow_size=1280 x 960","[display]\nwindow_size=1280 x960","[display]\nwindow_size=1280x 960",
      "[display]\nwindow_size=desktop","[display]\nwindow_size=","[display]\nwindow_size=1280","[display]\nwindow_size=1280x960x1",
      "[display]\nwindow_size=-1280x960","[display]\nwindow_size=+1280x960","[display]\nwindow_size=1280x960.5","[display]\nwindow_size=0x0",
      "[display]\nwindow_size=319x960","[display]\nwindow_size=1280x319","[display]\nwindow_size=16385x960","[display]\nwindow_size=1280x16385",
      "[display]\nwindow_size=99999x960","[display]\nwindow_size=123456x960","[display]\nwindow_size=99999999999999999999x960",
      "[display]\nwindow_size=1280x960\nwindow_size=1280x960",
      "[display]\nfullscreen_size=Desktop","[display]\nfullscreen_size=1280X960","[display]\nfullscreen_size=0x0","[display]\nfullscreen_size=01920x1080",
      "[display]\nfullscreen_size=1920x","[display]\nfullscreen_size=x1080","[display]\nfullscreen_size=1920 x 1080","[display]\nfullscreen_size=319x1080",
      "[display]\nfullscreen_size=16385x1080","[display]\nfullscreen_size=","[display]\nfullscreen_size=desktop\nfullscreen_size=desktop",
      "[display]\nfast_loads=true","[display]\nsharp_bilinear=true","[experimental]\nscreen=window","[experimental]\nfilter=nearest",
      "[experimental]\nwindow_size=1280x960","[experimental]\nfullscreen_size=desktop","[experimental]\nfast_loads=true\n[display]\nmemory_card=true",
      "screen=window","[display]\nfilter=nearest\n[experimental]\nscreen=window"};
    size_t u;
    assert(argc>=2);
    /* defaults */
    d=dflt();
    assert(d.pixel_perfect_ui==G && d.arcade_video==G && d.center_ui==G && d.fast_loads==1 && d.memory_card==1 && d.arcade_intro==0 && d.arcade_assets==0 && d.settings_menu==0);
    assert(d.screen==PORT_SCREEN_WINDOW && d.window_w==1280 && d.window_h==960 && d.fullscreen_w==0 && d.fullscreen_h==0 && d.filter==FILTER);
    assert(PORT_WINDOW_DEFAULT_W==1280 && PORT_WINDOW_DEFAULT_H==960 && PORT_SIZE_MIN==320 && PORT_SIZE_MAX==16384);
    /* names */
    assert(!strcmp(port_screen_name(PORT_SCREEN_WINDOW),"window") && !strcmp(port_screen_name(PORT_SCREEN_FULLSCREEN),"fullscreen") && !strcmp(port_screen_name(PORT_SCREEN_BORDERLESS),"borderless"));
    assert(!strcmp(port_filter_name(PORT_FILTER_NEAREST),"nearest") && !strcmp(port_filter_name(PORT_FILTER_BILINEAR),"bilinear") && !strcmp(port_filter_name(PORT_FILTER_SHARP_BILINEAR),"sharp_bilinear"));
    assert(!port_screen_name(-1) && !port_screen_name(3) && !port_filter_name(-1) && !port_filter_name(3));
    /* the globals */
    assert(!strcmp(port_settings_path,"settings.ini"));
    /* load: a missing file */
    s=sentinel();
    assert(port_settings_load(argv[1],&s,0,err,sizeof err));{ struct port_settings keep=sentinel();assert(same(&s,&keep)); }
    assert(!port_settings_load(argv[1],&s,1,err,sizeof err));assert(same(&s,&d));
    /* the template */
    if(argc>=3) {
        struct port_settings t=sentinel();
#ifdef PORT_HAVE_PSYZ
        assert(!port_settings_load(argv[2],&t,0,err,sizeof err));assert(same(&t,&d));
#else
        assert(!port_settings_load(argv[2],&t,0,err,sizeof err));
        t.filter=PORT_FILTER_NEAREST;t.pixel_perfect_ui=t.arcade_video=t.center_ui=0;assert(same(&t,&d));   /* the template is the PsyZ build's defaults */
#endif
    }
    /* reads */
    OK("",d);
    OK("[experimental]\n",d);OK("[display]\n",d);OK("\n\n",d);
    { struct port_settings x=d;x.arcade_assets=1;OK("[experimental]\narcade_assets=true",x); }
    { const char *t="\xef\xbb\xbf # comment\r\n [experimental] \r\n pixel_perfect_ui = true ; yes\r\narcade_video=false";
      struct port_settings x=d;x.pixel_perfect_ui=1;x.arcade_video=0;OK(t,x); }
    { struct port_settings x=d;x.arcade_video=1;OK("[experimental]\narcade_video=true",x); }
    { struct port_settings x=d;x.center_ui=1;OK("[experimental]\ncenter_ui=true",x); }
    { struct port_settings x=d;x.fast_loads=0;OK("[experimental]\nfast_loads=false",x); }
    { struct port_settings x=d;x.memory_card=0;OK("[experimental]\nmemory_card=false",x); }
    { struct port_settings x=d;x.arcade_intro=1;OK("[experimental]\narcade_intro=true",x); }
    { struct port_settings x=d;x.settings_menu=1;OK("[experimental]\nsettings_menu=true",x);OK("[experimental]\nsettings_menu = true # on",x); }
    { struct port_settings x=d;x.settings_menu=1;x.fast_loads=0;OK("[experimental]\nsettings_menu=true\nfast_loads=false\n",x); }
    /* display keys */
    { struct port_settings x=d;x.screen=PORT_SCREEN_FULLSCREEN;OK("[display]\nscreen = fullscreen",x);
      x.screen=PORT_SCREEN_BORDERLESS;OK("[display]\nscreen=borderless\n",x);
      x.screen=PORT_SCREEN_WINDOW;OK("[display]\nscreen = window  # the default",x); }
    OK("[display]\nfilter = nearest",with_filter(PORT_FILTER_NEAREST));
    OK("[display]\nfilter=bilinear",with_filter(PORT_FILTER_BILINEAR));
    OK("[display]\nfilter=sharp_bilinear",with_filter(PORT_FILTER_SHARP_BILINEAR));
    OK("[display]\nwindow_size = 640x480 ; comment",with_window(640,480));
    OK("[display]\nwindow_size=320x320",with_window(320,320));
    OK("[display]\nwindow_size=16384x16384",with_window(16384,16384));
    OK("[display]\nwindow_size=1920x1440\r\n",with_window(1920,1440));
    OK("[display]\nfullscreen_size=desktop",with_fullscreen(0,0));
    OK("[display]\nfullscreen_size = 1920x1080",with_fullscreen(1920,1080));
    OK("[display]\nfullscreen_size=320x16384",with_fullscreen(320,16384));
    /* sections in any order and repeated; one file with both */
    { struct port_settings x=d;x.fast_loads=0;x.filter=PORT_FILTER_BILINEAR;x.window_w=800;x.window_h=600;
      OK("[display]\nfilter=bilinear\n[experimental]\nfast_loads=false\n[display]\nwindow_size=800x600\n",x); }
    /* the legacy key, both ways */
    OK("[experimental]\nsharp_bilinear=true",with_filter(PORT_FILTER_SHARP_BILINEAR));
    OK("[experimental]\nsharp_bilinear=false",with_filter(PORT_FILTER_NEAREST));
    { struct port_settings x=with_filter(PORT_FILTER_NEAREST);x.fast_loads=0;OK("[experimental]\nsharp_bilinear=false\nfast_loads=false",x); }
    { struct port_settings x=with_filter(PORT_FILTER_SHARP_BILINEAR);x.window_w=640;x.window_h=480;OK("[experimental]\nsharp_bilinear=true\n[display]\nwindow_size=640x480",x); }
    BAD("[experimental]\nsharp_bilinear=true\n[display]\nfilter=sharp_bilinear","both given");
    BAD("[display]\nfilter=nearest\n[experimental]\nsharp_bilinear=false","both given");
    BAD("[experimental]\nsharp_bilinear=false\n[display]\nfilter=nearest","both given");
    BAD("[display]\nfilter=bilinear\n[experimental]\nsharp_bilinear=true","both given");
    /* named refusals */
    BAD("[display]\nwindow_size=1280X960","WxH");
    BAD("[experimental]\nscreen=window","wrong section");
    BAD("[display]\nfast_loads=true","wrong section");
    BAD("[display]\nsharp_bilinear=true","wrong section");
    BAD("[display]\nscreen=full","window, fullscreen or borderless");
    BAD("[display]\nfilter=linear","nearest, bilinear or sharp_bilinear");
    BAD("[experimental]\nfast_loads=true\nfast_loads=true","duplicate");
    BAD("[experimental]\nnope=true","unknown key");
    BAD("[nope]\n","unknown section");
    BAD("[display]\nwindow_size=1280x960\nwindow_size=1280x960","line 3");
    for(u=0;u<sizeof invalid/sizeof invalid[0];u++)bad_case(invalid[u],strlen(invalid[u]),NULL);
    memset(longline,'x',sizeof longline);bad_case(longline,sizeof longline,"too long");
    { const char t[]="[experimental]\narcade_video=true\0ignored";bad_case(t,sizeof t-1,"NUL"); }
    { const char t[]="[display]\nscreen=window\0";bad_case(t,sizeof t-1,"NUL"); }
    { const char t[]="[display]\nfilter=nearest # \0";bad_case(t,sizeof t-1,"NUL"); }
    /* a refusal after a valid part leaves the output as it was (transactional) */
    BAD("[display]\nfilter=nearest\nwindow_size=1280X960\n","WxH");
    /* save: golden bytes of the defaults of this build, then every field round-trips */
    assert(!port_settings_save(argv[1],&d,err,sizeof err));
    slurp(argv[1],text,sizeof text);
    snprintf(golden,sizeof golden,"[experimental]\npixel_perfect_ui = %s\narcade_video = %s\ncenter_ui = %s\nfast_loads = true\nmemory_card = true\narcade_intro = false\n"
        "arcade_assets = false\nsettings_menu = false\n\n[display]\nscreen = window\nwindow_size = 1280x960\nfullscreen_size = desktop\nfilter = %s\n",
        G?"true":"false",G?"true":"false",G?"true":"false",G?"sharp_bilinear":"nearest");
    if(strcmp(text,golden)) { fprintf(stderr,"saved:\n%s\nwanted:\n%s\n",text,golden);abort(); }
    assert(!strstr(text,"sharp_bilinear =") );
    for(a=0;a<256;a++) {
        s=dflt();
        s.pixel_perfect_ui=a&1;s.arcade_video=a>>1&1;s.center_ui=a>>2&1;s.fast_loads=a>>3&1;s.memory_card=a>>4&1;s.arcade_intro=a>>5&1;s.arcade_assets=a>>6&1;s.settings_menu=a>>7&1;
        s.screen=a%3;s.filter=(a/3)%3;
        if(a&1) { s.window_w=320+a*61;s.window_h=PORT_SIZE_MAX-a; }
        if(a&2) { s.fullscreen_w=PORT_SIZE_MAX-a*7;s.fullscreen_h=320+a; }
        assert(!port_settings_save(argv[1],&s,err,sizeof err));
        w=sentinel();assert(!port_settings_load(argv[1],&w,0,err,sizeof err));
        assert(same(&w,&s));
    }
    /* every screen x filter x fullscreen form, at the edges of the sizes */
    for(i=0;i<3;i++)for(j=0;j<3;j++)for(k=0;k<4;k++) {
        s=dflt();s.screen=i;s.filter=j;
        s.window_w=k&1?PORT_SIZE_MIN:PORT_SIZE_MAX;s.window_h=k&1?PORT_SIZE_MAX:PORT_SIZE_MIN;
        if(k&2) { s.fullscreen_w=PORT_SIZE_MIN;s.fullscreen_h=PORT_SIZE_MIN; }
        assert(!port_settings_save(argv[1],&s,err,sizeof err));
        w=sentinel();assert(!port_settings_load(argv[1],&w,0,err,sizeof err));assert(same(&w,&s));
    }
    /* save refuses every field out of range, keeps the prior file, and says why */
    s=dflt();s.window_w=1111;assert(!port_settings_save(argv[1],&s,err,sizeof err));
    slurp(argv[1],text,sizeof text);
    {
        int *fields[]={&s.pixel_perfect_ui,&s.arcade_video,&s.center_ui,&s.fast_loads,&s.memory_card,&s.arcade_intro,&s.arcade_assets,&s.settings_menu};
        for(u=0;u<sizeof fields/sizeof *fields;u++) {
            int bads[]={2,-1,7};size_t b;
            for(b=0;b<3;b++) {
                s=dflt();s.window_w=1111;*fields[u]=bads[b];
                assert(port_settings_save(argv[1],&s,err,sizeof err));assert(err[0]);
                { char again[1024];assert(!strcmp(slurp(argv[1],again,sizeof again),text)); }
            }
        }
    }
    {
        struct port_settings badv[16];int c=0;
        s=dflt();s.screen=-1;badv[c++]=s;
        s=dflt();s.screen=3;badv[c++]=s;
        s=dflt();s.filter=-1;badv[c++]=s;
        s=dflt();s.filter=3;badv[c++]=s;
        s=dflt();s.window_w=319;badv[c++]=s;
        s=dflt();s.window_w=16385;badv[c++]=s;
        s=dflt();s.window_h=319;badv[c++]=s;
        s=dflt();s.window_h=16385;badv[c++]=s;
        s=dflt();s.window_w=0;s.window_h=0;badv[c++]=s;
        s=dflt();s.fullscreen_w=1920;s.fullscreen_h=0;badv[c++]=s;   /* neither 0, 0 nor in range */
        s=dflt();s.fullscreen_w=0;s.fullscreen_h=1080;badv[c++]=s;
        s=dflt();s.fullscreen_w=319;s.fullscreen_h=1080;badv[c++]=s;
        s=dflt();s.fullscreen_w=1920;s.fullscreen_h=16385;badv[c++]=s;
        s=dflt();s.fullscreen_w=-1;s.fullscreen_h=-1;badv[c++]=s;
        for(i=0;i<c;i++) {
            char again[1024];
            assert(port_settings_save(argv[1],&badv[i],err,sizeof err));assert(err[0]);
            assert(!strcmp(slurp(argv[1],again,sizeof again),text));
        }
    }
    /* a file saved reads back with the legacy key absent */
    s=dflt();assert(!port_settings_save(argv[1],&s,err,sizeof err));assert(!strstr(slurp(argv[1],text,sizeof text),"sharp_bilinear ="));
    /* a failed save keeps the old file: an exclusive temporary file that exists already refuses it */
    { size_t m=strlen(argv[1]);char *tmp=malloc(m+5);struct port_settings x=dflt(),y;assert(tmp);memcpy(tmp,argv[1],m);memcpy(tmp+m,".tmp",5);
      x.fast_loads=0;assert(!port_settings_save(argv[1],&x,err,sizeof err));
      f=fopen(tmp,"wb");assert(f);assert(fputs("preserved",f)>=0);assert(!fclose(f));
      x.fast_loads=1;x.filter=PORT_FILTER_BILINEAR;assert(port_settings_save(argv[1],&x,err,sizeof err));
      y=sentinel();assert(!port_settings_load(argv[1],&y,0,err,sizeof err));assert(y.fast_loads==0 && y.filter==d.filter);
      assert(!strcmp(slurp(tmp,text,sizeof text),"preserved"));
      assert(!remove(tmp));free(tmp); }
    assert(!remove(argv[1]));
    puts("settings: defaults, [experimental] and [display] keys, sizes, legacy sharp_bilinear, BOM/CRLF/comments, strict rejection/transaction, validated atomic roundtrip and preservation pass");
    return 0;
}
