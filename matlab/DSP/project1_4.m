% Program P1_4
% Generation of a sinusoidal sequence
n = 0:1000;
f = 0.1;
%phase = 0;
%A = 3;
%arg = 1.3*pi*f*n - phase;
%x = A*cos(1.3*pi*f*n)-4*sin(0.5*pi*f*n+.5*pi);
%x = 5*cos(1.5*pi*n)+4*cos(0.6*pi*n)-sin(.5*pi*n);
%x = 3*cos(1.3*pi*n)-4*sin(0.5*pi*n+0.5*pi);
x = exp((pi*n)/5)+ exp((pi*n)/10);
clf; % Clear old graph
stem(n,x); % Plot the generated sequence
axis([0 60 0 1000]);
grid;
title('Sinusoidal Sequence');
xlabel('Time index n');
ylabel('Amplitude');
axis;