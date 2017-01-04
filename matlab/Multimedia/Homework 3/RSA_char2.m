clear all;
close all;

disp('RSA algorithm');
primeval=0; %value for checking if prime is ture
while(primeval==0)
    p=input('Enter prime number for p: ');
    primeval=isprime(p); % check if prime number
end
primeval=0;
while(primeval==0)
    q=input('Enter prime number for q: ');
    primeval=isprime(q); % check if prime number
end
n=p*q;
fprintf('\nn=%d',n);
phi=(p-1)*(q-1);
fprintf('\nphi(%d) = %d',n,phi);
primeval=0;
commondenom=0;

while(commondenom~=1||primeval==0)
%e=randi(n,1);
e=5;
primeval=isprime(e);
commondenom=gcd(e,phi);
end
fprintf('\ne=%d',e);

dephi_mod=0;
d=0;
while(dephi_mod~=1);%find a value of d that multiplied by e mods evenly with phi
d=d+1;
dephi_mod=mod(d*e,phi);
end
fprintf('\nd=%d',d);
fprintf('\nPublic key is (%d,%d)',e,n);
fprintf('\nPrivate key is (%d,%d)',d,n);
input_message=input('\nEnter the message: ','s');
ASCII_message=input_message-0;
disp('ASCII equivalent of message ');
fprintf('\t%d',ASCII_message);
Message_length=length(ASCII_message);
 index=1;
 while(index<=Message_length);
    input_message=ASCII_message(index)-32;
bine=dec2bin(e);
length_bine=length(bine);
idx_encrypted=1;
idxe=1;
while(idxe<=length_bine)  
    if(bine(idxe)=='1')
      idx_encrypted=mod(mod((idx_encrypted^2),n)*input_message,n);
    elseif(bine(idxe)=='0')
        idx_encrypted=(mod(idx_encrypted^2,n));
    end
    idxe=idxe+1;
end
encryptedmessage(index)=idx_encrypted;

bind=dec2bin(d);
length_bind=length(bind);
idx_decrypted=1;
idxd=1;
while(idxd<=length_bind)    
    if(bind(idxd)=='1')
       idx_decrypted=mod(mod((idx_decrypted^2),n)*idx_encrypted,n);
    elseif(bind(idxd)=='0')
        idx_decrypted=(mod(idx_decrypted^2,n));
    end
     idxd=idxd+1;    
end
decryptedmessage(index)=char(idx_decrypted+32);
index=index+1;
 end

 
 
index=1;
fprintf('\nThe encrypted message is \n');
while(index<=Message_length)
   fprintf('\t%d',encryptedmessage(index)); 
   index=index+1;
end
index=1;
fprintf('\nThe decrypted message in ASCII is \n');
while(index<=Message_length)
fprintf('\t%d',decryptedmessage(index));
index=index+1;
end

fprintf('\nThe decrypted message is: ');
disp(decryptedmessage);

fprintf('\n');
