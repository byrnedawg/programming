% Compute the impulse response h
clf;
n = -100:100;
num = 1 - 2*cos(i*pi*n);
den = [1];
y = impz(num,den,200);
% Plot the impulse response
stem(y);
xlabel('Time index n'); ylabel('Amplitude');
title('Impulse Response'); grid;