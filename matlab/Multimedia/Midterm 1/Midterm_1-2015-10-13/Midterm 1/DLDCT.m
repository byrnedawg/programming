%/*************************************************************************
%/* File Name: LSBStoreNameRow.m
%/* Purpose: replace the least significant bit plane of cover images with hidden 
%   message bits
%/* Input: cover images in the format such as '.bmp', embeding rate and secret key 
%/* Output: stego images with PSNR denoting the visual distortion  
%/*************************************************************************


clear all
close all

% The folder of cover images
Source_Folder = 'N:\Private\Projects\Electrical Engineering\MATLAB\Midterm 1\Midterm_1-2015-10-13\Midterm 1\'

% The folder of stego images
Destination_Folder = 'N:\Private\Projects\Electrical Engineering\MATLAB\Midterm 1\Midterm_1-2015-10-13\Midterm 1\'
mkdir(Destination_Folder)

% Image foramt
File_Format = '.bmp'


% Embedding Process
% Image name
ImageName = 'lena'
inputImage = strcat(Source_Folder,ImageName,File_Format)

% Reading image
OriI = imread(inputImage);

figure(1)
imshow(uint8(OriI))
title('Cover image')

message = ['This is a 1000 bit long watermark for the midterm written by Gregory Byrne this message is used to see if we can extract data'];
Watermark_Size=1000;

message = strtrim(message); %Get rid of white space
%Convert message to Ascii values
AsciiCode = uint8(message);
%Convert to Ascii values to binary
binaryString = transpose(dec2bin(AsciiCode,8));
binaryString = binaryString(:);
LengthofMessage = length(binaryString);
DataDouble = zeros(LengthofMessage,1);
for k = 1:LengthofMessage
  if(binaryString(k) == '1')
      DataDouble(k) = 1;
  else
      DataDouble(k) = 0;
  end
end

% Determinie the color or gray image
GCsign = OriI(1,1,:);

% if not a gray image, then replace the LSB of the Red component
    if length(GCsign)==3
        CI = OriI(:,:,1);
    else
        CI = OriI;
    end

I = double(CI);

I=I(:,:,1);
[Iwidth,Iheight]=size(I);
DCT_Cover_Image=dct2(I);
DCT_Cover_VEC=reshape(DCT_Cover_Image,1,Iwidth*Iheight);
[D_vec_srt,Idx]=sort(abs(DCT_Cover_VEC),'descend');

figure(2)
imshow(uint8(DCT_Cover_Image))
title('DCT image')


% The hidden message
HiddenData = double(DataDouble);
WaterMark = HiddenData;

figure(3);
plot(WaterMark);
title('Watermark');

Idx2=Idx(2:Watermark_Size+1);%choosing 1000 biggest values other than the DC
%finding associated row-column order for vector values
tempSeq=zeros(Watermark_Size,2);

for k=1:Watermark_Size
    x= floor(Idx2(k)/Iwidth)+1;%associated culomn in the image
    y= mod(Idx2(k),Iwidth);%associated row in the image
    tempSeq(k,1)=y;
    tempSeq(k,2)=x;
    end
Watermarked_Image=DCT_Cover_Image;
%WATERMARK EMBEDDING
for k=1:Watermark_Size
     Watermarked_Image(tempSeq(k,1),tempSeq(k,2))=Watermarked_Image(tempSeq(k,1),tempSeq(k,2))+.1*Watermarked_Image(tempSeq(k,1),tempSeq(k,2)).*WaterMark(k);
     
end
I2=idct2(Watermarked_Image);%inverse DCT to produce the watermarked asset
I2_int=uint8(I2);

SavingName = strcat(Destination_Folder,'stegoDCT',ImageName,File_Format)
    if length(GCsign)==3
       oriI(:,:,1) = I2_int;
        imwrite(uint8(oriI),SavingName)
    else
        imwrite(uint8(InverseDCTCover),SavingName)
    end


%imwrite(I2_int,'I2_watermarkedn.bmp','bmp');
figure(4);
imshow(uint8(I2_int))
title('Watermarked Image');
%subplot(2,3,3),imshow('I2_watermarkedn.bmp'),title('Watermarked Image');

%% WATERMARK EXTRACTION

WMI = imread(SavingName);
figure(5);
imshow(uint8(WMI))
title('Watermarked Image for Extraction');



Extracted_Data=[1000];
for k=1:Watermark_Size
    Extracted_Data(k)=((Watermarked_Image(tempSeq(k,1),tempSeq(k,2))/DCT_Cover_Image(tempSeq(k,1),tempSeq(k,2))-1)*10);
  
end

%convert binary valaues to chars
binaryVector = Extracted_Data;
binValues = [ 128 64 32 16 8 4 2 1 ];
binaryVector = binaryVector(:);
if mod(length(binaryVector),8) ~= 0
error('Length of binary vector must be a multiple of 8.');
end
binMatrix = reshape(binaryVector,8,[]);
textString = char(binValues*binMatrix);
disp(textString);

figure(6);
plot(Extracted_Data),title('Extracted Watermark');

%%
Wstar = Extracted_Data;
Worig = Extracted_Data;
SIM=sum(Wstar.*Worig)/sqrt(sum(Wstar.*Wstar))