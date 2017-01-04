{"filter":false,"title":"bisection.cpp","tooltip":"/data_struct_hw/class_programs/bisection.cpp","undoManager":{"mark":33,"position":33,"stack":[[{"start":{"row":0,"column":0},"end":{"row":39,"column":1},"action":"insert","lines":["#include <iostream>","#include <cmath>","using namespace std;","","double f(double x) { return x*x - 7; }","typedef double (*FuncOneVar)(double x);","","double bisection(FuncOneVar f, double a, double b, double eps) {","\tdouble ya = f(a), yb = f(b);","\tif (ya * yb >= 0) {","\t\tcerr << \"possibly no solution?\\n\";","\t\texit(-1);","\t}","\t// we now have two points one above, one below the x axis","\t","\tif (ya > yb) {","\t\tswap(a, b);","\t}","\t// f(a) < 0 < f(b)","\tdouble x;","","\tdo {","\t\tx = (a+b)/2;","\t\tif (f(x) > 0)","\t\t\tb = x;","\t\telse if (f(x) < 0)","\t\t\ta = x;","\t\telse","\t\t\treturn x;","\t} while ( abs(b - a) > eps);","\t//\tguess improves by a factor of 2 = 1 bit","}","","","","","","\t","\t","}"],"id":1}],[{"start":{"row":39,"column":0},"end":{"row":39,"column":1},"action":"remove","lines":["}"],"id":2}],[{"start":{"row":4,"column":38},"end":{"row":5,"column":0},"action":"insert","lines":["",""],"id":3}],[{"start":{"row":33,"column":0},"end":{"row":34,"column":0},"action":"insert","lines":["",""],"id":4}],[{"start":{"row":34,"column":0},"end":{"row":34,"column":1},"action":"insert","lines":["i"],"id":5}],[{"start":{"row":34,"column":1},"end":{"row":34,"column":2},"action":"insert","lines":["n"],"id":6}],[{"start":{"row":34,"column":2},"end":{"row":34,"column":3},"action":"insert","lines":["t"],"id":7}],[{"start":{"row":34,"column":3},"end":{"row":34,"column":4},"action":"insert","lines":[" "],"id":8}],[{"start":{"row":34,"column":4},"end":{"row":34,"column":5},"action":"insert","lines":["m"],"id":9}],[{"start":{"row":34,"column":5},"end":{"row":34,"column":6},"action":"insert","lines":["a"],"id":10}],[{"start":{"row":34,"column":6},"end":{"row":34,"column":7},"action":"insert","lines":["i"],"id":11}],[{"start":{"row":34,"column":7},"end":{"row":34,"column":8},"action":"insert","lines":["n"],"id":12}],[{"start":{"row":34,"column":8},"end":{"row":34,"column":10},"action":"insert","lines":["()"],"id":13}],[{"start":{"row":34,"column":10},"end":{"row":35,"column":0},"action":"insert","lines":["",""],"id":14}],[{"start":{"row":35,"column":0},"end":{"row":35,"column":1},"action":"insert","lines":["{"],"id":15}],[{"start":{"row":35,"column":1},"end":{"row":37,"column":1},"action":"insert","lines":["","    ","}"],"id":16}],[{"start":{"row":36,"column":4},"end":{"row":36,"column":5},"action":"insert","lines":["c"],"id":17}],[{"start":{"row":36,"column":5},"end":{"row":36,"column":6},"action":"insert","lines":["o"],"id":18}],[{"start":{"row":36,"column":6},"end":{"row":36,"column":7},"action":"insert","lines":["u"],"id":19}],[{"start":{"row":36,"column":7},"end":{"row":36,"column":8},"action":"insert","lines":["t"],"id":20}],[{"start":{"row":36,"column":8},"end":{"row":36,"column":9},"action":"insert","lines":[" "],"id":21}],[{"start":{"row":36,"column":9},"end":{"row":36,"column":10},"action":"insert","lines":["<"],"id":22}],[{"start":{"row":36,"column":10},"end":{"row":36,"column":11},"action":"insert","lines":["<"],"id":23}],[{"start":{"row":36,"column":11},"end":{"row":36,"column":12},"action":"insert","lines":[" "],"id":24}],[{"start":{"row":36,"column":12},"end":{"row":36,"column":13},"action":"insert","lines":["b"],"id":25}],[{"start":{"row":36,"column":13},"end":{"row":36,"column":14},"action":"insert","lines":["e"],"id":26}],[{"start":{"row":36,"column":13},"end":{"row":36,"column":14},"action":"remove","lines":["e"],"id":27}],[{"start":{"row":36,"column":13},"end":{"row":36,"column":14},"action":"insert","lines":["i"],"id":28}],[{"start":{"row":36,"column":12},"end":{"row":36,"column":14},"action":"remove","lines":["bi"],"id":29},{"start":{"row":36,"column":12},"end":{"row":36,"column":23},"action":"insert","lines":["bisection()"]}],[{"start":{"row":36,"column":22},"end":{"row":36,"column":23},"action":"insert","lines":["f"],"id":30}],[{"start":{"row":36,"column":22},"end":{"row":36,"column":23},"action":"remove","lines":["f"],"id":31}],[{"start":{"row":36,"column":23},"end":{"row":36,"column":24},"action":"insert","lines":[";"],"id":32}],[{"start":{"row":0,"column":0},"end":{"row":44,"column":0},"action":"remove","lines":["#include <iostream>","#include <cmath>","using namespace std;","","double f(double x) { return x*x - 7; }","","typedef double (*FuncOneVar)(double x);","","double bisection(FuncOneVar f, double a, double b, double eps) {","\tdouble ya = f(a), yb = f(b);","\tif (ya * yb >= 0) {","\t\tcerr << \"possibly no solution?\\n\";","\t\texit(-1);","\t}","\t// we now have two points one above, one below the x axis","\t","\tif (ya > yb) {","\t\tswap(a, b);","\t}","\t// f(a) < 0 < f(b)","\tdouble x;","","\tdo {","\t\tx = (a+b)/2;","\t\tif (f(x) > 0)","\t\t\tb = x;","\t\telse if (f(x) < 0)","\t\t\ta = x;","\t\telse","\t\t\treturn x;","\t} while ( abs(b - a) > eps);","\t//\tguess improves by a factor of 2 = 1 bit","}","","int main()","{","    cout << bisection();","}","","","","","\t","\t",""],"id":33},{"start":{"row":0,"column":0},"end":{"row":77,"column":1},"action":"insert","lines":["bisection.cc","Details","Activity","bisection.cc","Sharing Info","D","General Info","Type","C++","Size","906 bytes","Storage used","0 bytesOwned by someone else","Location","Session04 - ShufflingSearching","Owner","Dov Kruger","Modified","Sep 29, 2015 by Dov Kruger","Opened","10:45 PM by me","Created","Sep 29, 2015","Description","No description","Download permissions","Viewers can download","All selections cleared","","","#include <iostream>","#include <iomanip>","#include <utility>","#include <cmath>","using namespace std;","","#if 0","","inline void swap(double& a, double& b) {","\tdouble temp = a;","\ta = b;","\tb = temp;","}","#endif","double f(double x) { return x*x - 2; } // root is sqrt(2)","","typedef double (*FuncOneVar)(double x);","","double bisection(FuncOneVar f, double a, double b, double eps) {","\tif (f(a) > 0 && f(b) < 0) {","\t\tswap(a,b);","\t}","\tif (f(a) * f(b) > 0) {","\t\tthrow \"No Root!\";","\t}","  double guess;","\tdo {","\t\tguess = (a + b) / 2;","\t\tdouble y = f(guess);","\t\tif (y > 0) {","\t\t\tb = guess;","\t\t} else if (y < 0) {","\t\t\ta = guess;","\t\t}","\t} while (fabs(b - a) > eps);","\treturn (a + b)/2;","}","","int main() {","\ttry {","\t\tdouble eps = 0.1;","\t\tfor (int i = 1; i <= 18; i++, eps *= 0.1)","\t\t\tcout << setprecision(15) << eps << '\\t' << bisection(f, 0, 3, eps) << '\\n'; ","","\t} catch(const char* msg) {","\t\tcout << msg << '\\n';","\t}","}"]}],[{"start":{"row":0,"column":0},"end":{"row":29,"column":0},"action":"remove","lines":["bisection.cc","Details","Activity","bisection.cc","Sharing Info","D","General Info","Type","C++","Size","906 bytes","Storage used","0 bytesOwned by someone else","Location","Session04 - ShufflingSearching","Owner","Dov Kruger","Modified","Sep 29, 2015 by Dov Kruger","Opened","10:45 PM by me","Created","Sep 29, 2015","Description","No description","Download permissions","Viewers can download","All selections cleared","",""],"id":35}]]},"ace":{"folds":[],"scrolltop":0,"scrollleft":0,"selection":{"start":{"row":15,"column":48},"end":{"row":15,"column":48},"isBackwards":false},"options":{"guessTabSize":true,"useWrapMode":false,"wrapToView":true},"firstLineState":0},"timestamp":1474425959625,"hash":"fdc9bdc181ad174f15dd1283f98b9b707298510a"}