clear all;
close all;

disp('Homework 3 RSA algorithm');%display name of program
primeval=0; %value for checking if prime is true
while(primeval==0)
    p=input('Enter prime number for p: ');
    primeval=isprime(p); % check if prime number
end
primeval=0;
while(primeval==0)
    q=input('Enter prime number for q: ');
    primeval=isprime(q); % check if prime number
end
n=p*q; %calculate n
fprintf('\nn=%d',n); %display n
phi=(p-1)*(q-1); %calculate phi
fprintf('\nphi(%d) = %d',n,phi); %display phi

primeval=0; %value for checking if prime is true 
commondenom=0; %value for checking common denominator

while(commondenom~=1||primeval==0)
e=randi(n,1); %find random e value up to n
primeval=isprime(e); %check if random e is prime
commondenom=gcd(e,phi); %check if greatest commin denominator of e = 1
end
fprintf('\ne=%d',e); %display random value of e found

dephi_mod=0; %variable for checking the mod = 0
d=0; %variable for storing the calculated value of d
while(dephi_mod~=1);%find a value of d that multiplied by e mods evenly with phi
d=d+1; %increment d
dephi_mod=mod(d*e,phi); %try next value of d to see if it satisfies condition
end
fprintf('\nd=%d',d); %display d
fprintf('\nPublic key is (%d,%d)',e,n); %display public key
fprintf('\nPrivate key is (%d,%d)',d,n); %display private key
input_message=input('\nEnter the message: ','s'); %prompt for message
ASCII_message=input_message;
disp('ASCII equivalent of message '); %display message
fprintf('\t%d',ASCII_message); %display message
Message_length=length(ASCII_message); %get message lenght
 index=1; %index to go through message element by element
 while(index<=Message_length);
    inputmessage(index)= ASCII_message(index)-32; %scale ascii values
    CipherText(index) = powermod(inputmessage(index),e,n); %powermod msg e
    index=index+1; %increment index
 end
 
fprintf('\n');
disp('Cipher Text '); %display ciphertext
fprintf('\t%d',CipherText);

Cipher_length=length(CipherText); %get length of cipher
index2=1; %index for going through the ciphertext element by element
 while(index2<=Cipher_length);
    origText(index2) = powermod(CipherText(index2),d,n);%powermod cipher d
    outputText(index2) = origText(index2)+32; %rescale ascii values
    index2=index2+1; %increment index
 end
fprintf('\n');
disp('Decrypted message back to ASCII '); %display decrypted ascii message
fprintf('\t%d',outputText);
fprintf('\nThe decrypted message is: '); %display message as char
disp(char(outputText));
