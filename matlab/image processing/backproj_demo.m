

obj=phantom(256);


for angles=1:5:180,

angles,

 r=radon(obj,angles);
 backimg=backproj(r,angles);

figure(1),
 imagesc(backimg);
 colormap(gray(256));
 truesize;

figure(2)
plot(r);

figure(3)
mesh(1:256,1:256,backimg);
colormap('default');
axis([1,256,1,256,1,256]);

pause
end

pause

%backprojection

for k=1:36

angles=1:5:k*5,

 r=radon(obj,angles);
 backimg=backproj(r,angles);

figure(1)
 imagesc(backimg);
 colormap(gray(256));
 truesize;

figure(3)
mesh(1:256,1:256,backimg);
colormap('default');
%axis([1,256,1,256,1,256]);


pause;

end

pause

%Filtered backprojection

for k=1:36

angles=1:5:k*5;

 p=radon(obj,angles);

 len=size(p,1);  

% differentiator is simple two-sample difference

pd=p(2:len,:)-p(1:len-1,:);

% convolution with the filter 1/tau is equivalent to a Hilbert 
% transform. Try “help Hilbert” in Matlab

pdh=hilbert(pd);   % Hilbert transform on each column -- projection
pdh=imag(pdh);     % In Matlab Hilbert function, the imaginary part of     
                   %the output is the Hilbert transform result.


 backimg=backproj(pdh,angles);

backimg = backimg/(2*length(angles));  % normalize the image


figure(1)
 imagesc(backimg);
 colormap(gray(256));
 truesize;

%figure(3)
%mesh(1:256,1:256,backimg);
%colormap('default');

pause(1);

end

