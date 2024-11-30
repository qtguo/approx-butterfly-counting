#include<iostream>
#include<math.h>
#include<string.h>
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#define MAXNUM 1000000
#define BIT_SIZE 16
#include <iostream>
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

using namespace std;
unsigned int
BOB4(const unsigned char * str, unsigned int len);

unsigned int
BOB1(const unsigned char * str, unsigned int len);

unsigned int
BOB2(const unsigned char * str, unsigned int len);

unsigned int
BOB3(const unsigned char * str, unsigned int len);

unsigned int
PJW(const unsigned char * str, unsigned int len);

unsigned int
SDBW(const unsigned char * str, unsigned int len);

unsigned int
DJB(const unsigned char * str, unsigned int len);

unsigned int
RS(const unsigned char * str, unsigned int len);

unsigned int
JS(const unsigned char * str, unsigned int len);

unsigned int
Hsieh(const unsigned char* str, unsigned int len);

unsigned int
RSHash(const unsigned char* str, unsigned int len);

unsigned int
JSHash(const unsigned char* str, unsigned int len);

unsigned int
BKDR(const unsigned char* str, unsigned int len);

unsigned int
DJBHash(const unsigned char* str, unsigned int len);

unsigned  int
DEKHash(const unsigned char* str, unsigned int len);

unsigned int
APHash(const unsigned char* str, unsigned int len);

unsigned int
CRC32(const unsigned char* str, unsigned int len);

unsigned int
SDBM(const unsigned char *str, unsigned int len);

unsigned int
OCaml(const unsigned char *str, unsigned int len);

unsigned int
SML(const unsigned char *str, unsigned int len);

unsigned int
STL(const unsigned char *str, unsigned int len);

unsigned int
FNV32(const unsigned char *str, unsigned int len);

unsigned int
PJWHash (const unsigned char *str, unsigned int len);

unsigned int
MurmurHash(const unsigned char *key, unsigned int len);

unsigned int
MurmurHash4(const unsigned char *key, unsigned int len);


extern unsigned int (*hfunc[14])(const unsigned char*, unsigned int);

