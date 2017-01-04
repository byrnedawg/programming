 %--------------------------------------------------------------
    % Compression and uncompression of a grayscale image
    % and computed MSE and PSNR error values.
    %
    % Two measures are commonly used to quantify the error between
    % two images: the Mean Square Error(MSE) and the Peak Signal
    % to Noise Ratio (PSNR) which is expressed in decibels.
    %
    % This example shows first how to compress the mask image
    % using the 'spiht' compression method and save it to the file
    % 'mask.wtc'.
    % Then, it shows how to load the stored image from the file
    % 'mask.wtc', uncompress it and delete the file 'mask.wtc'.
    %--------------------------------------------------------------
    load mask;
    [cr,bpp] = wcompress('c',X,'mask.wtc','spiht','maxloop',12)
    Xc = wcompress('u','mask.wtc');
    delete('mask.wtc')
    D = abs(X-Xc).^2;
    mse = sum(D(:))/numel(X)
    psnr = 10*log10(255*255/mse)
    % Display the original and the compressed image
    colormap(pink(255))
    subplot(1,2,1); image(X); title('Original image'); axis square
    subplot(1,2,2); image(Xc); title('Compressed image'); axis square