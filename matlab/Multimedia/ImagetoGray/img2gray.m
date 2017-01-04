

clear all
close all

% The folder of cover images
Source_Folder = 'N:\Private\Projects\Electrical Engineering\MATLAB\ImagetoGray\'

% The folder of stego images
Destination_Folder = 'N:\Private\Projects\Electrical Engineering\MATLAB\ImagetoGray\'
mkdir(Destination_Folder)

% Image foramt
File_Format = '.jpg';


% Embedding Process
% Image name
ImageName = 'CAC card';
inputImage = strcat(Source_Folder,ImageName,File_Format);

% Reading image
color = imread(inputImage);

figure(1)
imshow(uint8(color));
title('cover image');

gray = rgb2gray(uint8(color));
figure(2);
imshow(uint8(gray));
title('grayscale image');
SavingName = strcat(Destination_Folder,'grayscale',ImageName,File_Format);
  
imwrite(uint8(gray),SavingName);
