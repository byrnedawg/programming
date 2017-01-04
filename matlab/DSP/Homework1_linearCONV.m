x = [-4 5 1 -2 -3 0 2 0 0 0 0 0];
y = [6 -3 -1 0 8 7 -2 0 0 0];
w = [0 0 0 3 2 2 -1 0 -2 5];
a = [0 1 3 -2 1]
b = [0 1 1 0 0]
clin = conv(a,b);
clin

%stem(clin,'markerfacecolor',[0 0 1]);