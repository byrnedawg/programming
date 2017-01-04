% Program P4_3
% Zero Locations of Linear Phase FIR Filters
clf;
w = 0:2*pi/1023:2*pi;
b = [1 -8.5 30.5 -63];
num1 = [b 81 fliplr(b)];
num2 = [b 81 81 fliplr(b)];
num3 = [b 0 -fliplr(b)];
num4 = [b 81 -81 -fliplr(b)];
den = [1];
n1 = 0:length(num1)-1;
n2 = 0:length(num2)-1;
n3 = 0:length(num3)-1;
n4 = 0:length(num4)-1;

phi1 = phasez(n1,num1,w);
phi2 = phasez(n2,num2,w);
phi3 = phasez(n3,num3,w);
phi4 = phasez(n4,num4,w);

subplot(2,2,1);
plot(w/pi,phi1);
axis([0 1 -15 0]);grid
title('Type 1 FIR Filter')
xlabel('\omega /\pi');
ylabel('Phase in radians');

subplot(2,2,2); 
plot(w/pi,phi2);
axis([0 1 -15 0]);grid
title('Type 2 FIR Filter')
xlabel('\omega /\pi');
ylabel('Phase in radians');

subplot(2,2,3); 
plot(w/pi,phi3);
axis([0 1 -15 0]);grid
title('Type 3 FIR Filter')
xlabel('\omega /\pi');
ylabel('Phase in radians');

subplot(2,2,4);
plot(w/pi,phi4);
axis([0 1 -15 0]);grid
title('Type 4 FIR Filter')
xlabel('\omega /\pi');
ylabel('Phase in radians');

