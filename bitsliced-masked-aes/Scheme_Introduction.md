# Scheme Introduction

This repository implements a first-order bit-sliced masked AES, created by the Secure Embedded Systems Research group at Virginia Tech. According to their paper \[YYPYS18\] , the underlying SCA protection scheme is decomposing all non-linear operation to 2-bit AND operation, then masking all ANDs with the ISW scheme \[ISW 03\]. Strictly speaking, the implemented scheme is slightly different from the origianl ISW scheme, although the difference might have no influence on its security at all.

## BitSliced AES
As discribed in the authors' [README.md](README.md), bitslicing is a technique that takes advantage of the internal parrallel. Assume fk\(p\) is one output bit of AES-128 under secret key k and plaintext p, it is not surprising that we can write fk\(\) as a 128-1 boolean function, where each plaintext bit becomes one input variable. Computing fk\(\) in software is all but efficient, as each time we are computing only 1 bit of AES output and all other 31 bits are wasted. However, as most modern processors have larger bit-width (say 32), we can concurrently compute more plaintext blocks to get a better throughput. For instance, if we are computing 32 AES-128 encryptions together, we can "stuff" the same bits of all 32 blocks into one variable, then compute fk\(\) in exactly the same way. As all 32 blocks go through the same encryption procedure, now we can get one bit outputs of all 32 blocks, which significantly improve the throughput. Be aware that this claim only holds when the application allows multiple block encryptions: if we are forced to encrypt one block at a time, a bitsliced implementation will certainly be much slower.     

### BitSlice in this implementation
The bitslice technique used in this implementation is exactly the one mentioned above, which will be denoted as "block-slicing" in this project. Technically speaking, "block-slicing" simply ignored whatever happens inside the cipher, always "slicing" bits from the following blocks. For block ciphers (ECB mode), as all blocks must go through the same encryption procedure, this type of slicing does not require any pre-condition.

On the other hand, in order to achieve the maximal throughput, "block-slicing" has to recieve 32 concurrent encryption blocks to start with, which may make it somehow high-maintenance for some applications. Although you can alway use the same code with all the unneeded blocks set to 0, the encrytion time remains the same, which means the throughput will be much lower.    

## 2-shares Masking Scheme

### Mask &amp Secret shares

Although we are still using the term "mask", for many masking schemes nowdays, there is little distinction between "random masks" and "masked states". Instead, we often use the same terminolgy as in secret sharing scheme, where both the random masks and masked states are simply called secret shares of state x. That is to say, for a 2-shares scheme, each protected intermediate state will be represented by 2 seperate shares, x1 and x2, where x=x1 xor x2 gives the original state. This type of masking is often called "boolean masking", as the link between all shares is simply a boolean xor.

### Boolean Masking for block ciphers
A nice property of boolean masking is that it leads to straightforward implementations for all linear operations. Note that here "linear" means linear on GF(2^n) (i.e. "crypto linear"), rather than linear on real numbers. For any linear operation P, we always have P(x1) xor P(x2) = P(x). This means for the shared version, all we have to do is simply performing the opeartion P seperately on each share. Although this does cause some performance overhead, compared with the non-linear part, such overhead is quite reasonable.

Pretecting the sboxes is usually a critical task for masked implementations. As S(x1) xor S(x2) != S(x), there is no trivial way to do this. Recalls that in the byte-wise masked implementation, one of the mask (m) is selected before each encryption. This means within this encryption process, the first shares of all Sbox look-ups will always be m. In that case, we can pre-computed S'(x2)=S(x2 xor m) xor S(m) as the masked Sbox. The output mask will become \[S(x) xor S(m),S(m)\]: if you want it to be anything else, simply changes the expression of S'. However, applying this approach means you have to start the encryption with generating the masked table S' for a new random mask m. Unless you are willing to store the masked tables for all possible values of m, such overhead is evitable. What makes things worse is such procedure repeatly uses the same value m, which forms a perfect target for horizontal attacks. In other words, this step is not only time-consuming, but also concerned from a security perspective.

Another direction to solve this issue would be, decomposing the Sbox into smaller components, then masking each gadgets seperately. As a consequence, we can have a general way of computing S'(x1,x2)=(y1,y2), where y1 xor y2=S(x1 xor x2). There are different ways to do such decomposition: the most trivial (or perhaps, common) way to do this, would be decomposing the Sbox to 2 input logic gates. Only the non-linear ones (AND/OR/NAND/NOR) need to be treated carefully, as linear ones (NOT/XOR/XNOR) simply apply to each share. Using the algebratic normal form (ANF) would be an easy answer for the decomposition, although no one would use it for AES considering its poor efficiency. Most circuit optimazition techniques actually fit this task quite well, despite that problem itself does not have an determinstic efficient solution. Nonetheless, as the AES Sbox is a fixed 8-bit tables, tons of efforts have been denoted to optimazing its "circuit" (or "decomposition" in this case). [Dr. Rene Peralta's website](http://cs-www.cs.yale.edu/homes/peralta/CircuitStuff/CMT.html) has several several "circuits" for AES: this one used here should be one of them, although it seems the variable names have been updated. Nonetheless, any representation works for the AES Sbox computation, with the cost of only a few gates less (or more) than others.

### ISW masking for AND2
Now the only left question is how to compute a 2-input AND gate on the secret shares. Ishai, Sahai and Wagner proposed a idealised "private circuit" to perform this task \[ISW03\]. For a d-shares scheme, if we are computing (p1,p2,...,pd) \* (q1,q2,...,qd):

- Compute a matrix T as follow
  - if i==j, t(i,i)=pi\*qi
  - if i<j,  t(i,j)= r(i,j), here r(i,j) is a fresh random bit
  - if i>j,  t(i,j)= r(j,i) xor pi\*qj xor pj\*qi
- Compute zi=sum(t(i,j)) for all possible j

Now if we add all zi together, all the randomness will be cancelled out with z=(p1 xor p2 xor ... xor pd)\*(q1 xor q2 xor ... xor qd). Note that you can also use the lower triangle as randomness or sum the column instead of row, it is still the same scheme. Under certain assumptions, the authors had proved that any attack order lower than floor((d-1)/2) cannot learn any information.

For clarity, here we simply write the expression of a 2-share ISW AND2:
```
z1=p1q1 xor r
z2=p2q2 xor p2q1 xor p1q2 xor r
```
Although the authors claimed this implementation is ISW scheme \[YYPYS18\], there is indeed a tiny difference. What is computed here is:
```
z1=p1q2 xor p1q1 xor r
z2=p2q1 xor p2q2 xor r
```
Compared to the ISW expression, this form is more balanced, which helps the bitsliced implementation. In fact, this is not the only bitsliced masked AES implementation that uses this form: [Sebastien Riou's implemetation](https://github.com/sebastien-riou/masked-bit-sliced-aes-128/blob/master/source/secure_aes_pbs.c]) uses the same trick. Personally, I cannot see how this form can introduce any unexpected security flaw. However, I am not an expert in security proof, so do not quote me on this one.

### Bit allocation
To make this clear, I will wrote a small table here to remind us what is stored in each bit of our 32-bit register in this implementation. In the following:
- A,B,C,... as blocks of plaintexts
- A(1) and A(2) are the two shares of the first plaintext block
- A(1)_1 is the LSB of the first share of plaintext A
Now, the i-th bit register in this implementation looks like:

bit |   0   |   1   |   2   |   3   |  ...  |   31   |
    |A(1)_i |A(2)_i |B(1)_i |B(2)_i |  ...  | P(2)_i |      

Note that this is just an exemplary table, endiness in practice could be different from here.

### A few (un-)useful facts
- In the ISW AND2 implementation, there is a pre-computed 8-bit table "bitswap", whose purpose is to swap the first and second shares through a table look-up.

- The randomness is generated by a on-board PRNG based on OpenSSL's AES. Both the seed (IV) and the secret key is fixed in code. Randomness is genrated in a way that close to the OFB mode, where the ciphertext is feedbacked as plaintext. Although the output of this PRNG is not secure in practice (no secure "seed" or "IV"), this does not affect our security evaluation with TVLA (unless the output becomes not uniformally random, which is highly unlikely for AES).

- In theory, this scheme takes 16 128-bit randomness for the plaintext shares. Each Sbox has 32 ANDs, each AND uses 16 bits randomness(both shares should get the same randomness). There are 16 Sboxes within one round, so all 10 rounds use 10\*32\*16\*2B+16\*16B=10496B randomness in total. However, for the Sbox part, the authors choose to limited the fresh randomness to a 256-length random word-table. When it gets to the end of this table, it will simply rewind to the begining of this table and use the same randomness again. In this case, only 16\*16B+256\*2B=768B randomness is actually needed. On the other hand, the randomness will repeat twice within each round of Sbox computation.

## References
\[YYPYS18\] Y. Yao, M. Yang, C. Patrick, B. Yuce and P. Schaumont, "Fault-assisted side-channel analysis of masked implementations," 2018 IEEE International Symposium on Hardware Oriented Security and Trust (HOST), Washington, DC, 2018, pp. 57-64.
\[ISW 03\] Ishai Y., Sahai A., Wagner D. (2003) Private Circuits: Securing Hardware against Probing Attacks. In: Boneh D. (eds) Advances in Cryptology - CRYPTO 2003. CRYPTO 2003. Lecture Notes in Computer Science, vol 2729. Springer, Berlin, Heidelberg.