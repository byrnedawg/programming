function MSE_value = MSE(x,y)
   
  
  [r,c] = size(x);
  N = r * c;
  MSE_value  = sum( sum( (y - x).*(y - x)  ) ) / N;
