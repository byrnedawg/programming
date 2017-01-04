% Program project3_Q26
% Stability Test
clf;
den = [1 .2 .3 .4 .5 .6];
ki = poly2rc(den);
disp('Stability test parameters are');
disp(ki);