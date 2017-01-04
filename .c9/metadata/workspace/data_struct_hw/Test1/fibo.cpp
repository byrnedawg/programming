{"changed":true,"filter":false,"title":"fibo.cpp","tooltip":"/data_struct_hw/Test1/fibo.cpp","value":"// Name: Gregory Byrne\n\n#include <iostream>\n\nusing namespace std;\n\n\nint fibo(int n)\n{\n    int a = 0; \n    int b = 1; \n    int c = 0;\n    for(int i = 0; i < n; i++)\n    {\n        c = a + b;\n        a = b;\n        b = c;\n    }\n    return c;\n    \n}\n\nint recursiveFibo(int n)\n{\n    if( n < 2)\n    {\n        return 1;\n    }\n    \n    return recursiveFibo(n-1) + recursiveFibo(n-2);\n    \n}\n\nint main()\n{\n    cout << \"Brute Fibo = \" << fibo(6) << \"\\n\";\n    cout << \"Brute Fibo = \" << fibo(5) << \"\\n\";\n    \n    cout << \"Recursive Fibo = \" << recursiveFibo(6) << \"\\n\";\n    cout << \"Recursive Fibo = \" << recursiveFibo(5) << \"\\n\";\n    \n \n    \n    return 0;\n}","undoManager":{"mark":98,"position":100,"stack":[[{"start":{"row":16,"column":8},"end":{"row":16,"column":9},"action":"insert","lines":["r"],"id":175}],[{"start":{"row":16,"column":9},"end":{"row":16,"column":10},"action":"insert","lines":["n"],"id":176},{"start":{"row":16,"column":10},"end":{"row":16,"column":11},"action":"insert","lines":["b"]}],[{"start":{"row":16,"column":10},"end":{"row":16,"column":11},"action":"remove","lines":["b"],"id":177}],[{"start":{"row":16,"column":9},"end":{"row":16,"column":10},"action":"remove","lines":["n"],"id":178}],[{"start":{"row":16,"column":8},"end":{"row":16,"column":9},"action":"remove","lines":["r"],"id":179}],[{"start":{"row":16,"column":8},"end":{"row":16,"column":9},"action":"insert","lines":["r"],"id":180}],[{"start":{"row":16,"column":9},"end":{"row":16,"column":10},"action":"insert","lines":["n"],"id":181}],[{"start":{"row":16,"column":10},"end":{"row":16,"column":11},"action":"insert","lines":[" "],"id":182}],[{"start":{"row":16,"column":11},"end":{"row":16,"column":12},"action":"insert","lines":["c"],"id":183}],[{"start":{"row":16,"column":12},"end":{"row":16,"column":13},"action":"insert","lines":[";"],"id":184}],[{"start":{"row":41,"column":1},"end":{"row":45,"column":43},"action":"remove","lines":["   cout << \"Final GCD = \" << gcd(6,12) << \"\\n\";","    cout << \"Final GCD = \" << gcd(12,18) << \"\\n\";","    ","    cout << \"LCM = \" << lcm(6,12) << \"\\n\";","    cout << \"LCM = \" << lcm(12,18) << \"\\n\";"],"id":185}],[{"start":{"row":35,"column":13},"end":{"row":35,"column":34},"action":"remove","lines":["Trial by division GCD"],"id":186},{"start":{"row":35,"column":13},"end":{"row":35,"column":14},"action":"insert","lines":["B"]}],[{"start":{"row":35,"column":14},"end":{"row":35,"column":15},"action":"insert","lines":["r"],"id":187}],[{"start":{"row":35,"column":15},"end":{"row":35,"column":16},"action":"insert","lines":["u"],"id":188}],[{"start":{"row":35,"column":16},"end":{"row":35,"column":17},"action":"insert","lines":["t"],"id":189}],[{"start":{"row":35,"column":17},"end":{"row":35,"column":18},"action":"insert","lines":["e"],"id":190}],[{"start":{"row":35,"column":18},"end":{"row":35,"column":19},"action":"insert","lines":[" "],"id":191}],[{"start":{"row":35,"column":19},"end":{"row":35,"column":20},"action":"insert","lines":["g"],"id":192}],[{"start":{"row":35,"column":19},"end":{"row":35,"column":20},"action":"remove","lines":["g"],"id":193}],[{"start":{"row":35,"column":19},"end":{"row":35,"column":20},"action":"insert","lines":["F"],"id":194}],[{"start":{"row":35,"column":20},"end":{"row":35,"column":21},"action":"insert","lines":["u"],"id":195}],[{"start":{"row":35,"column":20},"end":{"row":35,"column":21},"action":"remove","lines":["u"],"id":196}],[{"start":{"row":35,"column":20},"end":{"row":35,"column":21},"action":"insert","lines":["b"],"id":197}],[{"start":{"row":35,"column":21},"end":{"row":35,"column":22},"action":"insert","lines":["i"],"id":198}],[{"start":{"row":35,"column":22},"end":{"row":35,"column":23},"action":"insert","lines":["p"],"id":199}],[{"start":{"row":35,"column":22},"end":{"row":35,"column":23},"action":"remove","lines":["p"],"id":200}],[{"start":{"row":35,"column":21},"end":{"row":35,"column":22},"action":"remove","lines":["i"],"id":201}],[{"start":{"row":35,"column":20},"end":{"row":35,"column":21},"action":"remove","lines":["b"],"id":202}],[{"start":{"row":35,"column":20},"end":{"row":35,"column":21},"action":"insert","lines":["i"],"id":203}],[{"start":{"row":35,"column":21},"end":{"row":35,"column":22},"action":"insert","lines":["b"],"id":204}],[{"start":{"row":35,"column":22},"end":{"row":35,"column":23},"action":"insert","lines":["o"],"id":205}],[{"start":{"row":38,"column":23},"end":{"row":38,"column":26},"action":"remove","lines":["GCD"],"id":206},{"start":{"row":38,"column":23},"end":{"row":38,"column":24},"action":"insert","lines":["F"]}],[{"start":{"row":38,"column":24},"end":{"row":38,"column":25},"action":"insert","lines":["i"],"id":207}],[{"start":{"row":38,"column":25},"end":{"row":38,"column":26},"action":"insert","lines":["b"],"id":208}],[{"start":{"row":38,"column":26},"end":{"row":38,"column":27},"action":"insert","lines":["o"],"id":209}],[{"start":{"row":39,"column":23},"end":{"row":39,"column":26},"action":"remove","lines":["GCD"],"id":210},{"start":{"row":39,"column":23},"end":{"row":39,"column":27},"action":"insert","lines":["Fibo"]}],[{"start":{"row":36,"column":13},"end":{"row":36,"column":34},"action":"remove","lines":["Trial by division GCD"],"id":211},{"start":{"row":36,"column":13},"end":{"row":36,"column":23},"action":"insert","lines":["Brute Fibo"]}],[{"start":{"row":35,"column":31},"end":{"row":35,"column":54},"action":"remove","lines":["trialDicvisionGCD(6,12)"],"id":212}],[{"start":{"row":36,"column":31},"end":{"row":36,"column":55},"action":"remove","lines":["trialDicvisionGCD(12,18)"],"id":213}],[{"start":{"row":38,"column":35},"end":{"row":38,"column":53},"action":"remove","lines":["recursiveGCD(6,12)"],"id":214}],[{"start":{"row":39,"column":35},"end":{"row":39,"column":54},"action":"remove","lines":["recursiveGCD(12,18)"],"id":215}],[{"start":{"row":35,"column":31},"end":{"row":35,"column":32},"action":"insert","lines":["f"],"id":216}],[{"start":{"row":35,"column":32},"end":{"row":35,"column":33},"action":"insert","lines":["i"],"id":217}],[{"start":{"row":35,"column":33},"end":{"row":35,"column":34},"action":"insert","lines":["b"],"id":218}],[{"start":{"row":35,"column":31},"end":{"row":35,"column":34},"action":"remove","lines":["fib"],"id":219},{"start":{"row":35,"column":31},"end":{"row":35,"column":37},"action":"insert","lines":["fibo()"]}],[{"start":{"row":35,"column":36},"end":{"row":35,"column":37},"action":"insert","lines":["1"],"id":220}],[{"start":{"row":35,"column":36},"end":{"row":35,"column":37},"action":"remove","lines":["1"],"id":221}],[{"start":{"row":35,"column":36},"end":{"row":35,"column":37},"action":"insert","lines":["2"],"id":222}],[{"start":{"row":35,"column":37},"end":{"row":35,"column":38},"action":"insert","lines":["0"],"id":223}],[{"start":{"row":36,"column":31},"end":{"row":36,"column":39},"action":"insert","lines":["fibo(20)"],"id":224}],[{"start":{"row":36,"column":36},"end":{"row":36,"column":38},"action":"remove","lines":["20"],"id":225},{"start":{"row":36,"column":36},"end":{"row":36,"column":37},"action":"insert","lines":["1"]}],[{"start":{"row":36,"column":37},"end":{"row":36,"column":38},"action":"insert","lines":["5"],"id":226}],[{"start":{"row":38,"column":35},"end":{"row":38,"column":36},"action":"insert","lines":["r"],"id":227}],[{"start":{"row":38,"column":36},"end":{"row":38,"column":37},"action":"insert","lines":["e"],"id":228}],[{"start":{"row":38,"column":35},"end":{"row":38,"column":37},"action":"remove","lines":["re"],"id":229},{"start":{"row":38,"column":35},"end":{"row":38,"column":50},"action":"insert","lines":["recursiveFibo()"]}],[{"start":{"row":38,"column":49},"end":{"row":38,"column":50},"action":"insert","lines":["2"],"id":230}],[{"start":{"row":38,"column":50},"end":{"row":38,"column":51},"action":"insert","lines":["0"],"id":231}],[{"start":{"row":39,"column":35},"end":{"row":39,"column":36},"action":"insert","lines":["r"],"id":232}],[{"start":{"row":39,"column":36},"end":{"row":39,"column":37},"action":"insert","lines":["e"],"id":233}],[{"start":{"row":39,"column":35},"end":{"row":39,"column":37},"action":"remove","lines":["re"],"id":234},{"start":{"row":39,"column":35},"end":{"row":39,"column":48},"action":"insert","lines":["recursiveFibo"]}],[{"start":{"row":39,"column":48},"end":{"row":39,"column":50},"action":"insert","lines":["()"],"id":235}],[{"start":{"row":39,"column":49},"end":{"row":39,"column":50},"action":"insert","lines":["1"],"id":236}],[{"start":{"row":39,"column":50},"end":{"row":39,"column":51},"action":"insert","lines":["5"],"id":237}],[{"start":{"row":9,"column":15},"end":{"row":10,"column":0},"action":"insert","lines":["",""],"id":238},{"start":{"row":10,"column":0},"end":{"row":10,"column":4},"action":"insert","lines":["    "]}],[{"start":{"row":10,"column":4},"end":{"row":10,"column":5},"action":"insert","lines":["i"],"id":239}],[{"start":{"row":10,"column":5},"end":{"row":10,"column":6},"action":"insert","lines":["n"],"id":240}],[{"start":{"row":10,"column":6},"end":{"row":10,"column":7},"action":"insert","lines":["t"],"id":241}],[{"start":{"row":10,"column":7},"end":{"row":10,"column":8},"action":"insert","lines":[" "],"id":242}],[{"start":{"row":10,"column":15},"end":{"row":11,"column":0},"action":"insert","lines":["",""],"id":243},{"start":{"row":11,"column":0},"end":{"row":11,"column":4},"action":"insert","lines":["    "]}],[{"start":{"row":11,"column":4},"end":{"row":11,"column":5},"action":"insert","lines":["i"],"id":244}],[{"start":{"row":11,"column":5},"end":{"row":11,"column":6},"action":"insert","lines":["n"],"id":245}],[{"start":{"row":11,"column":6},"end":{"row":11,"column":7},"action":"insert","lines":["t"],"id":246}],[{"start":{"row":11,"column":7},"end":{"row":11,"column":8},"action":"insert","lines":[" "],"id":247}],[{"start":{"row":11,"column":9},"end":{"row":11,"column":10},"action":"insert","lines":[" "],"id":248}],[{"start":{"row":11,"column":10},"end":{"row":11,"column":11},"action":"insert","lines":["="],"id":249}],[{"start":{"row":11,"column":11},"end":{"row":11,"column":12},"action":"insert","lines":[" "],"id":250}],[{"start":{"row":11,"column":11},"end":{"row":11,"column":12},"action":"remove","lines":[" "],"id":251}],[{"start":{"row":11,"column":11},"end":{"row":11,"column":12},"action":"insert","lines":[" "],"id":252}],[{"start":{"row":11,"column":12},"end":{"row":11,"column":13},"action":"insert","lines":["o"],"id":253}],[{"start":{"row":11,"column":12},"end":{"row":11,"column":13},"action":"remove","lines":["o"],"id":254}],[{"start":{"row":11,"column":12},"end":{"row":11,"column":13},"action":"insert","lines":["0"],"id":255}],[{"start":{"row":12,"column":16},"end":{"row":12,"column":17},"action":"remove","lines":["1"],"id":256},{"start":{"row":12,"column":16},"end":{"row":12,"column":17},"action":"insert","lines":["0"]}],[{"start":{"row":12,"column":22},"end":{"row":12,"column":23},"action":"remove","lines":["="],"id":257}],[{"start":{"row":37,"column":36},"end":{"row":37,"column":38},"action":"remove","lines":["20"],"id":258},{"start":{"row":37,"column":36},"end":{"row":37,"column":37},"action":"insert","lines":["6"]}],[{"start":{"row":38,"column":36},"end":{"row":38,"column":38},"action":"remove","lines":["15"],"id":259},{"start":{"row":38,"column":36},"end":{"row":38,"column":37},"action":"insert","lines":["6"]}],[{"start":{"row":38,"column":36},"end":{"row":38,"column":37},"action":"remove","lines":["6"],"id":260}],[{"start":{"row":38,"column":36},"end":{"row":38,"column":37},"action":"insert","lines":["5"],"id":261}],[{"start":{"row":40,"column":49},"end":{"row":40,"column":51},"action":"remove","lines":["20"],"id":262},{"start":{"row":40,"column":49},"end":{"row":40,"column":50},"action":"insert","lines":["6"]}],[{"start":{"row":41,"column":49},"end":{"row":41,"column":50},"action":"remove","lines":["1"],"id":263}],[{"start":{"row":24,"column":11},"end":{"row":24,"column":12},"action":"remove","lines":["="],"id":264}],[{"start":{"row":24,"column":11},"end":{"row":24,"column":12},"action":"insert","lines":["="],"id":265}],[{"start":{"row":12,"column":16},"end":{"row":12,"column":17},"action":"remove","lines":["0"],"id":266},{"start":{"row":12,"column":16},"end":{"row":12,"column":17},"action":"insert","lines":["1"]}],[{"start":{"row":9,"column":12},"end":{"row":9,"column":13},"action":"remove","lines":["1"],"id":267},{"start":{"row":9,"column":12},"end":{"row":9,"column":13},"action":"insert","lines":["0"]}],[{"start":{"row":12,"column":16},"end":{"row":12,"column":17},"action":"remove","lines":["1"],"id":268}],[{"start":{"row":12,"column":16},"end":{"row":12,"column":17},"action":"insert","lines":["0"],"id":269}],[{"start":{"row":12,"column":16},"end":{"row":12,"column":17},"action":"remove","lines":["0"],"id":270},{"start":{"row":12,"column":16},"end":{"row":12,"column":17},"action":"insert","lines":["1"]}],[{"start":{"row":12,"column":16},"end":{"row":12,"column":17},"action":"remove","lines":["1"],"id":271}],[{"start":{"row":12,"column":16},"end":{"row":12,"column":17},"action":"insert","lines":["0"],"id":272}],[{"start":{"row":24,"column":11},"end":{"row":24,"column":12},"action":"remove","lines":["="],"id":273}],[{"start":{"row":33,"column":0},"end":{"row":34,"column":0},"action":"remove","lines":["",""],"id":274}],[{"start":{"row":32,"column":0},"end":{"row":33,"column":0},"action":"remove","lines":["",""],"id":275}]]},"ace":{"folds":[],"scrolltop":0,"scrollleft":0,"selection":{"start":{"row":32,"column":0},"end":{"row":32,"column":0},"isBackwards":false},"options":{"guessTabSize":true,"useWrapMode":false,"wrapToView":true},"firstLineState":0},"timestamp":1474399050188}