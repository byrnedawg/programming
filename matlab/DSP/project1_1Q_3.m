% Program P1_1 Q1.3
% Generation of a Delayed Unit Sample Sequence
clf;
% Generate a vector from -10 to 20
n = -10:20;
% Generate the Delayed unit sample sequence
u = [zeros(1,21) 1 zeros(1,9)];
% Plot the Delayed unit sample sequence
stem(n,u);
xlabel('Time index n');ylabel('Amplitude');
title('Q1.3 Unit Sample Sequence');
axis([-10 20 0 1.2]);