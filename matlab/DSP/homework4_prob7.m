Wp = 1500/10000;
Ws = 6000/10000;

[n,Wn] = buttord(Wp,Ws,.25,25)

Wp2 = 1000/10000;
Ws2 = 5000/10000;

[n2,Wn2] = buttord(Wp2,Ws2,1,40)

Wp = [60 200]/1000;
Ws = [50 250]/1000;
Rp = 3;
Rs = 40;
[n,Wn] = buttord(Wp,Ws,Rp,Rs)