num=[.15 0 -.15];
den=[.7 -.5 1];
L = 100;
[g,t]=impz(num,den, L)
stem(t,g), grid
xlabel 'time n'
ylabel 'h[n]'
title('100 samples of h[n] 4.37')