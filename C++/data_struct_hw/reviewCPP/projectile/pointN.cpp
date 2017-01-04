#include <iostream>

using namespace std;

class PointN
{
    public:
        int n;
        double x[];

    PointN(int n)
    {
        this.n = n;
        x = new double[n];
    }

    PointN(PointN a)
    {
        n = a.n;
        x = new double[n];
        for (int i = 0; i < n; i++)
        x[i] = a.x[i];
    }
    
    double this[int i]
{
    get
    {
        if (i < 0 || i >= n)
            return 0;
        else
            return x[i]; 
    }
    set
    {
        if (i >= 0 && i < n)
            x[i] = value;
    }
}
};