#include <string.h>
#include <stdlib.h>
#include <time.h>


#if defined(WIN32) && !defined(__cplusplus)

#define inline __inline

#endif

#if defined(WIN32)  && !defined(__MINGW32__) && !defined(__CYGWIN__)      
#define  CONFIG_WIN32      
#endif      
#if defined(WIN32) && !defined(__MINGW32__)  && !defined(__CYGWIN__) && !defined(EMULATE_INTTYPES)      
#define EMULATE_INTTYPES      
#endif      
#ifndef EMULATE_INTTYPES      
#include  <inttypes.h>     
#else
typedef signed char int8_t;      
typedef  signed short int16_t;      
    typedef signed int   int32_t;      
    typedef  unsigned char  uint8_t;      
    typedef unsigned short uint16_t;      
    typedef  unsigned int   uint32_t;      
#ifdef CONFIG_WIN32      
        typedef signed  __int64   int64_t;      
        typedef unsigned __int64 uint64_t;      
#else /*  other OS */      
        typedef signed long long   int64_t;      
        typedef  unsigned long long uint64_t;      
#endif /* other OS */      
#endif /*  EMULATE_INTTYPES */

/**参数说明：
input_data:64位数据
input_size:传入数据长度
output_data:存储相应返回的16K数据
output_size:输出数据长度.**/
typedef int (* encrypt_data)(uint8_t *input_data, uint32_t input_size,  uint8_t *output_data, uint32_t output_size);