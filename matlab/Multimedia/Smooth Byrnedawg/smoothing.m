close all;
I = imread('ByrneDawginvert 300x300.bmp'); 
   % Show current image.
   figure;
   imshow(I);
 
   %G=imnoise(I,'gaussian',0.0005,0.0019);
   G = I;
   h = fspecial('average',3);
   F=imfilter(G,h);
   figure;
   imshow(F); %
   
  B = imsharpen(F,'Threshold', 1);
  figure;
  imshow(B); %
   
  