function [m, thd, SWR, rloss] = smith_ch_calc(Z0, Zl) 

% Draw appropriate chart
draw_smith_chart 

% Normalize given impedance
zl = Zl/Z0; 

% Calculate reflection, magnitude and angle
g = (zl - 1)/(zl + 1);
m = abs(g);
th = angle(g); 

% Plot appropriate point
polar(th, m, 'r*') 

% Change radians to degrees
thd = th * 180/pi; 

% Calculate VSWR and return loss.
% We can add epsilon to magnitude, to avoid div by 0 or log(0)
SWR = (1 + m)/(1 - m + eps);
rloss = -20 * log10(m + eps);