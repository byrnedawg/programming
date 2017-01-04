function SIM=WM_detect(Xextracted,Xoriginal)
SIM=sum(Xextracted.*Xoriginal)/sqrt(sum(Xextracted.*Xextracted));
end