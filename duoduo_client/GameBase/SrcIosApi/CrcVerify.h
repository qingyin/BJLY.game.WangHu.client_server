//
//  CrcVerify.h
//  kkddz
//
//  Created by macbook110 on 13-6-5.
//
//

#ifndef kkddz_CrcVerify_h
#define kkddz_CrcVerify_h
/*****************************************************
 ** Name         : crc32.c
 ** Author       : gzshun
 ** Version      : 1.0
 ** Date         : 2011-12
 ** Description  : CRC32 Checking
 ******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFSIZE     1024*4
static unsigned int crc_table[256];

class CrcVerify {
    
    
public:
    
    
    static void usage(void);
    static void init_crc_table(void);
    static unsigned int crc32(unsigned int crc, unsigned char * buffer, unsigned int size);
    static int calc_img_crc(const char * in_file, unsigned int * img_crc);
    static int main(int argc, char **argv);

};


#endif
