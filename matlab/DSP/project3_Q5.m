num=[.15 0 -.15];
den=[1 -.5 .7];
L = 100;
[g,t]=impz(num,den, L)
stem(t,g), grid
xlabel 'time n'
ylabel 'h[n]'
title('100 samples of h[n] 4.36')