% Program Q4_15a
clear;
num = [1739.14 1739.14];
den = [3.21758 1];
[g,w] = gain(num, den);
dB3 = -3*ones(1,length(g));
t1 = .3;
t2 = -50:50.5/511:0.5;
plot(w/pi,g,w/pi,dB3,t1,t2,'r');grid;
%axis([0 1 -50 0.5])
xlabel('\omega /\pi');ylabel('Gain in dB');
title(['HLP10(Z) \omega_{c} = ',num2str(t1),'\pi']);