% Program Q4_14
clear;
num = [.46065 .46065];
den = [1 -.07870];
[g,w] = gain(num, den);
dB3 = -3*ones(1,length(g));
t1 = .45;
t2 = -50:50.5/511:0.5;
plot(w/pi,g,w/pi,dB3,t1,t2,'r');grid;
axis([0 1 -50 0.5])
xlabel('\omega /\pi');ylabel('Gain in dB');
title(['Lowpass: \omega_{c} = ',num2str(t1),'\pi']);