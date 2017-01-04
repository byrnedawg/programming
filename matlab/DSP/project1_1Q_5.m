% Program Q1_5
% Generation of an Advanced Unit Step Sequence
clf;
% Generate a vector from -10 to 20
n = -10:20;
% Generate the Advanced unit step sequence
sd = [zeros(1,3) ones(1,28)];
% Plot the Advanced unit step sequence
stem(n,sd);
xlabel('Time index n');ylabel('Amplitude');
title('Q1.5 Advanced Unit Step Sequence');
axis([-10 20 0 1.2]);