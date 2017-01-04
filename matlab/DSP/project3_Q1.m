% Program Q4_1
clear;
% User specifies filter length
M = input('Enter the filter length M: ');
% Compute the frequency samples of the DTFT
w = 0:2*pi/1023:2*pi;
num = (1/M)*ones(1,M);
den = [1];
% Compute and plot the DTFT
h = freqz(num, den, w);
subplot(2,1,1)
plot(w/pi,abs(h));grid
title('Magnitude Spectrum |H(e^{j\omega})|')
xlabel('\omega /\pi');
ylabel('Amplitude');
subplot(2,1,2)
plot(w/pi,angle(h));grid
title('Phase Spectrum arg[H(e^{j\omega})]')
xlabel('\omega /\pi');
ylabel('Phase in radians');