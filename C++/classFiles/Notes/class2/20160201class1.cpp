Notes Class 1
02/01/16

Data types
 bool        typically one byte   8 bits
 char        1 byte               8 bits     
 short       >= 16 bits           2 bytes
 int         whatever is fastest, >= 16 bits              2 - 4 bytes
 long        >= int, >= 32 bits                           4 bytes
 long long   64 bits                                      8 bytes
 
 fractional Data types
 float       typically 32 bits IEEE standard             4 bytes
 double      typically 64 bits IEEE standard             8 bytes
 long double optional may be identical to double 64 bits - 128 bits    16 bytes
 
 
 Type	Typical Bit Width	Typical Range
char	1 byte	                             -127 to 127 or 0 to 255
unsigned char	1 byte	                       0 to 255
signed char	1 byte	                      -127 to 127
int	4 bytes	                   -2,147,483,648 to 2,147,483,647
unsigned int	4 bytes	                       0 to 4,294,967,295
signed int	4 bytes	            -2,147,483,648 to 2,147,483,647
short int	2 bytes	                    -32,768 to 32,767
unsigned short int	                  Range	0 to 65,535
signed short int	              Range	-32,768 to 32,767
long int	4 bytes	              -2,147,483,648 to 2,147,483,647
signed long int	4 bytes	same as long int
unsigned long int	4 bytes	                  0 to 4,294,967,295
float	4 bytes	                       +/- 3.4e +/- 38 (~7 digits)
double	8 bytes	                      +/- 1.7e +/- 308 (~15 digits)
long double	8 bytes	                 +/- 1.7e +/- 308 (~15 digits)
wchar_t	2 or 4 bytes	1 wide character