function img=backproj(p,theta)
% Funcion for back projection
% Input: p: the projection matrix, each column is a projection, 
%           the number of rows is the number of projections
%        theta: a vector of all angles that projections were taken 
% Output: img: the reconstructed image.
% example of usage: 
% > obj=phantom(256);
% > p=radon(obj,1:5:180);
% > img=backproj(p,1:5:180);

len=size(p,1);   % length of each projection, i.e. diagonal size of NxN
N = 2*floor(len/(2*sqrt(2)))-2;  % from length of projection to get N

% Define the x & y axes for the reconstructed image so that the origin
% (center) is in the spot which RADON would choose.
xax = (1:N)-ceil(N/2);
x = repmat(xax, N, 1);    % x coordinates
y = rot90(x);             % y coordinates

theta = pi*theta/180;   % convert to angles

costheta = cos(theta);
sintheta = sin(theta);
ctrIdx = ceil(len/2);     % index of the center of the projections

img=zeros(N,N);   % memory for reconstructed image

num_proj=size(p,2);

for i=1:num_proj,
  proj = p(:,i);
  t = x.*costheta(i) + y.*sintheta(i); 
  a = floor(t);  
  img = img + (t-a).*proj(a+1+ctrIdx) + (a+1-t).*proj(a+ctrIdx);
end

