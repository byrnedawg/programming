% Program Q2_4
% Generate the input signal
n = 0:100;
a = pi/200;
b = 0;
arg = a*n.*n + b*n;
x = cos(arg);
M = input('Desired length of the filter = ');
num = ones(1,M);
y = filter(num,1,x)/M;
clf;
subplot(2,1,1);
plot(n, x);
axis([0, 100, -1.5, 1.5]);
xlabel('Time index n'); ylabel('Amplitude');
title('Input Signal');
subplot(2,1,2);
plot(n, y);
axis([0, 100, -1.5, 1.5]);
xlabel('Time index n'); ylabel('Amplitude');
title('Output Signal');
axis;