% Program project3_Q25
% Stability Test
clf;
den = [1 2.5 2.5 1.25 .3125 .03125];
ki = poly2rc(den);
disp('Stability test parameters are');
disp(ki);