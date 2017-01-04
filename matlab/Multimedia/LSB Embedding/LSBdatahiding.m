%/*************************************************************************
%/* File Name: LSBhiding.m
%/* Purpose: replace the least significant bit plane of cover images with hidden 
%   message bits
%/* Input: cover images in the format such as '.bmp', embeding rate and secret key 
%/* Output: stego images with PSNR denoting the visual distortion  
%/* Copyright 2011 Guoqi Luo
%/*************************************************************************


clear all
close all

% The folder of cover images
Source_Folder = 'N:\Private\Projects\Electrical Engineering\MATLAB\LSB Embedding\'

% The folder of stego images
Destination_Folder = 'N:\Private\Projects\Electrical Engineering\MATLAB\LSB Embedding\'
mkdir(Destination_Folder)

% Image foramt
File_Format = '.bmp'


% Embedding Process
% Image name
ImageName = 'blackpoke'
inputImage = strcat(Source_Folder,ImageName,File_Format)

% Reading image
color = imread(inputImage);

figure(1)
imshow(uint8(color))
title('cover image')


gray = rgb2gray(uint8(color));
figure(2)
imshow(gray)
title('Gray Scale')

oriI = gray;

% Determinie the color or gray image
GCsign = oriI(1,1,:);

% if not a gray image, then replace the LSB of the Red component
    if length(GCsign)==3
        CI = oriI(:,:,1);
    else
        CI = oriI;
    end

I = double(CI);

% The height and width of the image
[Iwidth, Iheight] = size(I);



% The total number of pixels
num = Iwidth * Iheight;

% The embedding rate bit per pixel
EmbeddingRate = 1;

counts = [.50 .50]; % A one occurs 50% of the time.
len = EmbeddingRate*num;
% The hidden message
HData = randsrc(len,1,[0 1; counts]);

% Replacing the least significant bit plane
Iseq = I(:);

% The secret Key
%SecretKey = 738977812
SecretKey = 3

rand('twister',SecretKey);

% The secret key determines the embedding position
randomPosition = randperm(num);
tempSeq = Iseq(randomPosition);
tempSeq(1:len) = 2*fix(tempSeq(1:len)/2) + HData;
Iseq(randomPosition) = tempSeq;
LsbI = reshape(Iseq, Iwidth, Iheight);

% PSNR value
MSE_value = MSE(I,LsbI)

%psnr_LSB = psnr(I,LsbI)
peaksnr = psnr(I,LsbI)

%psnr_LSB = psnr(I,I)

% saving the stego image
SavingName = strcat(Destination_Folder,'stegoLSB',ImageName,File_Format)
    if length(GCsign)==3
        oriI(:,:,1) = LsbI;
        imwrite(uint8(oriI),SavingName)
    else
        imwrite(uint8(LsbI),SavingName)
    end


figure(3)
imshow(uint8(oriI))
title('stego image')

diff = I - LsbI;


%% Extracting process

% Reading image
OsI = imread(SavingName);

% Determinie the color or gray image
GCsign = OsI(1,1,:);

% if not a gray image, then replace the LSB of the Red component
    if length(GCsign)==3
        SI = OsI(:,:,1);
    else
        SI = OsI;
    end

SI = double(SI);
% The secret Key
SecretKey = 738977812
rand('twister',SecretKey);

% The secret key determines the embedding position
randomPosition = randperm(num);
tempSeq = Iseq(randomPosition);
% The decodedMessage
decodedMessage = mod(tempSeq(1:len),2);

Diff = HData- decodedMessage;
DecodingBitError = length( find(Diff ~=0) )/len

