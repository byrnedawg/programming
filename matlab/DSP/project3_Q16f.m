% Program Q4_14c
clear;

w = 0:2*pi/511:2*pi;

num1=[.19360 0 -.19360];
den1=[1 .54626 .6128];

num2 = [.8064 .546255 .8064];
den2 = [1 .54626 .6128];

h1 = freqz(num1, den1, w);
h2 = freqz(num2, den2, w);
y = (abs(h1).^2) + (abs(h2).^2);
plot(w/pi,y);
axis([0 1 0 1.5])
grid;
title('Sum of Square Magnitude |H_{BP}(Z)|^2 + |H_{BS}(Z)|^2');
xlabel('\omega /\pi');
ylabel('Amplitude');