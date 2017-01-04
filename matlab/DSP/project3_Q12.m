% Program Q4_12
clear;
K = input('Enter K: ');
hertz = [1];
for i=1:K;
hertz = conv(hertz,[1 1]);
end;
hertz = (0.5)^K * hertz;
[g,w] = gain(hertz,1);
dB3 = -3*ones(1,length(g));
t1 = 2*acos((0.5)^(1/(2*K)))*ones(1,512)/pi;
t2 = -50:50.5/511:0.5;
plot(w/pi,g,w/pi,dB3,t1 'Color', [1.0, 0.0, 0.0],t2);grid;
axis([0 1 -50 0.5])
xlabel('\omega /\pi');ylabel('Gain in dB');
title(['K = ',num2str(K),'; Theoretical \omega_{c} = ',num2str(t1(1))]);