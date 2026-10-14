/* Isolated generated configuration fixtures. No game, graphics or private inputs. */
#include "settings.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
static void read_case(const char *text,size_t bytes,int ok,int ui,int arcade,int center,int fast,int card,int sharp)
{
    FILE *f=tmpfile();char err[128];struct port_settings s={7,9,11,13,15,17,19};int rc;
    assert(f);assert(fwrite(text,1,bytes,f)==bytes);rewind(f);
    rc=port_settings_read(f,&s,err,sizeof err);fclose(f);
    if(ok) { assert(!rc);assert(s.pixel_perfect_ui==ui && s.arcade_video==arcade && s.center_ui==center && s.fast_loads==fast && s.memory_card==card && s.sharp_bilinear==sharp); }
    else { assert(rc);assert(s.pixel_perfect_ui==7 && s.arcade_video==9 && s.center_ui==11 && s.fast_loads==13 && s.memory_card==15 && s.sharp_bilinear==17);assert(err[0]); }
}
int main(int argc,char **argv)
{
    struct port_settings s;char err[128],longline[700];FILE *f;int a,b,c,d,e,g,h;
    const char *invalid[]={"x=true","[other]\nx=true","[experimental]\nnope=true", "[experimental]\narcade_video=yes",
      "[experimental]\narcade_video=true\narcade_video=false", "[experimental]\npixel_perfect_ui=true\ninvalid=false", "[experimental]\ncenter_ui=yes", "[experimental]\ncenter_ui=true\ncenter_ui=false",
      "[experimental]\nfast_loads=1", "[experimental]\nfast_loads=true\nfast_loads=false",
      "[experimental]\nmemory_card=1", "[experimental]\nmemory_card=true\nmemory_card=false",
      "[experimental]\narcade_intro=yes", "[experimental]\narcade_intro=true\narcade_intro=false",
      "[experimental]\nsharp_bilinear=1", "[experimental]\nsharp_bilinear=true\nsharp_bilinear=false"};
    size_t i;
    assert(argc==2);port_settings_defaults(&s);assert(!s.pixel_perfect_ui && !s.arcade_video && !s.center_ui && s.fast_loads && !s.memory_card && !s.sharp_bilinear && !s.arcade_intro);
    s.pixel_perfect_ui=7;s.arcade_video=9;s.center_ui=11;s.fast_loads=13;s.memory_card=15;s.sharp_bilinear=17;
    assert(port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.pixel_perfect_ui==7 && s.arcade_video==9 && s.center_ui==11 && s.fast_loads==13 && s.memory_card==15 && s.sharp_bilinear==17);
    assert(!port_settings_load(argv[1],&s,1,err,sizeof err));assert(!s.pixel_perfect_ui && !s.arcade_video && !s.center_ui && s.fast_loads && !s.memory_card && !s.sharp_bilinear && !s.arcade_intro);
    read_case("",0,1,0,0,0,1,0,0);
    { const char *text="\xef\xbb\xbf # comment\r\n [experimental] \r\n pixel_perfect_ui = true ; yes\r\narcade_video=false";
      read_case(text,strlen(text),1,1,0,0,1,0,0); }
    read_case("[experimental]\narcade_video=true",32,1,0,1,0,1,0,0);
    read_case("[experimental]\ncenter_ui=true",29,1,0,0,1,1,0,0);
    read_case("[experimental]\nfast_loads=true",30,1,0,0,0,1,0,0);
    read_case("[experimental]\nfast_loads=false",31,1,0,0,0,0,0,0);
    read_case("[experimental]\nmemory_card=true",31,1,0,0,0,1,1,0);
    read_case("[experimental]\nsharp_bilinear=true",34,1,0,0,0,1,0,1);
    read_case("[experimental]\nsharp_bilinear=false\nfast_loads=false",52,1,0,0,0,0,0,0);
    for(i=0;i<sizeof invalid/sizeof invalid[0];i++)read_case(invalid[i],strlen(invalid[i]),0,0,0,0,0,0,0);
    memset(longline,'x',sizeof longline);read_case(longline,sizeof longline,0,0,0,0,0,0,0);
    { const char text[]="[experimental]\narcade_video=true\0ignored";read_case(text,sizeof text-1,0,0,0,0,0,0,0); }
    for(a=0;a<2;a++)for(b=0;b<2;b++)for(c=0;c<2;c++)for(d=0;d<2;d++)for(e=0;e<2;e++)for(g=0;g<2;g++)for(h=0;h<2;h++) {
        s.pixel_perfect_ui=a;s.arcade_video=b;s.center_ui=c;s.fast_loads=d;s.memory_card=e;s.sharp_bilinear=g;s.arcade_intro=h;
        assert(!port_settings_save(argv[1],&s,err,sizeof err));
        s.pixel_perfect_ui=7;s.arcade_video=9;s.center_ui=11;s.fast_loads=13;s.memory_card=15;s.sharp_bilinear=17;
        assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.pixel_perfect_ui==a && s.arcade_video==b && s.center_ui==c && s.fast_loads==d && s.memory_card==e && s.sharp_bilinear==g && s.arcade_intro==h);
    }
    s.arcade_intro=2;assert(port_settings_save(argv[1],&s,err,sizeof err));
    assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.arcade_intro==1);
    s.pixel_perfect_ui=2;assert(port_settings_save(argv[1],&s,err,sizeof err));
    assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.pixel_perfect_ui==1 && s.arcade_video==1 && s.center_ui==1 && s.fast_loads==1);
    s.pixel_perfect_ui=1;s.center_ui=2;assert(port_settings_save(argv[1],&s,err,sizeof err));
    assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.center_ui==1);
    s.center_ui=1;s.fast_loads=2;assert(port_settings_save(argv[1],&s,err,sizeof err));
    assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.fast_loads==1);
    s.fast_loads=1;s.memory_card=2;assert(port_settings_save(argv[1],&s,err,sizeof err));
    assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.memory_card==1);
    s.memory_card=1;s.sharp_bilinear=2;assert(port_settings_save(argv[1],&s,err,sizeof err));
    assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.sharp_bilinear==1);
    { size_t n=strlen(argv[1]);char *tmp=malloc(n+5);assert(tmp);memcpy(tmp,argv[1],n);memcpy(tmp+n,".tmp",5);
      f=fopen(tmp,"wb");assert(f);assert(fputs("preserved",f)>=0);assert(!fclose(f));
      s.pixel_perfect_ui=0;s.arcade_video=0;s.center_ui=0;s.fast_loads=0;s.memory_card=0;s.sharp_bilinear=0;assert(port_settings_save(argv[1],&s,err,sizeof err));
      assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.pixel_perfect_ui==1 && s.arcade_video==1 && s.center_ui==1 && s.fast_loads==1 && s.memory_card==1 && s.sharp_bilinear==1);
      assert(!remove(tmp));free(tmp); }
    assert(!remove(argv[1]));
    puts("settings: defaults, independent flags, BOM/CRLF/comments, strict rejection/transaction, atomic roundtrip and preservation pass");
    return 0;
}
