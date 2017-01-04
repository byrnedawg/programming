%/*************************************************************************
%/* File Name: MSBStoreNameRow.m
%/* Purpose: replace the Most significant bit plane of cover images with hidden 
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
ImageName = 'mandril_color'
inputImage = strcat(Source_Folder,ImageName,File_Format)

% The secret Key
SecretKey = 513

% Reading image
oriI = imread(inputImage);
message = ['Gregory Byrne'];
oriI(1:1:1)= length(message) ; %to count massage Char to easly retrive all the massage 
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

% The height and width of the image
[Iwidth, Iheight] = size(I);

% The total number of pixels
num = Iwidth * Iheight;

% The hidden message
HiddenData = double(DataDouble);

% Replacing the least significant bit plane
Iseq = I(:);

% The secret key determines the embedding position
KeyPosition = [SecretKey:1:(SecretKey+LengthofMessage)];

tempSeq = Iseq(KeyPosition);

%MSB Encoding changing the MSB in the pixel
for k = 1:LengthofMessage
 if(tempSeq(k) < 128 && HiddenData(k) == 1)
     tempSeq(k) = 2*fix(tempSeq(k)/2)+128;
 elseif(tempSeq(k) >= 128 && HiddenData(k) == 0)
     tempSeq(k) = 2*fix(tempSeq(k)/2)-128;
 else
      tempSeq(k) = 2*fix(tempSeq(k)/2);
 end
end

Iseq(KeyPosition) = tempSeq;
MsbI = reshape(Iseq, Iwidth, Iheight);

% PSNR value
%psnr_MSB = PSNR(I,MsbI);

% saving the stego image
SavingName = strcat(Destination_Folder,'stegoMSB',ImageName,File_Format)
    if length(GCsign)==3
        oriI(:,:,1) = MsbI;
        imwrite(uint8(oriI),SavingName)
    else
        imwrite(uint8(MsbI),SavingName)
    end


figure(2)
imshow(uint8(oriI))
title('MSB stego image')


%% Extracting process
%SecretKey 
SecretKey = 513
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

Iseq2 = ISI(:);

ExtractedMessageLength =  double( OsI(1:1:1) ) * 8  ; %learn message length
%rand('twister',SecretKey);

% The secret key determines the embedding position
KeyPosition = [SecretKey:1:(SecretKey+ExtractedMessageLength)];

tempSeq2 = Iseq2(KeyPosition);
% The decodedMessage

decodedMessage = zeros(ExtractedMessageLength,1);

%MSB Decoding of MSB of pixel
for k = 1:ExtractedMessageLength
 if(tempSeq2(k) >= 128)
     decodedMessage(k) = 1;
 else
     decodedMessage(k) = 0;
 end
end

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

