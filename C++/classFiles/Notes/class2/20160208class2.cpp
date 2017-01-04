Notes Class 2
02/08/16

char c = 'a';
short s = 3;
int i = -4;
long L = 123L;
long long L2 = 123LL;
unsigned long = 123UL
unsigned int = 123U;
unsigned x = 3U;

float f = 3/2;
double g = 1.5; // IEEE
long double h = 1.5L;
float y = (int)3.6 * 3; // int goes first = 9.0f
float y2 = int(3.6) * 3; // another way to do the same looks like a function
float y3 = int 

// Arrays constant size arrays
int y; //global variable are always 0

int a[100]; // 100 int = 100* sizeof(int) = 400 bytes
int b[500000000]; // too big for windows
int n = 50000000000 // windows hack
int b[n]; //dynamically allocated on the heap
int c[2*5*3];
int d[n]; // int* p = new int[n];
// there is a safe array type Array<int,100> a;
int gr[5]; // values are random garbage
int gr[5] = { 95, 80, 90, 65}; // gr[4] == 0;

// pass by reference
double mean(const int g[], int length)
{
    
}

void stats (const int g[], int length, double& mean, int& max, int& min)
}

// Strings
char a[] = "testing"; //a[7] = '\0' NUL character
a[7] = 'x'; // memory bug!

//n gon problem
n = 5
if odd mult by 3 add 1
if even divede by 2
// look at his one pager on cmath


// notes on reference
int& r = a;
r = 2;
a is now = 2;
// this is illegal
int& q = b;
q = 7;

const int& s = a; // animals analogy
a = 19;

