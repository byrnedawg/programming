% Program Q4_14c
clear;

w = 0:2*pi/511:2*pi;
num1 = [.46065 .46065];
den1 = [1 -.07870];

num2 = [.53935 -.53935];
den2 = [1 -.07870];

h1 = freqz(num1, den1, w);
h2 = freqz(num2, den2, w);
y = abs(h1+h2);
plot(w/pi,y);
axis([0 1 0 1.5])
grid;
title('Sum of Magnitude |HLP(Z) + HHP(Z)|');
xlabel('\omega /\pi');
ylabel('Amplitude');