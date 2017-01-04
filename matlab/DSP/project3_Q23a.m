% Program Q4_23a
clear;

num1 = [1];
den1 = [1 -1.848 .85];
zplane(num1,den1, 1)
xlabel 'Real Part'
ylabel 'Imaginary Part'
title('zplane of H_{1}(Z)')