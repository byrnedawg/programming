% Program Q4_13
clear;
M = input('Enter the filter length M: ');
n = 0:M-1;
num = (-1).^n .* ones(1,M)/M;
[g,w] = gain(num,1);
plot(w/pi,g);grid;
axis([0 1 -50 0.5]);
xlabel('\omega /\pi');ylabel('Gain in dB');
title(['M = ', num2str(M)]);