function SIM=Similarity(Xextracted,Xoriginal)
SIM=sum(Xextracted.*Xoriginal)/sqrt(sum(Xextracted.*Xextracted));
end