%/*************************************************************************
%/* File Name: midtermDCT.m
%/* Purpose: Using the DCT to embedded watermark in the frequency domain
%/* Input: cover images in the format such as '.bmp' 
%/* Output: watermarked image and extracted watermark with similarity value    
%/*************************************************************************


clear all
close all

% The folder of cover images
Source_Folder = 'N:\MATLAB\Midterm 1\'

% The folder of stego images
Destination_Folder = 'N:\MATLAB\Midterm 1\'
mkdir(Destination_Folder)

% Image foramt
File_Format = '.bmp'


% Image name
ImageName = 'lena'
inputImage = strcat(Source_Folder,ImageName,File_Format)

% Reading image
OriI = imread(inputImage);

figure(1)
imshow(uint8(OriI))
title('Cover image')

Watermark_Size=1000;
Watermark = randn(1,Watermark_Size); %generate random watermark size = 1000

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
DCT_Cover_Image=dct2(I); %DCT of the image
DCT_Cover_VEC=reshape(DCT_Cover_Image,1,Iwidth*Iheight); %variable to store DCT as vector
[DCT_vec_srt,Pos]=sort(abs(DCT_Cover_VEC),'descend'); %re-arrange DCT vector in abs order

figure(2)
imshow(uint8(DCT_Cover_Image)) % show DCT image
title('DCT image')

figure(3);
plot(Watermark); % show ramdom watermark size 1000 to be inserted
title('Watermark');

Index=Pos(2:Watermark_Size+1);%find 1000 biggest values in DCT other than the first one
%finding associated row-column order for vector values
Row_Column_Matrix=zeros(Watermark_Size,2);

for k=1:Watermark_Size
    x= floor(Index(k)/Iwidth)+1;% image column
    y= mod(Index(k),Iwidth);% image row
    Row_Column_Matrix(k,1)=y; % store row in matrix 
    Row_Column_Matrix(k,2)=x; % store column in matrix
end
Watermarked_Image=DCT_Cover_Image;
% Embedding the watermark in the DCT of the image
watermark_strength = 0.1;
for k=1:Watermark_Size
     Watermarked_Image(Row_Column_Matrix(k,1),Row_Column_Matrix(k,2))= ...
     Watermarked_Image(Row_Column_Matrix(k,1),Row_Column_Matrix(k,2))+ ...
        watermark_strength*Watermarked_Image(Row_Column_Matrix(k,1),...
        Row_Column_Matrix(k,2)).*Watermark(k);   
end
%take the inverse dct of the watermarked image created in the frequency
%domain
InverseDCT=idct2(Watermarked_Image);%inverse DCT 
InverseDCT_uint8=uint8(InverseDCT); %convert to uint8

SavingName = strcat(Destination_Folder,'stegoDCT',ImageName,File_Format)
    if length(GCsign)==3
       oriI(:,:,1) = InverseDCT_uint8;
        imwrite(uint8(oriI),SavingName)
    else
        imwrite(uint8(InverseDCTCover),SavingName)
    end


figure(4);
imshow(uint8(InverseDCT_uint8))
title('Watermarked Image');


% Create distorted watermarked images to test robustness

qtr_scaled_watermark = imresize(InverseDCT_uint8,.25);
qtr_scaled_watermark = imresize(qtr_scaled_watermark,4);
figure(5);
imshow(uint8(qtr_scaled_watermark))
title('1/4 Scaled Watermarked Image then Rescaled');


%Specify SNR
SNR=10;
mean_value=0;
noisyimage = InverseDCT_uint8;
var=mean(mean((double(noisyimage)/255).^2))*10^(-SNR/10)-mean_value^2;
noisyimage=imnoise(noisyimage,'gaussian',mean_value,var);
figure(6);
imshow(uint8(noisyimage))
title('Noisy Watermarked Image');

% Extracting the watermark
% Various images to test the robustness of this watermarking scheme
WMI = imread(SavingName); %original test
%WMI = I; %Test with unwatermarked version of lena
%WMI = qtr_scaled_watermark; % test quater scaled watermark
%WMI = noisyimage; % test noisy image watermark


figure(7);
imshow(uint8(WMI))
title('Image for Extraction');

DCTWatermarkedImage=dct2(double(WMI)); %DCT of the watermarked image
Extracted_Data=[1000]; %variable to extracting watermark data
for k=1:Watermark_Size
    %extracting watermark from the watermarked image
    Extracted_Data(k)=((DCTWatermarkedImage(Row_Column_Matrix(k,1),...
    Row_Column_Matrix(k,2))/DCT_Cover_Image(Row_Column_Matrix(k,1),...
    Row_Column_Matrix(k,2))-1)*10);
end

figure(8);
plot(Extracted_Data),title('Extracted Watermark');

% Check watermark similarity
Similarity(Extracted_Data,Watermark)

