[gd,w] = grpdelay([ 0 1 -1.2 1] , [1 -1.3 1.04 -.222], 512);
%[h,w] = freqz([ 0 1 -1.2 1] , [1 -1.3 1.04 -.222],512);
plot(w/pi,gd), grid
xlabel '\omega /\pi'
ylabel 'Group Delay'
title('Group Delay of H(e^{j\omega})')