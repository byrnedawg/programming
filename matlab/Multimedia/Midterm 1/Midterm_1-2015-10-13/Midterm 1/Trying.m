
clear all
close all

[fname pthname]=uigetfile('*.jpg;*.png;*.tif;*bmp','Select the Asset Image'); %select image
I=imread([pthname fname]);
wmsz=1000; %watermark size
I=I(:,:,1);%get the first color in case of RGB image
[r,c]=size(I);
D=dct2(I);%get DCT of the Asset
D_vec=reshape(D,1,r*c);%putting all DCT values in a vector
[D_vec_srt,Idx]=sort(abs(D_vec),'descend');%re-ordering all the absolute values


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

HiddenData = double(DataDouble);
W = HiddenData;





%W=randn(1,wmsz);%generate a Gaussian spread spectrum noise to use as watermark signal
Idx2=Idx(2:wmsz+1);%choosing 1000 biggest values other than the DC value
%finding associated row-column order for vector values
IND=zeros(wmsz,2);
for k=1:wmsz
 x=floor(Idx2(k)/r)+1;%associated culomn in the image
 y=mod(Idx2(k),r);%associated row in the image
 IND(k,1)=y;
 IND(k,2)=x;
end
D_w=D;
for k=1:wmsz
 %insert the WM signal into the DCT values
D_w(IND(k,1),IND(k,2))=D_w(IND(k,1),IND(k,2))+.1*D_w(IND(k,1),IND(k,2)).*W(k);
end
I2=idct2(D_w);%inverse DCT to produce the watermarked asset
%The extraction process is simply subtracting the original DCT coefficients from the
%watermarked image ones. The code can be written like below:
W2=[];%will contain watermark signal extracted from the image
for k=1:wmsz
 W2=[W2 (D_w(IND(k,1),IND(k,2))/D(IND(k,1),IND(k,2))-1)*10];%watermark extraction
end

binaryVector = W2;
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