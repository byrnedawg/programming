% Program P4_3
% Zero Locations of Linear Phase FIR Filters
clf;
b = [2 1.5 -3.2 -5.2];
num1 = [b 6.4 fliplr(b)];
num2 = [b 6.4 6.4 fliplr(b)];
num3 = [b 6.4 0 -6.4 -fliplr(b)];
num4 = [b 6.4 -6.4 -fliplr(b)];
subplot(2,2,1); zplane(num1,1);
title('Type 1 FIR Filter');
subplot(2,2,2); zplane(num2,1);
title('Type 2 FIR Filter');
subplot(2,2,3); zplane(num3,1);
title('Type 3 FIR Filter');
subplot(2,2,4); zplane(num4,1);
title('Type 4 FIR Filter');
disp('Zeros of Type 1 FIR Filter are');
disp(roots(num1));
disp('Zeros of Type 2 FIR Filter are');
disp(roots(num2));
disp('Zeros of Type 3 FIR Filter are');
disp(roots(num3));
disp('Zeros of Type 4 FIR Filter are');
disp(roots(num4));