clc;
clear all
workspace;
format compact;
format long g;
angle = [0.4, 0.6, pi/4, 1.0, 1.2];
V0 = 120;
g = -9.8;
t = 0 : .01 : 500;
Ax = 0;
Ay= g;
numberOfAngles = length(angle);
for k = 1 : numberOfAngles
	thisAngle = angle(k);
	xVelocity = V0 * cos(thisAngle);
	yVelocity = V0 * sin(thisAngle);
	x = xVelocity .* t + (1/2) * Ax .* t.^2;
	y = yVelocity .* t + (1/2) * Ay .* t.^2;
	subplot(2, 3, k);
	plot(x, y, 'b-', 'LIneWidth', 3);
	caption = sprintf('Angle = %.3f radians = %.2f degrees\n', ...
		thisAngle, thisAngle*180/pi);
	title(caption, 'FontSize', 15);
	grid on;
	xlim([0 6e4]);
end
% Enlarge figure to full screen.
set(gcf, 'units','normalized','outerposition',[0 0 1 1]);