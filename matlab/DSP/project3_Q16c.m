% Program Q4_16c
clear;
num = [.19360 0 -.19360];
den = [1 .54626 .6128];
[g,w] = gain(num, den);
dB3 = -3*ones(1,length(g));
t = .61;
bandwidth = .15;
t1 = t + bandwidth/2;
t2 = t - bandwidth/2;
y = -50:50/511:.5;
plot(w/pi,g,w/pi,dB3,t1,y,'g',t2,y,'r');grid;
axis([0 1 -50 0.5])
xlabel('\omega /\pi');ylabel('Gain in dB');
title(['H_{BP,2}(Z) \omega_{0} = ',num2str(t),...
    '\pi \Delta\omega_{3dB} = ',num2str(bandwidth),'\pi' ]);