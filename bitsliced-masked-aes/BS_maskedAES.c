

//Following Dan's README.md on https://github.com/danpage/scale-hw/tree/fa15667bf32cdae6ed65c8f2c26735290ff85429
//For M0: export TARGET="${SCALE_HW}/target/lpc1114fn28"
//	  16 blocks Full Encryption (key expansion/slicing cost included)     	:12MHz <134.67ms
//	  Triggered Time (First Sbox in the first Round)	:12MHz <232.5us
//	  Image Size					:12860
//For M3: export TARGET="${SCALE_HW}/target/lpc1313fbd48"
//Make & Program: sudo  make --no-builtin-rules -f ${TARGET}/build/lib/scale.mk BSP="${TARGET}/build" USB="/dev/ttyUSB0" PROJECT="BS_maskedAES" PROJECT_SOURCES="aes_core.h aes_locl.h aes_rng.h key_schedule.h bs.h aes_core.c aes_rng.c bs.c BS_maskedAES.c" clean all program

#include <stdio.h>
#include <stdlib.h>
#define RANDOM  //Use Randomness or not
#include "BS_maskedAES.h"


int main( int argc, char* argv[] ) {

 if( !scale_init( &SCALE_CONF ) ) {
    return -1;
  }
  uint8_t plain[16];
  uint8_t cipher[16];
  uint8_t key[ 16 ] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
                      0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };
    

  //prng initialization
  prng(1);
    while( true ) 
    {

         for( int i = 0; i < 16; i++ ) 
	 {
             	plain[i]=(uint8_t)scale_uart_rd(SCALE_UART_MODE_BLOCKING);
         }
         
         aes_ecb(plain,key, cipher);
         
    	 for( int i = 0; i <16; i++ ) 
	 {
                scale_uart_wr(SCALE_UART_MODE_BLOCKING,( (char)cipher[ i ] ));
    	 }
    }


    return 0;
}
