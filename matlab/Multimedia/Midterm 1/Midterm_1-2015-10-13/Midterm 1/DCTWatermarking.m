%/*************************************************************************
%/* File Name: DCTWatermarking.m
%/* Purpose: Replace most Significant points in DCT with watermark data
%/* Input: cover images in the format such as '.bmp' 
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

% The secret Key
SecretKey = 1

% Reading image
oriI = imread(inputImage);
message = ['This is a 1000 bit long watermark for the midterm written by Gregory Byrne this message is used to see if we can extract data'];
%oriI(1:1:1)= length(message) ; %to count message Char to easly retrive all the massage 
% Write Message to be encoded
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

figure(1)
imshow(uint8(oriI))
title('cover image')


% Determinie the color or gray image
GCsign = oriI(1,1,:);

% if not a gray image, then replace the LSB of the Red component
    if length(GCsign)==3
        CI = oriI(:,:,1);
    else
        CI = oriI;
    end

I = double(CI);

coverDCT = dct(I);

[coverDCT,Idx]=sort(abs(coverDCT),'descend');%re-ordering all the absolute values 

figure(2)
imshow(uint8(coverDCT))
title('DCT image')




% The height and width of the image
[Iwidth, Iheight] = size(coverDCT);

% The total number of pixels
num = Iwidth * Iheight;

% The hidden message
HiddenData = double(DataDouble);

% Replacing the least significant bit plane
Iseq = ceil(coverDCT(:));

% The secret key determines the embedding position
KeyPosition = [SecretKey:1:(SecretKey+LengthofMessage)];

tempSeq = Iseq(KeyPosition);

tempSeq(1:LengthofMessage) = 2*fix(tempSeq(1:LengthofMessage)/2) + HiddenData;
Iseq(KeyPosition) = tempSeq;
LsbI = reshape(Iseq, Iwidth, Iheight);


figure(3)
imshow(uint8(LsbI))
title('LSB DCT')

InverseDCTCover = idct(LsbI);

%figure(4)
%imshow(uint8(InverseDCTCover))
%title('Recovered image')

% PSNR value
%psnr_LSB = PSNR(I,LsbI)

% saving the stego image
SavingName = strcat(Destination_Folder,'stegoDCT',ImageName,File_Format)
    if length(GCsign)==3
       oriI(:,:,1) = InverseDCTCover;
        imwrite(uint8(oriI),SavingName)
    else
        imwrite(uint8(InverseDCTCover),SavingName)
    end


figure(4)
imshow(uint8(oriI))
title('DCT stego image')


%% Extracting process

% The secret Key
SecretKey = 1

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

ISI = double(SI);

watermarkedDCT = dct(ISI);

Iseq2 = ceil(watermarkedDCT(:));

ExtractedMessageLength =  1000  ; %learn message length

% The secret key determines the embedding position
KeyPosition2 = [SecretKey:1:(SecretKey+ExtractedMessageLength)];

tempSeq2 = Iseq2(KeyPosition2);
% The decodedMessage
decodedMessage = mod(tempSeq2(1:ExtractedMessageLength),2);

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
%show error
Diff = HiddenData - decodedMessage;
DecodingBitError = length( find(Diff ~=0) )/ExtractedMessageLength

