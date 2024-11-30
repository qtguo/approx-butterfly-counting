#define ALG 2// 0: normal; 1: clr; 2: vertex

#if ALG == 1
static const int clr_num = 4;
#elif ALG == 2
static int mod = 1000;
static int up = 300;
#endif

// #define REC
// #define AMS
// #define MT
#define DBPEDIA_LEFT 172079
#define REUTERS_LEFT 781265
#define LJ_LEFT 3201192
#define GOTTRON_LEFT 
#define DELIUT_LEFT 
//#define TSTMP

const long long left_size = REUTERS_LEFT;