% Program Q4_8
% Impulse Response of Truncated Ideal Lowpass Filter
clf;
fc = 0.45;
n = [-9.5:1:9.5];
y = 2*fc*sinc(2*fc*n);k = n+9.5;
stem(k,y);title('N = 20');axis([0 19 -0.2 0.7]);
xlabel('Time index n');ylabel('Amplitude');grid;