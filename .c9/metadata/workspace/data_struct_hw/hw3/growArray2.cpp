{"filter":false,"title":"growArray2.cpp","tooltip":"/data_struct_hw/hw3/growArray2.cpp","undoManager":{"mark":65,"position":65,"stack":[[{"start":{"row":0,"column":0},"end":{"row":210,"column":0},"action":"insert","lines":["#include <iostream>","using namespace std;","","class GrowArray {","private:","\tint* p;","\tint size;","\tint used;","\t// this should double the size of your list","\tvoid grow() ","\t{","\t\tsize = size * 2;","\t}","public:","\tGrowArray() : p(new int[1]), size(1), used(0) {}","\tGrowArray(int capacity) : p(new int[capacity]), size(capacity), used(0) {}","\t~GrowArray() ","\t{","\t\tdelete [] p;","\t}","\t","\tvoid addEnd(int v)  //O(    )"," \t{  ","\t\tif (used == size)","\t\t\tgrow();","\t\t\t","\t\tint* temp = p; // O(1)","\t\t","    \tp = new int[used+1]; // O(1)","\t\t\tfor (int i = 0; i < used; i++)","\t\t\t{","\t\t\t\tp[i] = temp[i];","\t\t\t}","\t\tp[used] = v;","\t\tused++;","","\t}","\t","\tvoid addStart(int v) // O(   )","\t{ ","\t\tif (used == size)","\t\t\tgrow();","\t\t\t","\t\tint* temp = p;","\t\tp = new int[used+1];","\t\tfor (int i = 0; i < used; i++) //O(n)","\t\t\tp[i+1] = temp[i];","\t\tp[0] = v;","\t\tused++;","\t}","\t","\tvoid removeEnd() { //O(1)","\t\tused--;","\t}","\t","\tvoid removeStart() { //O(n)","\t\tfor (int i = 0; i < used-1; i++)","\t\t\tp[i] = p[i+1];","\t\tused--;","\t}","\t","\tint getCapacity() const ","\t{","    \treturn size;\t\t","\t}","","\tint getUsed() const ","\t{","\t\treturn used;","\t}","\t","\tint get(int i) const //O(1)","\t{","\t\tif (i < 0 || i >= used)","\t\t\tthrow \"Out of Bounds\";","\t\treturn p[i];","\t}","\t","\tvoid set(int i, int v) // O(1)","\t{ ","\t\tif (i < 0 || i >= used)","\t\t\t\tthrow \"Out of Bounds\";","\t\tp[i] = v;","\t}","\t","\tint operator[](int i) const ","\t{","\t\tif (i < 0 || i >= used)","\t\t\tthrow \"Out of Bounds\";","\t\treturn p[i];","\t}","\t","\tint& operator[](int i) ","\t{","\t\tif (i < 0 || i >= used)","\t\t\tthrow \"Out of Bounds\";","\t\treturn p[i];","\t}","\t","\tfriend ostream& operator<<(ostream& s, const GrowArray& gArray) ","\t{","\t\ts << \"{ \";","\t\tfor (int i = 0; i < gArray.used; i++) {","\t\t\ts << gArray[i] << ' ';","\t\t}","\t\ts << \"}\";","\t\treturn s;","\t}","\t","};","","class BadGrowArray {","private:","\tint* data;","\tint size;","public:","\tBadGrowArray() {","\t\tdata = nullptr; // O(1)","\t\tsize = 0;","\t}","\tBadGrowArray(int initialSize) {","\t\tdata = new int[initialSize]; //O(1)","\t\tsize = initialSize;","\t}","\t~BadGrowArray() {","\t\tdelete [] data;","\t}","\tvoid addEnd(int v) {   //O(n)","\t\tint* temp = data; // O(1)","\t\t","    data = new int[size+1]; // O(1)","\t\tfor (int i = 0; i < size; i++) //O(n)","\t\t\tdata[i] = temp[i];","\t\tdata[size] = v;","\t\tsize++;","\t}","\tvoid addStart(int v) { //O(n)","\t\tint* temp = data;","\t\tdata = new int[size+1];","\t\tfor (int i = 0; i < size; i++) //O(n)","\t\t\tdata[i+1] = temp[i];","\t\tdata[0] = v;","\t\tsize++;","\t}","\tvoid removeEnd() { //O(1)","\t\tsize--;","\t}","\tvoid removeStart() { //O(n)","\t\tfor (int i = 0; i < size-1; i++)","\t\t\tdata[i] = data[i+1];","\t\tsize--;","\t}","\t","\tint operator[](int i) const ","\t{","\t\treturn data[i];","\t}","\t","\tint& operator[](int i) ","\t{","\t\treturn data[i];","\t}","\t","\tfriend ostream& operator<<(ostream& s, const BadGrowArray& badArray) ","\t{","\t\ts << \"{ \";","\t\tfor (int i = 0; i < badArray.size; i++) {","\t\t\ts << badArray[i] << ' ';","\t\t}","\t\ts << \"}\";","\t\treturn s;","\t}","};","","int main() ","{","\tBadGrowArray a;","\tGrowArray b;","\tGrowArray c(23);","\tfor (int i = 0; i < 50; i++)","\t{","\t\ta.addEnd(i);","\t\ta.addStart(i+2);","\t\tb.addEnd(i);","\t}","\ta[5] = 201;","\tcout << a << \"\\n\\n\\n\";","\tcout << b << \"\\n\";","\tcout << \"Storage used = \" << b.getUsed() << \"\\tCapacity = \" << b.getCapacity() << \"\\n\";","\t","\tfor(int j = 0; j < 7; j++)","\t{","\t\tb.addStart(13);","\t\tb.addStart(53);","\t}","\tb.addStart(74);","\tcout << b << \"\\n\";","\tcout << \"Storage used = \" << b.getUsed() << \"\\tCapacity = \" << b.getCapacity() << \"\\n\";","\tb.removeStart();","\tb.removeEnd();","\tcout << b << \"\\n\";","\tcout << \"Storage used = \" << b.getUsed() << \"\\tCapacity = \" << b.getCapacity() << \"\\n\";","\tc.addStart(13);","\tc.addStart(53);","\tcout << c << \"\\n\";","\tcout << \"Storage used = \" << c.getUsed() << \"\\tCapacity = \" << c.getCapacity() << \"\\n\";","\tcout << c[3];","}","","",""],"id":1}],[{"start":{"row":206,"column":0},"end":{"row":206,"column":14},"action":"remove","lines":["\tcout << c[3];"],"id":2}],[{"start":{"row":205,"column":88},"end":{"row":206,"column":0},"action":"remove","lines":["",""],"id":3}],[{"start":{"row":26,"column":2},"end":{"row":32,"column":4},"action":"remove","lines":["int* temp = p; // O(1)","\t\t","    \tp = new int[used+1]; // O(1)","\t\t\tfor (int i = 0; i < used; i++)","\t\t\t{","\t\t\t\tp[i] = temp[i];","\t\t\t}"],"id":4}],[{"start":{"row":26,"column":1},"end":{"row":26,"column":2},"action":"remove","lines":["\t"],"id":5}],[{"start":{"row":26,"column":0},"end":{"row":26,"column":1},"action":"remove","lines":["\t"],"id":6}],[{"start":{"row":25,"column":3},"end":{"row":26,"column":0},"action":"remove","lines":["",""],"id":7}],[{"start":{"row":196,"column":10},"end":{"row":196,"column":11},"action":"remove","lines":["t"],"id":12}],[{"start":{"row":196,"column":9},"end":{"row":196,"column":10},"action":"remove","lines":["r"],"id":13}],[{"start":{"row":196,"column":8},"end":{"row":196,"column":9},"action":"remove","lines":["a"],"id":14}],[{"start":{"row":196,"column":7},"end":{"row":196,"column":8},"action":"remove","lines":["t"],"id":15}],[{"start":{"row":196,"column":6},"end":{"row":196,"column":7},"action":"remove","lines":["S"],"id":16}],[{"start":{"row":196,"column":6},"end":{"row":196,"column":7},"action":"insert","lines":["E"],"id":17}],[{"start":{"row":196,"column":7},"end":{"row":196,"column":8},"action":"insert","lines":["N"],"id":18}],[{"start":{"row":196,"column":8},"end":{"row":196,"column":9},"action":"insert","lines":["D"],"id":19}],[{"start":{"row":196,"column":8},"end":{"row":196,"column":9},"action":"remove","lines":["D"],"id":20}],[{"start":{"row":196,"column":7},"end":{"row":196,"column":8},"action":"remove","lines":["N"],"id":21}],[{"start":{"row":196,"column":7},"end":{"row":196,"column":8},"action":"insert","lines":["d"],"id":22}],[{"start":{"row":196,"column":7},"end":{"row":196,"column":8},"action":"remove","lines":["d"],"id":23}],[{"start":{"row":196,"column":6},"end":{"row":196,"column":7},"action":"remove","lines":["E"],"id":24}],[{"start":{"row":196,"column":6},"end":{"row":196,"column":7},"action":"insert","lines":["e"],"id":25}],[{"start":{"row":196,"column":7},"end":{"row":196,"column":8},"action":"insert","lines":["n"],"id":26}],[{"start":{"row":196,"column":8},"end":{"row":196,"column":9},"action":"insert","lines":["="],"id":27}],[{"start":{"row":196,"column":8},"end":{"row":196,"column":9},"action":"remove","lines":["="],"id":28}],[{"start":{"row":196,"column":7},"end":{"row":196,"column":8},"action":"remove","lines":["n"],"id":29}],[{"start":{"row":196,"column":6},"end":{"row":196,"column":7},"action":"remove","lines":["e"],"id":30}],[{"start":{"row":196,"column":6},"end":{"row":196,"column":7},"action":"insert","lines":["E"],"id":31}],[{"start":{"row":196,"column":3},"end":{"row":196,"column":7},"action":"remove","lines":["addE"],"id":32},{"start":{"row":196,"column":3},"end":{"row":196,"column":9},"action":"insert","lines":["addEnd"]}],[{"start":{"row":198,"column":88},"end":{"row":199,"column":0},"action":"insert","lines":["",""],"id":33},{"start":{"row":199,"column":0},"end":{"row":199,"column":1},"action":"insert","lines":["\t"]}],[{"start":{"row":199,"column":1},"end":{"row":200,"column":0},"action":"insert","lines":["",""],"id":34},{"start":{"row":200,"column":0},"end":{"row":200,"column":1},"action":"insert","lines":["\t"]}],[{"start":{"row":200,"column":1},"end":{"row":213,"column":38},"action":"insert","lines":[" unsigned long long testNumber;","  ifstream myfile (\"hw2.dat\");","  if (myfile.is_open()){","    while  (myfile >> testNumber){","      cout <<  testNumber << \"\\t\";","      if(millerRabin(testNumber)){","        cout << \"True\\n\";","      }","      else cout << \"False\\n\";","    }","    myfile.close();","  }","","  else cout << \"Unable to open file\"; "],"id":35}],[{"start":{"row":200,"column":1},"end":{"row":200,"column":2},"action":"remove","lines":[" "],"id":36}],[{"start":{"row":200,"column":0},"end":{"row":200,"column":1},"action":"remove","lines":["\t"],"id":37}],[{"start":{"row":200,"column":0},"end":{"row":200,"column":4},"action":"insert","lines":["    "],"id":38}],[{"start":{"row":201,"column":2},"end":{"row":201,"column":4},"action":"insert","lines":["  "],"id":39}],[{"start":{"row":202,"column":2},"end":{"row":202,"column":4},"action":"insert","lines":["  "],"id":40}],[{"start":{"row":203,"column":4},"end":{"row":203,"column":8},"action":"insert","lines":["    "],"id":41}],[{"start":{"row":204,"column":6},"end":{"row":204,"column":8},"action":"insert","lines":["  "],"id":42}],[{"start":{"row":204,"column":8},"end":{"row":204,"column":12},"action":"insert","lines":["    "],"id":43}],[{"start":{"row":203,"column":37},"end":{"row":204,"column":0},"action":"insert","lines":["",""],"id":44},{"start":{"row":204,"column":0},"end":{"row":204,"column":8},"action":"insert","lines":["        "]}],[{"start":{"row":206,"column":6},"end":{"row":206,"column":8},"action":"insert","lines":["  "],"id":45}],[{"start":{"row":206,"column":8},"end":{"row":206,"column":12},"action":"insert","lines":["    "],"id":46}],[{"start":{"row":207,"column":8},"end":{"row":207,"column":12},"action":"insert","lines":["    "],"id":47}],[{"start":{"row":207,"column":12},"end":{"row":207,"column":16},"action":"insert","lines":["    "],"id":48}],[{"start":{"row":206,"column":39},"end":{"row":207,"column":0},"action":"insert","lines":["",""],"id":49},{"start":{"row":207,"column":0},"end":{"row":207,"column":12},"action":"insert","lines":["            "]}],[{"start":{"row":209,"column":6},"end":{"row":209,"column":8},"action":"insert","lines":["  "],"id":50}],[{"start":{"row":209,"column":8},"end":{"row":209,"column":12},"action":"insert","lines":["    "],"id":51}],[{"start":{"row":210,"column":6},"end":{"row":210,"column":8},"action":"insert","lines":["  "],"id":52}],[{"start":{"row":210,"column":8},"end":{"row":210,"column":12},"action":"insert","lines":["    "],"id":53}],[{"start":{"row":210,"column":17},"end":{"row":211,"column":0},"action":"insert","lines":["",""],"id":54},{"start":{"row":211,"column":0},"end":{"row":211,"column":12},"action":"insert","lines":["            "]}],[{"start":{"row":211,"column":12},"end":{"row":211,"column":16},"action":"insert","lines":["    "],"id":55}],[{"start":{"row":212,"column":4},"end":{"row":212,"column":8},"action":"insert","lines":["    "],"id":56}],[{"start":{"row":212,"column":8},"end":{"row":212,"column":12},"action":"insert","lines":["    "],"id":57}],[{"start":{"row":206,"column":11},"end":{"row":212,"column":13},"action":"remove","lines":[" if(millerRabin(testNumber))","            {","                cout << \"True\\n\";","            }","            else ","                cout << \"False\\n\";","            }"],"id":58},{"start":{"row":206,"column":11},"end":{"row":206,"column":12},"action":"insert","lines":["\\"]}],[{"start":{"row":206,"column":11},"end":{"row":206,"column":12},"action":"remove","lines":["\\"],"id":59}],[{"start":{"row":206,"column":10},"end":{"row":206,"column":11},"action":"remove","lines":[" "],"id":60}],[{"start":{"row":206,"column":9},"end":{"row":206,"column":10},"action":"remove","lines":[" "],"id":61}],[{"start":{"row":206,"column":8},"end":{"row":206,"column":9},"action":"remove","lines":[" "],"id":62}],[{"start":{"row":206,"column":8},"end":{"row":206,"column":9},"action":"insert","lines":["}"],"id":63}],[{"start":{"row":202,"column":25},"end":{"row":203,"column":0},"action":"insert","lines":["",""],"id":64},{"start":{"row":203,"column":0},"end":{"row":203,"column":4},"action":"insert","lines":["    "]}],[{"start":{"row":208,"column":4},"end":{"row":209,"column":0},"action":"insert","lines":["",""],"id":65},{"start":{"row":209,"column":0},"end":{"row":209,"column":4},"action":"insert","lines":["    "]}],[{"start":{"row":209,"column":4},"end":{"row":209,"column":8},"action":"insert","lines":["    "],"id":66}],[{"start":{"row":208,"column":0},"end":{"row":208,"column":4},"action":"remove","lines":["    "],"id":67}],[{"start":{"row":207,"column":9},"end":{"row":208,"column":0},"action":"remove","lines":["",""],"id":68}],[{"start":{"row":209,"column":2},"end":{"row":209,"column":4},"action":"insert","lines":["  "],"id":69}],[{"start":{"row":201,"column":22},"end":{"row":201,"column":29},"action":"remove","lines":["hw2.dat"],"id":70},{"start":{"row":201,"column":22},"end":{"row":201,"column":30},"action":"insert","lines":["HW4a.txt"]}]]},"ace":{"folds":[],"scrolltop":1646.5,"scrollleft":0,"selection":{"start":{"row":164,"column":2},"end":{"row":164,"column":2},"isBackwards":false},"options":{"guessTabSize":true,"useWrapMode":false,"wrapToView":true},"firstLineState":{"row":299,"mode":"ace/mode/c_cpp"}},"timestamp":1475673376189,"hash":"1a870262539da9538b35a8fd6334d62b9b4d56a0"}