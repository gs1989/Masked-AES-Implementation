

#include "elmoasmfunctionsdef.h"
#include "bs.h"
#include "aes_rng.h"

#define BLOCK_NUM 16   //how many AES run in parallel
#define x BLOCK_NUM*16
#define RANDOM
void aesm_ecb_encrypt(uint8_t * outputb, uint8_t * inputb, uint8_t * m_vector, size_t size, uint8_t * key)
{
    uint8_t * maskb = m_vector;//this one has 1024B randomness
    int i,j;
    word_t input_space[BLOCK_SIZE];//128 bits as 128 words

    memset(input_space,0,BS_BLOCK_SIZE);
    //i=0,2,..,30
    for(i = 0; i < 2*size/16; i += 2)  
    {
        memmove(input_space + (i * WORDS_PER_BLOCK), inputb, BLOCK_SIZE/8);//16block of plaintext
        memmove(input_space + (i * WORDS_PER_BLOCK + WORDS_PER_BLOCK), maskb, BLOCK_SIZE/8);//16block of mask

        maskb += BLOCK_SIZE/8;
        //inputb += BLOCK_SIZE/8;

        for (j = 0; j < WORDS_PER_BLOCK; j++)
        {
            input_space[ i * WORDS_PER_BLOCK + j] ^= input_space[ i * WORDS_PER_BLOCK + WORDS_PER_BLOCK + j];//first halve become p^m
        }   //  |p+m|m|p+m|m|... p:certain byte of plaintext  
    }

    #ifdef RANDOM
    //Add new randomness
    //assign rng (16 bytes) to mask (512 bytes)  32 loop
    for (i=0; i<512; i++) //32 block=32*16B=512B Randomness
    {
        //memmove(m_vector+i*16, prng(0), 16);
          randbyte(m_vector+i);
    }
    #else
    for (i=0; i<512; i++) //32 block=32*16B=512B Randomness
    {
        m_vector[i]=0;
    }
    #endif

    bs_cipher(input_space, key, m_vector);//Core encryption

    for(i = 0; i < WORD_SIZE; i += 2)
    {
        for (j = 0; j < WORDS_PER_BLOCK; j++)
        {
            input_space[ i * WORDS_PER_BLOCK + j] ^= input_space[ i * WORDS_PER_BLOCK + WORDS_PER_BLOCK + j];
        }
    }
    //mo: change memmove    c|m|c|m output every the other byte
    memmove(outputb,input_space, 16);
}

void aes_ecb(uint8_t* pt_single,uint8_t* key_vector, uint8_t* ct_single)
{
    uint8_t m_vector[512];
    int i;

    #ifdef RANDOM
    //Add randomness
    //assign rng (16 bytes) to mask (256 bytes)  16 loop
    for (i=0; i<512; i++) //32 block=32*16B=512B Randomness
    {
        //memmove(m_vector+i*16, prng(0), 16);
        randbyte(m_vector+i);
    }
    #else
    for (i=0; i<512; i++) //32 block=32*16B=512B Randomness
    {
        m_vector[i]=0;
    }
    #endif

    aesm_ecb_encrypt(ct_single, pt_single, m_vector, x, key_vector);

}
