 %--------------------------------------------------------------
    % Compression and uncompression of a truecolor image
    % and computed MSE and PSNR error values.
    % Compression parameters are the same as those used for example 3, 
    % but using the 'spiht_3d' method give better performance yet.
    %--------------------------------------------------------------
    X = imread('lena56.bmp');
    [cr,bpp] = wcompress('c',X,'lena56.wtc','spiht','maxloop',8)
    Xc = wcompress('u','lena56.wtc');
    delete('lena56.wtc')
    D = abs(double(X)-double(Xc)).^2;
    mseOrig = sum(D(:))/numel(X)
    psnrORIG = 10*log10(255*255/mseOrig)
    mse = sum(D(:))/numel(X)
    psnr = 10*log10(255*255/mse)
    % Display the original and the compressed image
    subplot(1,2,1); image(X); title('Original image'); axis square
    subplot(1,2,2); image(Xc); title('Compressed image'); axis square