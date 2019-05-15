
A Brief Introduction
-------------
This is the an "adapted" version of the bytewise masked AES implementation from Virginia Tech (https://github.com/Secure-Embedded-Systems/Masked-AES-Implementation). The goal of all revisions is simply making sure that it can successfully run on more restrictly platforms, a.k.a Dr. Daniel Page's SCALE boards with ARM Cortex M0/M3 cores (https://github.com/danpage/scale). Major changes have been documented as follow. Ideally, such changes SHOULD NOT undermine any security claim; however, we cannot provide any warranty.


Change Log
--------
* Adaption to embedded platforms:
	- #include <scale/scale.h>
	- scale_init() in "byte_mask_aes.c"
	- plaintext recieving /ciphertext sending in "byte_mask_aes.c" 
* Acquisition Setup:
	- Added Trigger control (scale_gpio_wr( SCALE_GPIO_PIN_TRG, false/true)) 

* Other changes:
	- Added a few comments which may make it more comprehensible (or not)


Test Log
--------
* GNU ARM Embedded Toolchain:  arm-none-eabi-gcc (15:5.4.1+svn241155-1) 5.4.1 20160919

* Success on:
	- M0:	LPC1114fn28	(26th April 2019)



