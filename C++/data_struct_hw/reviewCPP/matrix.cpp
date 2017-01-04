// Name: Gregory Byrne
// Assignment: HW8 - Matrix
#include <iostream>
#include <memory.h>
using namespace std;
class Matrix {
private:
	int rows, cols;
	double* m;
public:
	Matrix(int row, int col) {
	    rows = row;
	    cols = col;
	    m = new double[rows*cols];
		for(int i = 0; i < rows; i++ ){
		    for(int j = 0; j < cols; j++){    
		        
		        m[i * cols + j] = 0;
		        
		    }
		}
	}
	
	void print(){ 
       for (int i = 0; i < rows; i++){
    		for(int j = 0; j < cols; j++){ 
    	         cout << m[i * cols + j] << '\t';
    		}
    		cout << '\n';
    	}
     } 
     
     double operator()( int i, int j ) const
     {
         return m[ i * cols + j ];
     }
     double& operator()( int i, int j ) 
     {
         return m[ i * cols + j ];
     }
   
	// move constructor	
	Matrix(Matrix&& orig) {
		m = orig.m;
		rows = orig.rows;
		cols = orig.cols;
		orig.m = nullptr;
	}
	
	
	//copy constructor
	Matrix(const Matrix& orig) {
		rows = orig.rows;
		cols = orig.cols;
		m = new double[rows * cols];
		for(int i = 0; i < rows; i++ ){
		    	for(int j = 0; j < cols; j++){    
		        
		    	 	m[i * cols + j] = orig.m[i * cols + j];
		    	}
			}
		memcpy(m, orig.m, rows * cols);
		
	}

	// destructor
	~Matrix() {
	    delete [] m;
	}
	
	Matrix& operator =(const Matrix& orig) {
		if (this != &orig) {
			delete [] m;
			rows = orig.rows;
			cols = orig.cols;
			m = new double[rows * cols];
			for(int i = 0; i < rows; i++ ){
		    	for(int j = 0; j < cols; j++){    
		        
		    	 	m[i * cols + j] = orig.m[i * cols + j];
		    	}
			}
			memcpy(m, orig.m, rows*cols);
		}
		return *this;
		
	}

    friend ostream& operator <<(ostream& s, const Matrix& mat) {
    		for (int i = 0; i < mat.rows; i++){
    			for(int j = 0; j < mat.cols; j++){ 
    			    s << mat.m[i * mat.cols + j] << '\t';
    			}
    			s << '\n';
    		}
    		s << '\n';
    	    return s;
    }
	
};

int main() {
	Matrix a(3, 3);  // 0 0 0
	                 // 0 0 0
	cout << "a= \n" << a;
	a(0,0) = 5.5;    // 5.5 0.0 0.0
		               // 0.0 0.0 0.0
	cout << "a= \n" << a;
	Matrix b = a;
	cout << "b= \n" << b;
	a(0,1) = 2.5;
	cout << "a= \n" << a;
	b = a;
	cout << "b= \n" << b;
	b(1,2) = 5.3;
	cout << "b= \n" << b;
	cout << "b(1,2) = " << b(1,2) << "\n";
	Matrix c(10,11);
	c(4,4) = 4.8;
	cout << "c= \n" << c; 
	Matrix d = b;
	cout << "d = \n" << d;
	cout << "a = \n" << a;
	Matrix e = move(c);
	cout << "e = c \n" << e;
return 0;
}
