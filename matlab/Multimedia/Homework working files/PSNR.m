function psnr_value = PSNR(x,y)
   
  
  [r,c] = size(x);
  N = r * c;
  psnr_denominator  = sum( sum( (y - x).*(y - x)  ) ) / N;
  psnr_value = 10 * log10(255*255/psnr_denominator);



