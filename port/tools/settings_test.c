/* Isolated generated configuration fixtures. No game, graphics or private inputs. */
#include "settings.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
static void read_case(const char *text,size_t bytes,int ok,int ui,int arcade)
{
    FILE *f=tmpfile();char err[128];struct port_settings s={7,9};int rc;
    assert(f);assert(fwrite(text,1,bytes,f)==bytes);rewind(f);
    rc=port_settings_read(f,&s,err,sizeof err);fclose(f);
    if(ok) { assert(!rc);assert(s.pixel_perfect_ui==ui && s.arcade_video==arcade); }
    else { assert(rc);assert(s.pixel_perfect_ui==7 && s.arcade_video==9);assert(err[0]); }
}
int main(int argc,char **argv)
{
    struct port_settings s;char err[128],longline[700];FILE *f;int a,b;
    const char *invalid[]={"x=true","[other]\nx=true","[experimental]\nnope=true", "[experimental]\narcade_video=yes",
      "[experimental]\narcade_video=true\narcade_video=false", "[experimental]\npixel_perfect_ui=true\ninvalid=false"};
    size_t i;
    assert(argc==2);port_settings_defaults(&s);assert(!s.pixel_perfect_ui && !s.arcade_video);
    s.pixel_perfect_ui=7;s.arcade_video=9;
    assert(port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.pixel_perfect_ui==7 && s.arcade_video==9);
    assert(!port_settings_load(argv[1],&s,1,err,sizeof err));assert(!s.pixel_perfect_ui && !s.arcade_video);
    read_case("",0,1,0,0);
    { const char *text="\xef\xbb\xbf # comment\r\n [experimental] \r\n pixel_perfect_ui = true ; yes\r\narcade_video=false";
      read_case(text,strlen(text),1,1,0); }
    read_case("[experimental]\narcade_video=true",32,1,0,1);
    for(i=0;i<sizeof invalid/sizeof invalid[0];i++)read_case(invalid[i],strlen(invalid[i]),0,0,0);
    memset(longline,'x',sizeof longline);read_case(longline,sizeof longline,0,0,0);
    { const char text[]="[experimental]\narcade_video=true\0ignored";read_case(text,sizeof text-1,0,0,0); }
    for(a=0;a<2;a++)for(b=0;b<2;b++) {
        s.pixel_perfect_ui=a;s.arcade_video=b;
        assert(!port_settings_save(argv[1],&s,err,sizeof err));
        s.pixel_perfect_ui=7;s.arcade_video=9;
        assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.pixel_perfect_ui==a && s.arcade_video==b);
    }
    s.pixel_perfect_ui=2;assert(port_settings_save(argv[1],&s,err,sizeof err));
    assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.pixel_perfect_ui==1 && s.arcade_video==1);
    { size_t n=strlen(argv[1]);char *tmp=malloc(n+5);assert(tmp);memcpy(tmp,argv[1],n);memcpy(tmp+n,".tmp",5);
      f=fopen(tmp,"wb");assert(f);assert(fputs("preserved",f)>=0);assert(!fclose(f));
      s.pixel_perfect_ui=0;s.arcade_video=0;assert(port_settings_save(argv[1],&s,err,sizeof err));
      assert(!port_settings_load(argv[1],&s,0,err,sizeof err));assert(s.pixel_perfect_ui==1 && s.arcade_video==1);
      assert(!remove(tmp));free(tmp); }
    assert(!remove(argv[1]));
    puts("settings: defaults, independent flags, BOM/CRLF/comments, strict rejection/transaction, atomic roundtrip and preservation pass");
    return 0;
}
