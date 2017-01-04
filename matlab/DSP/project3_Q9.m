% Program Q4_9
clf;
fc = 0.65;
n = [-7.0:1:7.0];
y = 2*fc*sinc(2*fc*n);k = n+7.0;
stem(k,y);title('N = 14');axis([0 14 -0.4 1.4]);
xlabel('Time index n');ylabel('Amplitude');grid;