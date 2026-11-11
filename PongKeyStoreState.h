/* SPDX-License-Identifier: BSD-2-Clause-Patent */
#pragma once
#include <stdint.h>
#include <stddef.h>
/* Shared state codec from upstream NativeKeyStoreEvents.cpp. The actual
 * board service supplies its state; this header publishes no second AKS
 * provider and does not replace existing wrapped key formats. */
namespace PongKeyStoreState {
struct Writer {
    uint8_t *p;size_t cap,n=0;bool ok=true;
    void put(uint8_t b){if(n<cap)p[n++]=b;else ok=false;}
    void len(size_t length){if(length<128)put(uint8_t(length));else{put(0x81);put(uint8_t(length));}}
};
static size_t integer(uint64_t value,uint8_t out[9]) {
    uint8_t raw[8];for(unsigned i=0;i<8;++i)raw[i]=uint8_t(value>>(56-8*i));
    unsigned first=0;while(first<7 && !raw[first])++first;
    size_t n=0;if(raw[first]&128)out[n++]=0;
    while(first<8)out[n++]=raw[first++];return n;
}
static size_t encode(uint8_t *out,size_t capacity,uint32_t flags,uint32_t lock) {
    if(!out)return 0;
    struct Entry{const char *key;uint64_t value;};
    const Entry entries[]={{"ss",flags},{"sls",lock},{"sb",0},{"sfa",0},{"sgs",0},
        {"sas",0},{"sgpe",0},{"srcd",0},{"sms",0},{"bh",0}};
    size_t total=0;
    for(const auto &e:entries){size_t k=0;while(e.key[k])++k;uint8_t v[9];total+=6+k+integer(e.value,v);}
    Writer w{out,capacity};w.put(0x31);w.len(total);
    for(const auto &e:entries){size_t k=0;while(e.key[k])++k;uint8_t v[9];const size_t n=integer(e.value,v);
        w.put(0x30);w.len(4+k+n);w.put(0x0c);w.put(uint8_t(k));
        for(size_t i=0;i<k;++i)w.put(e.key[i]);w.put(0x02);w.put(uint8_t(n));
        for(size_t i=0;i<n;++i)w.put(v[i]);}
    return w.ok?w.n:0;
}
}
