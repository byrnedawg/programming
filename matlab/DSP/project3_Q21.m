% Program Q4_21
clear;

w = 0:2*pi/511:2*pi;
num1 = [1.5 1.5];
den1 = [1 5 6];

h1 = freqz(num1, den1, w);

y = abs(h1)
plot(w/pi,y);
axis([0 1 0 1])
grid;
title('|H(z)|');
xlabel('\omega /\pi');
ylabel('Amplitude');