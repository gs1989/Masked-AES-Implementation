# Measurement Setup
-Scope:			LeCroy WavePro 760Zi-A

-Sampling Rate: 	50MSa/s

-Vertical Resolution:	120mV/div
-Vertical Offset:	-68mV
-Number of Samples: 	37500
-
Time Interval:		750us (Full 10 rounds)

-Number of Traces:	1M
-Key:			2B 7E 15 16 28 AE D2 A6 AB F7 15 88 09 CF 4F 3C
-SCALE board:		LPC1114 (M0), Working frequency 12MHz, low pass filter on, amplifier on
-Ttest Constant:	DA 39 A3 EE 5E 6B 4B 0D 32 55 BF EF 95 60 18 90 (CRI Constant)
-Ttest Strategy:	Standard CRI processing (Random altering, test twice, use AES output as random inputs)
-ARM Toolchian:		arm-none-eabi-gcc 5.4.1 20160919


# Leaky Samples
-First Attempt:		11085
-Second Attempt:	11088
-Both:			9986


# Matlab Plotting script

```
Threshold=4.5;
LeakSamples1=sum(abs(VirTechByteMaskO1G1)>4.5);
LeakSamples2=sum(abs(VirTechByteMaskO1G2)>4.5);
LeakSamplesBoth=sum(bitand(abs(VirTechByteMaskO1G1)>4.5,abs(VirTechByteMaskO1G2)>4.5));
subplot(211);
hold;
xlim([0 37500]);
ylim([-400 400]);
xlabel('Time [*4ns]');
ylabel('T statistics');
title('CRI 1st Order T-test, first attempt: N=1M, PRNGOn');
plot(VirTechByteMaskO1G1,'b-','LineWidth',1);
x=get(gca,'xlim');
y=4.5;
y1=-4.5;
plot(x,[y y],'k--');
plot(x,[y1 y1],'k--');
hold off;
subplot(212);
hold;
xlim([0 37500]);
ylim([-400 400]);
xlabel('Time [*4ns]');
ylabel('T statistics');
title('CRI 1st Order T-test, second attempt: N=1M, PRNGOn');
plot(VirTechByteMaskO1G2,'b-','LineWidth',1);
x=get(gca,'xlim');
y=4.5;
y1=-4.5;
plot(x,[y y],'k--');
plot(x,[y1 y1],'k--');
hold off;
```
