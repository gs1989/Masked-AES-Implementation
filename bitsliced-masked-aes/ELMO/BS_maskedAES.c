//Trivial implementation of byte-wise AES masking scheme from the DPA Book
//For ELMO: 
//Triggered Perfiod=first round
//Instruction Count=2278
//./elmo ${DIRECTORY}/ELMO/BS_maskedAES.bin -autotvla 10000
#define NOTRACES 20000

#define AUTON //Automaticially decide the number of traces N
#include <stdio.h>
#include <stdlib.h>
#include "elmoasmfunctionsdef.h"
#include "BS_maskedAES.h"


int main() 
{
    uint8_t *input, *output, *key;
    uint32_t i,j;
    uint32_t N;

    key = malloc(16*sizeof(uint8_t));
    input = malloc(16*sizeof(uint8_t));
    output = malloc(16*sizeof(uint8_t));

    static const uint8_t fixedkey[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};
    static const uint8_t fixedinput[16] = {0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90};
    
    #ifdef AUTON
     LoadNForTVLA(&N);//Get N from ELMO
    #else
     N=NOTRACES/2;
    #endif



    for(j=0;j<16;j++)
    {
        output[j] = 0x00;
    }

    for(i=0;i<2*N;i++)
    {
        
	//Switch from Fix to Random (Only do it once)
        if(i==N)
        {
		for(j=0;j<16;j++)
           	  output[j] = 0x00;
	}
       //Fix v.s. Random Plaintexts
        for(j=0;j<16;j++)
       {
            if(i<N)
            {
		input[j] = fixedinput[j]; // Fixed Traces
                key[j]=fixedkey[j];
	    }
	    else
            {
		input[j] = output[j]; // Random Traces
                key[j]=fixedkey[j];
	    }
        }
       //starttrigger();
       //endtrigger();
    aes_ecb(input,key, output);
        //Encryption  
        

    }

    endprogram();

    return 0;
}
