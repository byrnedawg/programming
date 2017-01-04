% Program Q4_15b
clear;
num = [.33754 .33754];
den = [1 -.32492];
[g,w] = gain(num, den);
dB3 = -3*ones(1,length(g));
t1 = .3;
t2 = -50:50.5/511:0.5;
plot(w/pi,g,w/pi,dB3,t1,t2,'r');grid;
axis([0 1 -50 0.5])
xlabel('\omega /\pi');ylabel('Gain in dB');
title(['HLP,10(Z) \omega_{c} = ',num2str(t1),'\pi']);