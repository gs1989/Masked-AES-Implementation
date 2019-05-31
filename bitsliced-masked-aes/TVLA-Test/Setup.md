# Measurement Setup
- Scope:				Picoscope 2206B
- Sampling Rate: 		62.5Ma/s
- Number of Samples: 		18400
- Time Interval:			294.4us (First Sbox in the first round)
- Number of Traces:		500K
- Key:				2B 7E 15 16 28 AE D2 A6 AB F7 15 88 09 CF 4F 3C
- SCALE board:			LPC1114 (M0), Working frequency 12MHz, low pass filter on, amplifier on
- Ttest Constant:		2B 7E 15 16 28 AE D2 A6 AB F7 15 88 09 CF 4F 3C

- Ttest Strategy:		Random altering + randomize the plaintext input for each encryption. All 16 blocks encrypt the same plaintext.
- ARM Toolchian:			arm-none-eabi-gcc 5.4.1 20160919

# Leaky Samples
- First Attempt:		13147

# Matlab Plotting script

```
Threshold=4.5;
LeakSamples=sum(abs(BSMaskedAESOneSboxRNGONO1)>4.5);
subplot(121);
hold;
xlim([0 18400]);
ylim([-100 100]);
xlabel('Time [*8ns]');
ylabel('T statistics');
title('1st Order T-test: N=500K, PRNGOn');
plot(BSMaskedAESOneSboxRNGONO1,'b-','LineWidth',1);
x=get(gca,'xlim');
y=4.5;
y1=-4.5;
plot(x,[y y],'k--');
plot(x,[y1 y1],'k--');
hold off;
subplot(122);
hold;
xlim([0 1000]);
ylim([0 100]);
xlabel('Time [*8ns]');
ylabel('Absolute 1st order T statistics');
title('Maximal absolute T-statistic v.s. N');
plot(BSMaskedAESOneSboxRNGONTtrendO1,'b-','LineWidth',1);
x=get(gca,'xlim');
y=4.5;
y1=-4.5;
plot(x,[y y],'k--');
plot(x,[y1 y1],'k--');
hold off;
```
