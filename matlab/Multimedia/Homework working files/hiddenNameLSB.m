Original_Image = imread('lena1.bmp');
Original_Image=imresize(Original_Image,[size(Original_Image,1) size(Original_Image,2)],'nearest');
message = 'Gregory Byrne' ; % Write Message to be encoded 
Original_Image(1:1:1)= length(message) ; %to count massage Char to easly retrive all the massage 
message = strtrim(message); %Get rid of white space
%Convert message to Ascii values
m = length(message) * 8;
AsciiCode = uint8(message);
%Convert to Ascii values to binary
binaryString = transpose(dec2bin(AsciiCode,8))
binaryString = binaryString(:);
LengthofMessage = length(binaryString);
b = zeros(LengthofMessage,1);
for k = 1:LengthofMessage
  if(binaryString(k) == '1')
      b(k) = 1;
  else
      b(k) = 0;
  end
end
%alter image with message data
Stego_Image = Original_Image;
  height = size(Original_Image,1);
  width = size(Original_Image,2);
k = 1;
for i = 1 : height
  for j = 1 : width
      LSB = mod(double(Original_Image(i,j)), 2);
      if (k>m || LSB == b(k))
          Stego_Image(i,j) = Original_Image(i,j);
      elseif(LSB == 1)
          Stego_Image(i,j) = (Original_Image(i,j) - 1);
      elseif(LSB == 0)
          Stego_Image(i,j) = (Original_Image(i,j) + 1);
      end
  k = k + 1;
  end
end
ImageWText = 'EncodedImage.bmp';
imwrite(Stego_Image,ImageWText);
%% Retriever code changes:
Stego_Image = imread('EncodedImage.bmp');
height = size(Stego_Image,1);
width = size(Stego_Image,2);
m =  double( Stego_Image(1:1:1) ) * 8  ;
k = 1;
for i = 1 : height
  for j = 1 : width
      if (k <= m)
          b(k) = mod(double(Stego_Image(i,j)),2);
          k = k + 1;
      end
  end
end
binaryVector = b;
binValues = [ 128 64 32 16 8 4 2 1 ];
binaryVector = binaryVector(:);
if mod(length(binaryVector),8) ~= 0
error('Length of binary vector must be a multiple of 8.');
end
binMatrix = reshape(binaryVector,8,[]);
textString = char(binValues*binMatrix);
disp(textString);