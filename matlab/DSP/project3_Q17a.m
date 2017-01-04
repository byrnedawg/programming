% Program Q4_17a
clear;

L = input('Enter the value of L ');
w = 0:2*pi/511:2*pi;
num1 = [.5 zeros(1,L-1) .5];
den1 = [1];

h1 = freqz(num1, den1, w);

y = abs(h1);
plot(w/pi,y);
axis([0 2 0 1])
grid;
title(['|H(z)| L=',num2str(L)]);
xlabel('\omega /\pi');
ylabel('Amplitude');