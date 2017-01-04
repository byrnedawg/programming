#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>
using namespace std;

class Bitmap {
private:
    uint32_t* bitmap;
    int w, h;
    Bitmap(const Bitmap& orig); // no copying!
    Bitmap& operator =(const Bitmap& orig); // no copying!
    uint32_t getXY(int x, int y) {
        return bitmap[y * w + x];
    }
    
public:
	typedef uint32_t Color;
	Bitmap(int w, int h, uint32_t rgba) : w(w), h(h) { // TODO: ADD INITIALIZER LIST
		bitmap = new uint32_t[w*h];
		for(int i = 0; i < h; i++ ){
		    for(int j = 0; j < w; j++){    
		        
		        bitmap[i * w + j] = rgba;
   
		    }
		}
	}
		
	~Bitmap() {
		delete [] bitmap;
	}
	void hline(uint32_t x1, uint32_t x2, uint32_t y, Color rgba) {
	
		if(x1 > x2)
  		{
    		swap(x1, x2);
  		}
		for(int i = x1; i <= x2; i++ ){
			bitmap[y * w + i] = rgba;
		}
	}
	void vline(uint32_t y1, uint32_t y2, uint32_t x, Color rgba) {
	
		if(y1 > y2)
  		{
    		swap(y1, y2);
  		}
		for(int i = y1; i <= y2; i++ ){
			bitmap[i * w + x] = rgba;
		}
	}
	//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	void line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, Color rgba) {
			
		uint32_t dx = x2 - x1;
		uint32_t dy = y2 - y1;
		uint32_t D = 2*dy - dx;
		  
		// plot(x0,y0)
		point(x1, y1, rgba);
		//y = y0
		uint32_t y = y1;
		  
		if (D > 0){
			y = y+1;
		}
		D = D - (2*dx);
	
		//for x from x0+1 to x1
		for(uint32_t i = x1+1; i <= x2; i++){
			point(i, y, rgba);
		    D = D + (2*dy);
		    if(D > 0){
		    	y = y+1;
		    }
		      	D = D - (2*dx);
		}
	}
	
	void Line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, Color rgba){
        // Bresenham's line algorithm
  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
  if(steep)
  {
    swap(x1, y1);
    swap(x2, y2);
  }
 
  if(x1 > x2)
  {
    swap(x1, x2);
    swap(y1, y2);
  }
 
  const float dx = x2 - x1;
  const float dy = fabs(y2 - y1);
 
  float error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  uint32_t y = y1;
 
 
  for(uint32_t x = x1; x <= x2; x++)
  {
    if(steep)
    {
        //SetPixel(y,x, color);
        point(y, x, rgba);
    }
    else
    {
        //SetPixel(x,y, color);
        point(x, y, rgba);
    }
 
    error -= dy;
    if(error < 0)
    {
        y += ystep;
        error += dx;
    }
  }
  //point(x2, y2, rgba);
}
	void triangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
								Color rgba) {
	}

	// first interpolate color on the edges
	// then scan across left to right, interpolating color
	void gouraud(uint32_t x1, uint32_t y1, Color c1,
							 uint32_t x2, uint32_t y2, Color c2,
							 uint32_t x3, uint32_t y3, Color c3) {
		
	}

	void fillRect(int x, int y, int w, int h, int rgba) {
	   
	    for(int i = y; i < h+y; i++ ){
		    for(int j = x; j < w+x; j++){    
		        bitmap[i * this->w + j] = rgba;
		    }
	    }
	}
	
	void point(int x, int y, int rgba) {
	//	bitmap[getXY(x,y)] = rgba;
		bitmap[y * w + x] = rgba;
	}
	void fillEllipse(int x, int y, int w, int h, int rgba) {
		int hh = h * h;
		int ww = w * w;
		int hhww = hh * ww;
		int x0 = w;
		int dx = 0;

		// do the horizontal diameter
		for (int i = 0; i <= w; i++)
		    point(x + i, y, rgba);
		
		// now do both halves at the same time, away from the diameter
		for (int j = 0; j <= h; j++)
		{
		    int k = x0 - (dx - 1);  // try slopes of dx - 1 or more
		    for ( ; k > 0; k--)
		        if (k*k*hh + j*j*ww <= hhww)
		            break;
		    dx = x0 - k;  // current approximation of the slope
		    x0 = k;
		
		    for (int i = 0; i <= x0; i++)
		    {
		         point(x + i, y - j, rgba);
		         point(x + i, y + j, rgba);
		    }
		}
		
	}
	

	friend ostream& operator <<(ostream& s, const Bitmap& b) {
		for(int i = 0; i < b.h; i++){
			for(int j = 0; j < b.w; j++){
				s <<b.bitmap[i * b.w + j] << '\t';
    		}
    		s << '\n';
    	}
    	s << '\n';
    	return s;
	}
};

int main() {
    Bitmap b(10, 14, 255); // w, h, default color
    cout << b << '\n';
    b.fillRect(4, 3, 3, 6, 0); //  b.fillRect(3, 4, 7, 2, 0);
    b.fillRect(2, 4, 2, 4, 0);
    b.fillRect(7, 4, 2, 4, 0);
    b.hline(1,5,7,53);
    b.vline(4,11,8,54);
    b.point(8,12, 11); // bottom right point
    b.Line(1,1,8,1,13);
    b.fillEllipse(5, 7, 3, 3, 93);
    cout << b << '\n';
   // cout << "value at 9,3" << getXY(9,3) << "/n";
    return 0;
}
