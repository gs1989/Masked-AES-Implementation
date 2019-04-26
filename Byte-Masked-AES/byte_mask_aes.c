
#include "byte_mask_aes.h"

#include <stdio.h>
#include <stdlib.h>

//Following Dan's README.md on https://github.com/danpage/scale-hw/tree/fa15667bf32cdae6ed65c8f2c26735290ff85429
//For M0: export TARGET="${SCALE_HW}/target/lpc1114fn28"
//	  Full Encryption without key expansion 	:12MHz <1.381ms
//	  Triggered Time (without mask initialization)	:12MHz <755us
//	  Image Size					:3980
//For M3: export TARGET="${SCALE_HW}/target/lpc1313fbd48"
//Make & Program: sudo  make --no-builtin-rules -f ${TARGET}/build/lib/scale.mk BSP="${TARGET}/build" USB="/dev/ttyUSB0" PROJECT="byte_mask_aes" clean all program


/******************************************************************/
//       AES masked encryption                                   //
/******************************************************************/
void aes128(uint8_t* state)
{	


	init_masking();

	remask(state,Mask[6],Mask[7],Mask[8],Mask[9],0,0,0,0);

    scale_gpio_wr( SCALE_GPIO_PIN_TRG, true  );//Trigger On
        addRoundKey_masked(state, 0);  

	uint8_t i;

    for (i = 1; i <10; i++) {
		subBytes_masked(state);
		shiftRows(state);

		remask(state,Mask[0],Mask[1],Mask[2],Mask[3],Mask[5],Mask[5],Mask[5],Mask[5]); 
		mixColumns(state);
        	addRoundKey_masked(state, i);
    }

			subBytes_masked(state);
			shiftRows(state);

    addRoundKey_masked(state, 10);
    scale_gpio_wr( SCALE_GPIO_PIN_TRG, false  );//Trigger Off
}

int main( int argc, char* argv[] ) {

 if( !scale_init( &SCALE_CONF ) ) {
    return -1;
  }
  uint8_t plain[16];
  uint8_t key[ 16 ] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
                      0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };
    

   
    while( true ) 
    {

         for( int i = 0; i < 16; i++ ) 
	 {
             	plain[i]=(uint8_t)scale_uart_rd(SCALE_UART_MODE_BLOCKING);
         }
 	 for( int i = 0; i < 6; i++ ) 
	 {
             	Mask[i]=(uint8_t)scale_uart_rd(SCALE_UART_MODE_BLOCKING);
         }
         KeyExpansion(key);
         aes128(plain);
    	 for( int i = 0; i <16; i++ ) 
	 {
                scale_uart_wr(SCALE_UART_MODE_BLOCKING,( (char)plain[ i ] ));
    	 }
    }


    return 0;
}
