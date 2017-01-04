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
Source_Folder = 'N:\Private\Projects\Electrical Engineering\MATLAB\Midterm 1\'

% The folder of stego images
Destination_Folder = 'N:\Private\Projects\Electrical Engineering\MATLAB\Midterm 1\'
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
title('cover image')

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

% The height and width of the image
[Iwidth, Iheight] = size(I);

DCT_Cover_Image=dct2(I);
DCT_Cover_VEC=reshape(DCT_Cover_Image,1,Iwidth*Iheight);
[D_vec_srt,Idx]=sort(abs(DCT_Cover_VEC),'descend');

figure(2)
imshow(uint8(DCT_Cover_Image))
title('DCT image')


% The hidden message
HiddenData = double(DataDouble);
W = HiddenData;

figure(3);
plot(W);
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
DCT_Watermarked=DCT_Cover_Image;
%WATERMARK EMBEDDING
for k=1:Watermark_Size
     fw=DCT_Watermarked(tempSeq(k,1),tempSeq(k,2));
     %fw = fw+0.1*fw.*W(k);
     
end
I2=idct2(DCT_Watermarked);%inverse DCT to produce the watermarked asset
I2_int=uint8(I2);

SavingName = strcat(Destination_Folder,'stegoDCT',ImageName,File_Format)
    if length(GCsign)==3
       oriI(:,:,1) = I2_int;
        imwrite(uint8(oriI),SavingName)
    else
        imwrite(uint8(I2_int),SavingName)
    end

figure(4);
imshow(uint8(I2_int))
title('Watermarked Image');

%% WATERMARK EXTRACTION

WMI = imread(SavingName);
figure(5);
imshow(uint8(WMI))
title('Watermarked Image for Extraction');



W2=[];
for k=1:Watermark_Size
    W2(k)=((DCT_Watermarked(tempSeq(k,1),tempSeq(k,2))/DCT_Cover_Image(tempSeq(k,1),tempSeq(k,2))-1)*10);
end

decodedMessage = W2;
%convert binary valaues to chars
binaryVector = decodedMessage;
binValues = [ 128 64 32 16 8 4 2 1 ];
binaryVector = binaryVector(:);
if mod(length(binaryVector),8) ~= 0
error('Length of binary vector must be a multiple of 8.');
end
binMatrix = reshape(binaryVector,8,[]);
textString = char(binValues*binMatrix);
disp(textString);

figure(6);
plot(W2),title('Extracted Watermark');

