{"filter":false,"title":"fermat.cpp","tooltip":"/data_struct_hw/hw2/fermat.cpp","undoManager":{"mark":42,"position":42,"stack":[[{"start":{"row":0,"column":0},"end":{"row":23,"column":1},"action":"insert","lines":["int powerMod(unsigned long long x, int n, int m){","    ","    int prod = 1;","    while(n > 0){","        if(n % 2 != 0){","            prod = (prod * x) % m;","        }","        n = n / 2;","        x = (x * x) % m;","    }","    return prod;","}","","bool fermat(long p, int k){","    for(int i = 1; i <= k; i++){","        int a = rand() % (p - 1) + 2; // 2 - p-1","        ","        if((powerMod(a, p-1, p)) != 1){","            return false;","        }","    }","    return true;","    ","}"],"id":1}],[{"start":{"row":0,"column":0},"end":{"row":1,"column":0},"action":"insert","lines":["",""],"id":2}],[{"start":{"row":1,"column":0},"end":{"row":2,"column":0},"action":"insert","lines":["",""],"id":3}],[{"start":{"row":0,"column":0},"end":{"row":10,"column":0},"action":"insert","lines":["// Name: Gregory Byrne","// Assignment: HW2 - Miller Rabin","#include <iostream>","#include <fstream>","#include <string>","#include <random>","","using namespace std;","","",""],"id":4}],[{"start":{"row":0,"column":0},"end":{"row":1,"column":33},"action":"remove","lines":["// Name: Gregory Byrne","// Assignment: HW2 - Miller Rabin"],"id":5},{"start":{"row":0,"column":0},"end":{"row":1,"column":33},"action":"insert","lines":["// Name: Gregory Byrne","// Assignment: HW2 - Miller Rabin"]}],[{"start":{"row":1,"column":21},"end":{"row":1,"column":33},"action":"remove","lines":["Miller Rabin"],"id":6},{"start":{"row":1,"column":21},"end":{"row":1,"column":22},"action":"insert","lines":["F"]}],[{"start":{"row":1,"column":22},"end":{"row":1,"column":23},"action":"insert","lines":["e"],"id":7}],[{"start":{"row":1,"column":23},"end":{"row":1,"column":24},"action":"insert","lines":["r"],"id":8}],[{"start":{"row":1,"column":24},"end":{"row":1,"column":25},"action":"insert","lines":["m"],"id":9}],[{"start":{"row":1,"column":25},"end":{"row":1,"column":26},"action":"insert","lines":["a"],"id":10}],[{"start":{"row":1,"column":26},"end":{"row":1,"column":27},"action":"insert","lines":["t"],"id":11}],[{"start":{"row":35,"column":1},"end":{"row":36,"column":0},"action":"insert","lines":["",""],"id":12}],[{"start":{"row":36,"column":0},"end":{"row":37,"column":0},"action":"insert","lines":["",""],"id":13}],[{"start":{"row":37,"column":0},"end":{"row":62,"column":1},"action":"insert","lines":["int main () {","  unsigned long long testNumber;","  int kFactor = 20;","  ifstream myfile (\"hw2.dat\");","  if (myfile.is_open())","  {","    while  (myfile >> testNumber)","    {","      cout <<  testNumber << \"\\t\";","      if(millerRabin(testNumber))","      //if(fermat(testNumber, kFactor))","      {","        cout << \"True\\n\";","      }","      else","      {","        cout << \"False\\n\";","      }","    }","    myfile.close();","  }","","  else cout << \"Unable to open file\"; ","","  return 0;","}"],"id":14}],[{"start":{"row":47,"column":7},"end":{"row":47,"column":8},"action":"remove","lines":["/"],"id":15}],[{"start":{"row":47,"column":6},"end":{"row":47,"column":7},"action":"remove","lines":["/"],"id":16}],[{"start":{"row":46,"column":6},"end":{"row":46,"column":33},"action":"remove","lines":["if(millerRabin(testNumber))"],"id":17}],[{"start":{"row":46,"column":5},"end":{"row":46,"column":6},"action":"remove","lines":[" "],"id":18}],[{"start":{"row":46,"column":4},"end":{"row":46,"column":5},"action":"remove","lines":[" "],"id":19}],[{"start":{"row":46,"column":0},"end":{"row":46,"column":4},"action":"remove","lines":["    "],"id":20}],[{"start":{"row":45,"column":34},"end":{"row":46,"column":0},"action":"remove","lines":["",""],"id":21}],[{"start":{"row":10,"column":0},"end":{"row":11,"column":0},"action":"remove","lines":["",""],"id":22}],[{"start":{"row":9,"column":0},"end":{"row":10,"column":0},"action":"remove","lines":["",""],"id":23}],[{"start":{"row":10,"column":49},"end":{"row":10,"column":50},"action":"insert","lines":[" "],"id":24}],[{"start":{"row":10,"column":50},"end":{"row":10,"column":51},"action":"insert","lines":[" "],"id":25}],[{"start":{"row":10,"column":51},"end":{"row":10,"column":52},"action":"insert","lines":["/"],"id":26}],[{"start":{"row":10,"column":52},"end":{"row":10,"column":53},"action":"insert","lines":["/"],"id":27}],[{"start":{"row":10,"column":53},"end":{"row":10,"column":54},"action":"insert","lines":[" "],"id":28}],[{"start":{"row":10,"column":54},"end":{"row":10,"column":55},"action":"insert","lines":[")"],"id":29}],[{"start":{"row":10,"column":55},"end":{"row":10,"column":57},"action":"insert","lines":["()"],"id":30}],[{"start":{"row":10,"column":55},"end":{"row":10,"column":57},"action":"remove","lines":["()"],"id":31}],[{"start":{"row":10,"column":54},"end":{"row":10,"column":55},"action":"remove","lines":[")"],"id":32}],[{"start":{"row":10,"column":54},"end":{"row":10,"column":55},"action":"insert","lines":["0"],"id":33}],[{"start":{"row":10,"column":54},"end":{"row":10,"column":55},"action":"remove","lines":["0"],"id":34}],[{"start":{"row":10,"column":54},"end":{"row":10,"column":55},"action":"insert","lines":["O"],"id":35}],[{"start":{"row":10,"column":55},"end":{"row":10,"column":57},"action":"insert","lines":["()"],"id":36}],[{"start":{"row":10,"column":56},"end":{"row":10,"column":57},"action":"insert","lines":["l"],"id":37}],[{"start":{"row":10,"column":57},"end":{"row":10,"column":58},"action":"insert","lines":["o"],"id":38}],[{"start":{"row":10,"column":58},"end":{"row":10,"column":59},"action":"insert","lines":["g"],"id":39}],[{"start":{"row":10,"column":59},"end":{"row":10,"column":60},"action":"insert","lines":["n"],"id":40}],[{"start":{"row":10,"column":59},"end":{"row":10,"column":60},"action":"remove","lines":["n"],"id":41}],[{"start":{"row":10,"column":59},"end":{"row":10,"column":60},"action":"insert","lines":[" "],"id":42}],[{"start":{"row":10,"column":60},"end":{"row":10,"column":61},"action":"insert","lines":["n"],"id":43}]]},"ace":{"folds":[],"scrolltop":360,"scrollleft":0,"selection":{"start":{"row":58,"column":11},"end":{"row":58,"column":11},"isBackwards":false},"options":{"guessTabSize":true,"useWrapMode":false,"wrapToView":true},"firstLineState":0},"timestamp":1473877212625,"hash":"ca8405d8b8484b98ece96c165ae2fb4b91612022"}