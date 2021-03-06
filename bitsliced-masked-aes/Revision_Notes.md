
A Brief Introduction
-------------
This is the an "adapted" version of the bit-sliced masked AES implementation from Virginia Tech (https://github.com/Secure-Embedded-Systems/Masked-AES-Implementation). The goal of all revisions is simply making sure that it can successfully run on more restrictly platforms, a.k.a Dr. Daniel Page's SCALE boards with ARM Cortex M0/M3 cores (https://github.com/danpage/scale). Major changes have been documented as follow. Ideally, such changes SHOULD NOT undermine any security claim; however, we cannot provide any warranty.


Change Log
--------
* Adaption to embedded platforms:
	- #include <scale/scale.h>
	- change top module from "main.c" to "BS_maskedAES.c"
	- scale_init() in "BS_maskedAES.c"
	- plaintext recieving /ciphertext sending in "BS_maskedAES.c" 

* Reducing Memory cost:
	- Round keys are generated online, so that only one bit-sliced round key will be stored in RAM 
	- Re-using the buffer of "m_vector" after sharing the plaintext (with fresh randomness), so that the size of "m_vector" can be halved


* Encryption Scheme:
	- Set it to ECB instead of CTR, where all (16) parallel operated blocks use the same plaintext (but with different secret shares) 

* Acquisition Setup:
	- Added Trigger control (scale_gpio_wr( SCALE_GPIO_PIN_TRG, false/true)) 

* Cleanup:
	- Removing codes that do not relate to the tested implementations. Here we only tested the masked encryption on a 32-bit processor, so anything corresponds to the decryption/debugging can be deleted. Note that this is solely for testing purpose though. 

* Other changes:
	- Added a few comments which may make it more comprehensible (or not)


Test Log
--------
GNU ARM Embedded Toolchain:  arm-none-eabi-gcc (15:5.4.1+svn241155-1) 5.4.1 20160919
* Success on:
	- M0:	LPC1114fn28	(26th April 2019)


Effective sources
--------
As this repository is a forked version of the original, we try our best to keep most codes unchanged. Not all sources files in the forder are useful for the SCALE project; more specficially, only the following files are relevent:
- .h files: aes_core.h aes_locl.h aes_rng.h key_schedule.h bs.h BS_maskedAES.h
- .c files: aes_core.c aes_rng.c bs.c BS_maskedAES.c


