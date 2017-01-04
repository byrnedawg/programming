% Program Q4_3
clear;
% User specifies filter length
%M = input('Enter the filter length M: ');
% Compute the frequency samples of the DTFT
w = 0:1*pi/1023:1*pi;
num = [.15 0 -.15];
den = [.7 -.5 1];
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