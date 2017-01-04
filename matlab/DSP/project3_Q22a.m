% Program Q4_22a
clear;

w = 0:2*pi/511:2*pi;
num1 = [1 -1];
den1 = [2 1 1];

h1 = freqz(num1, den1, w);

y = abs(h1);
plot(w/pi,y);
axis([0 1 0 2])
grid;
title('|G_{1}(z)|');
xlabel('\omega /\pi');
ylabel('Amplitude');