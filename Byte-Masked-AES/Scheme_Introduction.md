# Scheme Introduction

This repository implements a first-order byte-wise masked AES, created by the Secure Embedded Systems Research group at Virginia Tech. The underlying SCA protection scheme is indeed the exemplary one in the DPA book\(Chapter 9, page 228\)\[MOP07\]. The skeleton of this code follows [Ermin Sakic's implementation](https://github.com/ermin-sakic/smartcard-aes-fw) of masked AES on smart cards. 

## Scheme discription
Ermin has provided a quite informative figure in [his repository](https://github.com/ermin-sakic/smartcard-aes-fw), explaning the mask flow in the entire encryption procedure. Here we only provide a brief reminder for experienced security engineers: if you are at beginners' level, both Ermin's repository and the DPA book would be a better source.

### Basic AES
The underlying AES implementation here is rather trivial: nonetheless, here we list a few (un-)useful bullet points:
- Sbox implemented as a 256B table
- Byte-wise implementation, no T-table
- All roundkeys are pre-computed in memory, rather than computed online
- Key expansion should be called before encryption, note that this part is not masked
- Multiply 2 and 3 are performed with two seperate look up tables

### Mask Flow
The scheme ultilizes 6 random bytes for each encryption. In the SCALE version, we ask for 6 bytes fresh random bytes from the serial port, which eliminates the need for generating randomness on board. These 6 bytes are used as:

* Mask[0-3]:	The MixColumn input masks (m1,m2,m3,m4). Technically speaking, it is possible to use less random masks for MixColumn. However, as MixColumn combines several bytes together, for byte-wise masking schemes, it becomes impossibly tricky to write a satisfying implementation. Thus, it is indeed suggested in the DPA book that engineers should use 4 seperate bytes to protect each row. Since all 4 bytes for MixColumn come from different rows (protected with different masks), this approach saves a lot of engineering effort with the price of a little additional security margin.

* Mask[4]:	The Sbox input mask m. Used to generate the masked Sbox table.
* Mask[5]:	The Sbox output mask m'. Used to generate the masked Sbox table. Technically speaking, it is also possible to use the same m as the Sbox input mask. However, this means any bit-flip (from registers or memory buses) between the Sbox input and output should be carefully avoided. 

Other masking bytes include:
* Mask[6-9]:	As MixColumn is a linear tranformation, for any 32-bit masked state X=X xor Mask[0-3], MixColumn(X xor Mask[0-3])=MixColumn(X) xor MixColumn(Mask[0-3]). Here the code pre-computed (m1',m2',m3',m4') (a.k.a. Mask[6-9]) as Mask[6-9]=MixColumn(Mask[0-3]) (in function 'calcMixColMask').

The full encryption procedure is as follow:
- init_masking: this function generates the masked Sbox table as well as the masked round key, more specifically,
  - calcMixColMask: this function calculates Mask[6-9] from Mask[0-3]
  - calcSbox_masked: this function generates the masked Sbox table ('Sbox_masked') with mask pair (m,m') (a.k.a. Mask[4] and Mask[5]), as Sm[x]=S[x xor m] xor m'
  - copy_key/init_masked_round_keys: masked each roundkey with (m1',m2',m3',m4') xor m, except for the last round key is masked with (m',m',m',m')
- Initially, the plaintext state is masked with (m1',m2',m3',m4') [Mask tranfer flow: (0,0,0,0)->(m1',m2',m3',m4')]
- After the first AddRoundkey, the state is masked with  [Mask tranfer flow: (m1',m2',m3',m4')->(m,m,m,m)]
- Masked Sbox table look-up tranfers the mask from m to m' [Mask tranfer flow: (m,m,m,m)->(m',m',m',m')]
- ShiftRow does not change the mask
- After ShiftRow, 'remask' the masked state from (m',m',m',m') to (m1,m2,m3,m4) [Mask tranfer flow: (m',m',m',m')->(m1,m2,m3,m4)] to protect MixColumn against undesirable leakage
- MixColumn changes masks from (m1,m2,m3,m4) to (m1',m2',m3',m4') [Mask tranfer flow: (m1,m2,m3,m4)->(m1',m2',m3',m4')]
- Go back to the next AddRoundkey, till the last round
- The last round ends with ShiftRow, with masks (m',m',m',m'), which gets canceled by the last round key with the same mask


## References
\[MOP07\] Mangard S, Oswald E, Popp T. Power analysis attacks: Revealing the secrets of smart cards. Springer Science &amp;Business Media, 2007.