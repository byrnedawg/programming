% Program Q4_10
clear;
% Get "N" from the user command line
N = input('Enter the filter time shift N: ');
% compute the magnitude spectrum
No2 = N/2;
fc = 0.25;
n = [-No2:1:No2];
y = 2*fc*sinc(2*fc*n);
w = 0:pi/511:pi;
h = freqz(y, [1], w);
plot(w/pi,abs(h));
grid;
title(strcat('|H(e^{j\omega})|, N=',num2str(N)));
xlabel('\omega /\pi');
ylabel('Amplitude');