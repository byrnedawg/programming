% Program Q4_14d
clear;

w = 0:2*pi/511:2*pi;
num1 = [.46065 .46065];
den1 = [1 -.07870];

num2 = [.53935 -.53935];
den2 = [1 -.07870];

h1 = freqz(num1, den1, w);
h2 = freqz(num2, den2, w);
y = (abs(h1).^2) + (abs(h2).^2);
plot(w/pi,y);
axis([0 1 0 1.5])
grid;
title('Sum of Square Magnitude |HLP(Z)|^2 + |HHP(Z)|^2');
xlabel('\omega /\pi');
ylabel('Amplitude');