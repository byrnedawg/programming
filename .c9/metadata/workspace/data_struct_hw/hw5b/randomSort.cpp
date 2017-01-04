{"changed":true,"filter":false,"title":"randomSort.cpp","tooltip":"/data_struct_hw/hw5b/randomSort.cpp","value":"#include <iostream>\n#include <fstream>\n#include <string>\n\nusing namespace std;\n\t\t\t\t\t\t\t\t\t \nclass LinkedList2 {\nprivate:\n\tclass Node {\n\tpublic:\n    int val;\n\t\tNode* next;\n\t\tNode(int v, Node* n) : val(v), next(n) {}\n\t};\n\tNode* head;\n\tNode* tail;\n\tint count;\npublic:\n\tLinkedList2() : head(nullptr), tail(nullptr) \n\t{\n\t\tcount = 0;\n\t}\n\t\n\t~LinkedList2()\n\t{\n\t\tNode* q;\n\t\tfor (Node* p = head; p != nullptr; p = q) \n\t\t{\n\t\t\tq = p->next;\n\t\t\tdelete p;\n\t\t}\n\t}\n\n\tvoid addEnd(int v) \n\t{\n\t\tif (head == nullptr) \n\t\t{\n\t\t\thead = new Node(v, nullptr);\n\t\t\ttail = head;\n\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";\n\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";\n\t\t}\n\t\telse\n\t\t{\n\t\t\ttail -> next = new Node(v, nullptr);\n\t\t\ttail = tail->next;\n\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";\n\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";\n\t\t}\n\t\t\n\t\tcount++;\n\t}\n\t\n\tvoid addStart(int v) \n\t{ \n\t\t\n\t\tif(head == nullptr)\n\t\t{\n\t\t\thead = new Node(v, nullptr);\n\t\t\ttail = head;\n\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";\n\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";\n\t\t}\n\t\telse\n\t\t{\n\t\t\tNode *p = new Node(v, head);\n\t\t\tp -> next = head;\n\t\t\thead = p;\n\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";\n\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";\n\t\t}\n\t\tcount++;\n\t\t\n\t}\n\t\n\tvoid insert(int i, int v) \n\t{\n\t\t\n\t\tif(i > size())\n\t\t{\n\t\t\tthrow \"LinkedList index out of bounds\";\n\t\t}\n\t\telse if(i == size())\n\t\t{\n\t\t\taddEnd(v);\n\t\t}\n\t\telse if(i == 0)\n\t\t{\n\t\t\taddStart(v);\n\t\t}\n\t\telse\n\t\t{\n\t\t\tNode* p = head;\n\t    \twhile (i > 0) \n\t    \t{\n\t\t\t\tif (p == nullptr)\n\t\t\t\t{\n\t\t\t\t\treturn;\n\t\t\t\t}\n\t\t\t\tp = p->next;\n\t    \t\ti--;\n\t\t\t}\n\t    \tp->next = new Node(v, p->next);\n\t    \tcount++;\n\t\t}\n\t}\n\t\n\tvoid removeEnd() \n\t{\n\t\tif(head == nullptr)\n\t\t{\n\t\t\tcout << \"Empty List \\n\";\n\t\t\treturn;\n\t\t}\n\t\tif (head == tail)\n\t\t{\n\t\t\thead = nullptr;\n\t\t\ttail = nullptr;\n\t\t//\tcout <<\"\\n The Head is pointing to nothing\\n\";\n\t\t//\tcout << \"\\n The tail is pointing to nothing\\n\";\n\t\t\tcount--;\n\t\t}\n\t\telse if (head->next == tail) \n\t\t{\n\t\t\thead->next = nullptr;\n\t\t\ttail = head;\n\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";\n\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";\n\t\t\tcount--;\n\t\t\n\t\t}\n\t\telse\n\t\t{\n\t\t\tNode* p;\n\t\t\tfor (p = head; p->next->next != nullptr; p = p->next)\n\t\t\t{\n\t\t\t\t;\n\t\t\t}\n\t\t\t\tp->next = nullptr;\n\t\t\t\ttail = p;\n\t\t\t\tcount--;\n\t\t//\t\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";\n\t\t//\t\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";\n\t\t}\n\t\t\t\n\t}\n\t\n\tvoid removeStart() \n\t{\n\t\tNode *p = head;\n\t\tif(head == nullptr)\n\t\t{\n\t\t\tcout << \"Empty List \\n\";\n\t\t\treturn;\n\t\t}\n\t\t\n\t\tif (head == tail)\n\t\t{\n\t\t\thead = nullptr;\n\t\t\ttail = nullptr;\n\t\t//\tcout <<\"\\n The Head is pointing to nothing \\n\";;\n\t\t//\tcout << \"\\n The tail is pointing to nothing\\n\";\n\t\t\tcount--;\n\t\t}\n\t\telse if (head->next == tail) \n\t\t{\n\t\t\tdelete head;\n\t\t\thead = tail;\n\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";\n\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";\n\t\t\tcount--;\n\t\t}\n\t\telse\n\t\t{\n\t\t\thead = p-> next;\n\t\t\tdelete p;\n\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";\n\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";\n\t\t\tcount --;\n\t\t}\n\t}\n\n\tfriend ostream& operator<<(ostream& s, const LinkedList2& list) \n\t{\n\t    s << \"{ \";\n\t\tfor (Node* p = list.head; p != nullptr; p = p->next) \n\t\t{\n\t\t\ts << p->val << ' ';\n\t\t}\n\t\ts << \"}\";\n\t\treturn s;\n\t}\n \n\tint size() const \n\t{ \n\t\treturn count;\n\t}\n\n\tint get(int i) const \n\t{ \n\t\tif(i == size())\n\t\t{\n\t\t\treturn tail->val;\n\t\t}\n\t\telse if(i > size())\n\t\t{\n\t\t\treturn -1;\n\t\t\tthrow \"LinkedList index out of bounds\";\n\t\t}\n\t\telse\n\t\t{\n\t\t\tNode* p = head;\n\t\t\tfor ( ; i > 0; i--, p = p->next)\n\t\t\t\tif (p == nullptr)\n\t\t\t\t\tthrow \"LinkedList index out of bounds\";\n\t    \treturn p->val;\n\t\t}\n\t}\n\n\t\n};\n\n\nvoid firstTest()\n{\n\tLinkedList2 a;\n\tfor (int i = 0; i < 10; i++)\n\t\ta.addStart(i);\n\tcout << a << '\\n';\n\tfor (int i = 0; i < 10; i++)\n\t\ta.addEnd(i);\n\tcout << a << '\\n';\n\tcout << \"The Value at postion 3 is \" << a.get(3) << \" \\n\";\n\tcout << \"The Value at postion 10 is \" << a.get(10) << \" \\n\";\n\tcout << \" The size of the linked list is \" << a.size() << \"\\n\";\n\tcout << \"The Value at postion 20 is \" << a.get(20) << \" \\n\";\n\tcout << \"The Value at postion 21 is \" << a.get(21) << \" \\n\";\n\ta.insert(0,53);\n\tcout << a << '\\n';\n\tcout << \" The size of the linked list is \" << a.size() << \"\\n\";\n\ta.removeStart();\n\tcout << \" The size of the linked list is \" << a.size() << \"\\n\";\n\tcout << a << '\\n';\n\ta.removeEnd();\n\tcout << a << '\\n';\n\tcout << \" The size of the linked list is \" << a.size() << \"\\n\";\n\t\n}\n\nvoid openTestFile()\n{\n\tLinkedList2 testList;\n\tifstream myfile (\"HW4b.txt\");\n\t//\tifstream myfile (\"HW4c.txt\");\n\tstring::size_type sz;\n    string command;\n    string line;\n    string numbers = \"0123456789\";\n    string stringnum1, stringnum2, stringnum3, stringRemove;\n    int startNum, stepNum, stopNum;\n    string addFront = \"ADD_FRONT\";\n    string addBack = \"ADD_BACK\";\n    string output1 = \"OUTPUT\";\n    string output2 = \"OUTPUT\\r\";\n    string removeFront = \"REMOVE_FRONT\";\n    string removeBack = \"REMOVE_BACK\";\n    \n    if (myfile.is_open())\n\t{\n    \twhile  (getline(myfile, line))\n    \t{\n    \t\tcommand = line.substr(0, line.find_first_of(' '));\n    \t\tif(command.compare(\"\") != 0 && command.compare(\"\\r\")!= 0 )\n    \t\t{\n\t    \t\tif((command.compare(addFront) == 0))\n\t    \t\t{\n\t    \t\t\tstringnum1 = line.substr(line.find_first_of(numbers.c_str()), line.find_first_of(':') - line.find_first_of(numbers.c_str()));\n\t    \t\t\tstringnum2 = line.substr(line.find_first_of(':')+1,(line.find_last_of(':'))-line.find_first_of(':')-1);\n\t   \t\t\t\tstringnum3 = line.substr(line.find_last_of(':')+1);\n\t   \t\t\t\tstartNum = stoi(stringnum1, &sz);\n\t   \t\t\t\tstepNum = stoi(stringnum2, &sz);\n\t   \t\t\t\tstopNum = stoi(stringnum3, &sz);\n\t    \t\t//\tcout << \"The command for this line is ADD_FRONT\\n\";\n\t\t    \t//\tcout << \"StartNum = \" << startNum << \"\\n\";\n\t\t    \t//\tcout << \"StepNum = \" << stepNum << \"\\n\";\n\t\t   \t\t//\tcout << \"StopNum = \" << stopNum << \"\\n\";\n\t\t   \t\t\tfor(int i = startNum; i <= stopNum; i= i+stepNum)\n\t\t   \t\t\t{\n\t\t   \t\t\t\ttestList.addStart(i);\n\t\t   \t\t\t}\n\t   \t\t\t}\n\t   \t\t\telse if((command.compare(addBack) == 0))\n\t   \t\t\t{\n\t   \t\t\t\tstringnum1 = line.substr(line.find_first_of(numbers.c_str()), line.find_first_of(':') - line.find_first_of(numbers.c_str()));\n\t    \t\t\tstringnum2 = line.substr(line.find_first_of(':')+1,(line.find_last_of(':'))-line.find_first_of(':')-1);\n\t   \t\t\t\tstringnum3 = line.substr(line.find_last_of(':')+1);\n\t   \t\t\t\tstartNum = stoi(stringnum1, &sz);\n\t   \t\t\t\tstepNum = stoi(stringnum2, &sz);\n\t   \t\t\t\tstopNum = stoi(stringnum3, &sz);\n\t   \t\t\t//\tcout << \"The command for this line is ADD_BACK\\n\";\n\t   \t\t\t//\tcout << \"StartNum = \" << startNum << \"\\n\";\n\t\t    \t//\tcout << \"StepNum = \" << stepNum << \"\\n\";\n\t\t   \t\t//\tcout << \"StopNum = \" << stopNum << \"\\n\";\n\t\t   \t\t\tfor(int i = startNum; i <= stopNum; i = i+stepNum)\n\t\t   \t\t\t{\n\t\t   \t\t\t\ttestList.addEnd(i);\n\t\t   \t\t\t}\n\t    \t\t}\n\t    \t\telse if((command.compare(output1) == 0) || (command.compare(output2) == 0))\n\t    \t\t{\n\t    \t\t\t//cout << \"The command for this line is OUTPUT\\n\";\n\t    \t\t\tcout << testList << \"\\n\";\n\t\t\t\t\tcout << \"Size of List = \" << testList.size() << endl;\n\t\t\t\t//\tcout << \"Total Capacity = \" << testList.getCapacity() << \"\\n\";\n\t    \t\t}\n\t    \t\telse if((command.compare(removeFront) == 0))\n\t    \t\t{\n\t\t\t\t\tstringRemove = line.substr(line.find_first_of(numbers.c_str()));\n\t\t\t\t\tstartNum = stoi(stringRemove, &sz);\n\t    \t\t//\tcout << \"The command for this line is REMOVE_FRONT\\n\";\t\n\t    \t\t//\tcout << \"RemoveNum= \" << startNum << \"\\n\";\n\t    \t\t\t\n\t    \t\t\tfor(int i = 0; i < startNum; i++)\n\t    \t\t\t{\n\t    \t\t\t\ttestList.removeStart();\n\t    \t\t\t}\n\t    \t\t}\n\t    \t\telse if((command.compare(removeBack) == 0))\n\t    \t\t{\n\t    \t\t\tstringRemove = line.substr(line.find_first_of(numbers.c_str()));\n\t\t\t\t\tstartNum = stoi(stringRemove, &sz);\n\t    \t\t//\tcout << \"The command for this line is REMOVE_BACK\\n\";\t\n\t    \t\t//\tcout << \"RemoveNum= \" << startNum << \"\\n\";\n\t    \t\t\tfor(int i = 0; i < startNum; i++)\n\t    \t\t\t{\n\t    \t\t\t\ttestList.removeEnd();\n\t    \t\t\t}\n\t    \t\t}\n\t    \t\telse\n\t    \t\t{\n\t    \t\t\tcout << \"The Text File may be corrupt no understood commands\\n\\n\";\n\t    \t\t}\t\n\t\t    \t\n    \t\t}\n\t    \t\t\n    \t\n    \t}\n    \tmyfile.close();\n\t}\n\telse\n    {\n        cout << \"Unable to open file\";\n    }\n}\n\nvoid commandLineTest()\n{\n\tLinkedList2 commandLineList;\n\tint answer = 0;\n\tint value = 0;\n\tcout << \"Here is the current List: \\n\";\n\tcout << commandLineList << \"\\n\";\n//\tcout << \"Storage Used = \" << commandLineList.getUsed() << \"\\n\";\t\t\t\t\n//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";\n\tdo{\n\t\tcout << \"-----------------------------------------------------------\\n\";\n\t\tcout << \"What do you want to do to our List? \\n\";\n\t\tcout << \"1: Add value to the End \\n\";\n\t\tcout << \"2: Add value to the Front \\n\";\n\t\tcout << \"3: Remove value from the End \\n\";\n\t\tcout << \"4: Remove value from the Front \\n\";\n\t\tcout << \"5: Display List and Stats \\n\";\n\t\tcout << \"6: Finished End Program \\n\";\n\t\tcout << \"-----------------------------------------------------------\\n\";\n\t\tcin >> answer;\n\t\tif(answer == 1)\n\t\t{\n\t\t\tcout << \"What Value do you want to add to the End? \\n\";\n\t\t\tcin >> value;\n\t\t\tcommandLineList.addEnd(value);\n\t\t\tcout << \"Adding \" << value << \" to the end \\n\\n\";\n\t\t\tcout << commandLineList << \"\\n\";\n\t\t\tcout << \"Size of List = \" << commandLineList.size() << endl;\t\t\t\t\n\t\t//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";\n\t\t\t\n\t\t}\n\t\telse if(answer == 2)\n\t\t{\n\t\t\tcout << \"What Value do you want to add to the Front? \\n\";\n\t\t\tcin >> value;\n\t\t\tcommandLineList.addStart(value);\n\t\t\tcout << \"Adding \" << value << \" to the Front \\n\\n\";\n\t\t\tcout << commandLineList << \"\\n\";\n\t\t\tcout << \"Size of List = \" << commandLineList.size() << endl;\t\t\t\t\t\n\t\t//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";\n\t\t}\n\t\telse if(answer == 3)\n\t\t{\n\t\t\tcommandLineList.removeEnd();\n\t\t\tcout << \"Removing from the End \\n\\n\";\n\t\t\tcout << commandLineList << \"\\n\";\n\t\t\tcout << \"Size of List = \" << commandLineList.size() << endl;\t\t\t\t\t\n\t\t//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";\n\t\t}\n\t\telse if(answer == 4)\n\t\t{\n\t\t\tcommandLineList.removeStart();\n\t\t\tcout << \"Removing from the Front \\n\\n\";\n\t\t\tcout << commandLineList << \"\\n\";\n\t\t\tcout << \"Size of List = \" << commandLineList.size() << endl;\t\t\t\t\t\n\t\t//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";\n\t\t}\n\t\telse\n\t\t{\n\t\t\tcout << \"-----------------------------------------------------------\\n\";\n\t\t\tcout << commandLineList << \"\\n\";\n\t\t\tcout << \"Size of List = \" << commandLineList.size() << endl;\t\t\t\t\t\n\t\t//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";\n\t\t\tcout << \"-----------------------------------------------------------\\n\";\n\t\t}\n\t}while(answer != 6);\n\t\n}\n\n\nint main() \n{\n\topenTestFile();\n//\tcommandLineTest();\n//\tfirstTest();\n\n  return 0;\n}\n\n\t\n\n\n","undoManager":{"mark":-2,"position":0,"stack":[[{"start":{"row":0,"column":0},"end":{"row":437,"column":0},"action":"insert","lines":["#include <iostream>","#include <fstream>","#include <string>","","using namespace std;","\t\t\t\t\t\t\t\t\t ","class LinkedList2 {","private:","\tclass Node {","\tpublic:","    int val;","\t\tNode* next;","\t\tNode(int v, Node* n) : val(v), next(n) {}","\t};","\tNode* head;","\tNode* tail;","\tint count;","public:","\tLinkedList2() : head(nullptr), tail(nullptr) ","\t{","\t\tcount = 0;","\t}","\t","\t~LinkedList2()","\t{","\t\tNode* q;","\t\tfor (Node* p = head; p != nullptr; p = q) ","\t\t{","\t\t\tq = p->next;","\t\t\tdelete p;","\t\t}","\t}","","\tvoid addEnd(int v) ","\t{","\t\tif (head == nullptr) ","\t\t{","\t\t\thead = new Node(v, nullptr);","\t\t\ttail = head;","\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";","\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";","\t\t}","\t\telse","\t\t{","\t\t\ttail -> next = new Node(v, nullptr);","\t\t\ttail = tail->next;","\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";","\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";","\t\t}","\t\t","\t\tcount++;","\t}","\t","\tvoid addStart(int v) ","\t{ ","\t\t","\t\tif(head == nullptr)","\t\t{","\t\t\thead = new Node(v, nullptr);","\t\t\ttail = head;","\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";","\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";","\t\t}","\t\telse","\t\t{","\t\t\tNode *p = new Node(v, head);","\t\t\tp -> next = head;","\t\t\thead = p;","\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";","\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";","\t\t}","\t\tcount++;","\t\t","\t}","\t","\tvoid insert(int i, int v) ","\t{","\t\t","\t\tif(i > size())","\t\t{","\t\t\tthrow \"LinkedList index out of bounds\";","\t\t}","\t\telse if(i == size())","\t\t{","\t\t\taddEnd(v);","\t\t}","\t\telse if(i == 0)","\t\t{","\t\t\taddStart(v);","\t\t}","\t\telse","\t\t{","\t\t\tNode* p = head;","\t    \twhile (i > 0) ","\t    \t{","\t\t\t\tif (p == nullptr)","\t\t\t\t{","\t\t\t\t\treturn;","\t\t\t\t}","\t\t\t\tp = p->next;","\t    \t\ti--;","\t\t\t}","\t    \tp->next = new Node(v, p->next);","\t    \tcount++;","\t\t}","\t}","\t","\tvoid removeEnd() ","\t{","\t\tif(head == nullptr)","\t\t{","\t\t\tcout << \"Empty List \\n\";","\t\t\treturn;","\t\t}","\t\tif (head == tail)","\t\t{","\t\t\thead = nullptr;","\t\t\ttail = nullptr;","\t\t//\tcout <<\"\\n The Head is pointing to nothing\\n\";","\t\t//\tcout << \"\\n The tail is pointing to nothing\\n\";","\t\t\tcount--;","\t\t}","\t\telse if (head->next == tail) ","\t\t{","\t\t\thead->next = nullptr;","\t\t\ttail = head;","\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";","\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";","\t\t\tcount--;","\t\t","\t\t}","\t\telse","\t\t{","\t\t\tNode* p;","\t\t\tfor (p = head; p->next->next != nullptr; p = p->next)","\t\t\t{","\t\t\t\t;","\t\t\t}","\t\t\t\tp->next = nullptr;","\t\t\t\ttail = p;","\t\t\t\tcount--;","\t\t//\t\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";","\t\t//\t\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";","\t\t}","\t\t\t","\t}","\t","\tvoid removeStart() ","\t{","\t\tNode *p = head;","\t\tif(head == nullptr)","\t\t{","\t\t\tcout << \"Empty List \\n\";","\t\t\treturn;","\t\t}","\t\t","\t\tif (head == tail)","\t\t{","\t\t\thead = nullptr;","\t\t\ttail = nullptr;","\t\t//\tcout <<\"\\n The Head is pointing to nothing \\n\";;","\t\t//\tcout << \"\\n The tail is pointing to nothing\\n\";","\t\t\tcount--;","\t\t}","\t\telse if (head->next == tail) ","\t\t{","\t\t\tdelete head;","\t\t\thead = tail;","\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";","\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";","\t\t\tcount--;","\t\t}","\t\telse","\t\t{","\t\t\thead = p-> next;","\t\t\tdelete p;","\t\t//\tcout <<\"\\n The Head is pointing to \" << head -> val << \"\\n\";","\t\t//\tcout << \"\\n The tail is pointing to \" << tail -> val << \"\\n\";","\t\t\tcount --;","\t\t}","\t}","","\tfriend ostream& operator<<(ostream& s, const LinkedList2& list) ","\t{","\t    s << \"{ \";","\t\tfor (Node* p = list.head; p != nullptr; p = p->next) ","\t\t{","\t\t\ts << p->val << ' ';","\t\t}","\t\ts << \"}\";","\t\treturn s;","\t}"," ","\tint size() const ","\t{ ","\t\treturn count;","\t}","","\tint get(int i) const ","\t{ ","\t\tif(i == size())","\t\t{","\t\t\treturn tail->val;","\t\t}","\t\telse if(i > size())","\t\t{","\t\t\treturn -1;","\t\t\tthrow \"LinkedList index out of bounds\";","\t\t}","\t\telse","\t\t{","\t\t\tNode* p = head;","\t\t\tfor ( ; i > 0; i--, p = p->next)","\t\t\t\tif (p == nullptr)","\t\t\t\t\tthrow \"LinkedList index out of bounds\";","\t    \treturn p->val;","\t\t}","\t}","","\t","};","","","void firstTest()","{","\tLinkedList2 a;","\tfor (int i = 0; i < 10; i++)","\t\ta.addStart(i);","\tcout << a << '\\n';","\tfor (int i = 0; i < 10; i++)","\t\ta.addEnd(i);","\tcout << a << '\\n';","\tcout << \"The Value at postion 3 is \" << a.get(3) << \" \\n\";","\tcout << \"The Value at postion 10 is \" << a.get(10) << \" \\n\";","\tcout << \" The size of the linked list is \" << a.size() << \"\\n\";","\tcout << \"The Value at postion 20 is \" << a.get(20) << \" \\n\";","\tcout << \"The Value at postion 21 is \" << a.get(21) << \" \\n\";","\ta.insert(0,53);","\tcout << a << '\\n';","\tcout << \" The size of the linked list is \" << a.size() << \"\\n\";","\ta.removeStart();","\tcout << \" The size of the linked list is \" << a.size() << \"\\n\";","\tcout << a << '\\n';","\ta.removeEnd();","\tcout << a << '\\n';","\tcout << \" The size of the linked list is \" << a.size() << \"\\n\";","\t","}","","void openTestFile()","{","\tLinkedList2 testList;","\tifstream myfile (\"HW4b.txt\");","\t//\tifstream myfile (\"HW4c.txt\");","\tstring::size_type sz;","    string command;","    string line;","    string numbers = \"0123456789\";","    string stringnum1, stringnum2, stringnum3, stringRemove;","    int startNum, stepNum, stopNum;","    string addFront = \"ADD_FRONT\";","    string addBack = \"ADD_BACK\";","    string output1 = \"OUTPUT\";","    string output2 = \"OUTPUT\\r\";","    string removeFront = \"REMOVE_FRONT\";","    string removeBack = \"REMOVE_BACK\";","    ","    if (myfile.is_open())","\t{","    \twhile  (getline(myfile, line))","    \t{","    \t\tcommand = line.substr(0, line.find_first_of(' '));","    \t\tif(command.compare(\"\") != 0 && command.compare(\"\\r\")!= 0 )","    \t\t{","\t    \t\tif((command.compare(addFront) == 0))","\t    \t\t{","\t    \t\t\tstringnum1 = line.substr(line.find_first_of(numbers.c_str()), line.find_first_of(':') - line.find_first_of(numbers.c_str()));","\t    \t\t\tstringnum2 = line.substr(line.find_first_of(':')+1,(line.find_last_of(':'))-line.find_first_of(':')-1);","\t   \t\t\t\tstringnum3 = line.substr(line.find_last_of(':')+1);","\t   \t\t\t\tstartNum = stoi(stringnum1, &sz);","\t   \t\t\t\tstepNum = stoi(stringnum2, &sz);","\t   \t\t\t\tstopNum = stoi(stringnum3, &sz);","\t    \t\t//\tcout << \"The command for this line is ADD_FRONT\\n\";","\t\t    \t//\tcout << \"StartNum = \" << startNum << \"\\n\";","\t\t    \t//\tcout << \"StepNum = \" << stepNum << \"\\n\";","\t\t   \t\t//\tcout << \"StopNum = \" << stopNum << \"\\n\";","\t\t   \t\t\tfor(int i = startNum; i <= stopNum; i= i+stepNum)","\t\t   \t\t\t{","\t\t   \t\t\t\ttestList.addStart(i);","\t\t   \t\t\t}","\t   \t\t\t}","\t   \t\t\telse if((command.compare(addBack) == 0))","\t   \t\t\t{","\t   \t\t\t\tstringnum1 = line.substr(line.find_first_of(numbers.c_str()), line.find_first_of(':') - line.find_first_of(numbers.c_str()));","\t    \t\t\tstringnum2 = line.substr(line.find_first_of(':')+1,(line.find_last_of(':'))-line.find_first_of(':')-1);","\t   \t\t\t\tstringnum3 = line.substr(line.find_last_of(':')+1);","\t   \t\t\t\tstartNum = stoi(stringnum1, &sz);","\t   \t\t\t\tstepNum = stoi(stringnum2, &sz);","\t   \t\t\t\tstopNum = stoi(stringnum3, &sz);","\t   \t\t\t//\tcout << \"The command for this line is ADD_BACK\\n\";","\t   \t\t\t//\tcout << \"StartNum = \" << startNum << \"\\n\";","\t\t    \t//\tcout << \"StepNum = \" << stepNum << \"\\n\";","\t\t   \t\t//\tcout << \"StopNum = \" << stopNum << \"\\n\";","\t\t   \t\t\tfor(int i = startNum; i <= stopNum; i = i+stepNum)","\t\t   \t\t\t{","\t\t   \t\t\t\ttestList.addEnd(i);","\t\t   \t\t\t}","\t    \t\t}","\t    \t\telse if((command.compare(output1) == 0) || (command.compare(output2) == 0))","\t    \t\t{","\t    \t\t\t//cout << \"The command for this line is OUTPUT\\n\";","\t    \t\t\tcout << testList << \"\\n\";","\t\t\t\t\tcout << \"Size of List = \" << testList.size() << endl;","\t\t\t\t//\tcout << \"Total Capacity = \" << testList.getCapacity() << \"\\n\";","\t    \t\t}","\t    \t\telse if((command.compare(removeFront) == 0))","\t    \t\t{","\t\t\t\t\tstringRemove = line.substr(line.find_first_of(numbers.c_str()));","\t\t\t\t\tstartNum = stoi(stringRemove, &sz);","\t    \t\t//\tcout << \"The command for this line is REMOVE_FRONT\\n\";\t","\t    \t\t//\tcout << \"RemoveNum= \" << startNum << \"\\n\";","\t    \t\t\t","\t    \t\t\tfor(int i = 0; i < startNum; i++)","\t    \t\t\t{","\t    \t\t\t\ttestList.removeStart();","\t    \t\t\t}","\t    \t\t}","\t    \t\telse if((command.compare(removeBack) == 0))","\t    \t\t{","\t    \t\t\tstringRemove = line.substr(line.find_first_of(numbers.c_str()));","\t\t\t\t\tstartNum = stoi(stringRemove, &sz);","\t    \t\t//\tcout << \"The command for this line is REMOVE_BACK\\n\";\t","\t    \t\t//\tcout << \"RemoveNum= \" << startNum << \"\\n\";","\t    \t\t\tfor(int i = 0; i < startNum; i++)","\t    \t\t\t{","\t    \t\t\t\ttestList.removeEnd();","\t    \t\t\t}","\t    \t\t}","\t    \t\telse","\t    \t\t{","\t    \t\t\tcout << \"The Text File may be corrupt no understood commands\\n\\n\";","\t    \t\t}\t","\t\t    \t","    \t\t}","\t    \t\t","    \t","    \t}","    \tmyfile.close();","\t}","\telse","    {","        cout << \"Unable to open file\";","    }","}","","void commandLineTest()","{","\tLinkedList2 commandLineList;","\tint answer = 0;","\tint value = 0;","\tcout << \"Here is the current List: \\n\";","\tcout << commandLineList << \"\\n\";","//\tcout << \"Storage Used = \" << commandLineList.getUsed() << \"\\n\";\t\t\t\t","//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";","\tdo{","\t\tcout << \"-----------------------------------------------------------\\n\";","\t\tcout << \"What do you want to do to our List? \\n\";","\t\tcout << \"1: Add value to the End \\n\";","\t\tcout << \"2: Add value to the Front \\n\";","\t\tcout << \"3: Remove value from the End \\n\";","\t\tcout << \"4: Remove value from the Front \\n\";","\t\tcout << \"5: Display List and Stats \\n\";","\t\tcout << \"6: Finished End Program \\n\";","\t\tcout << \"-----------------------------------------------------------\\n\";","\t\tcin >> answer;","\t\tif(answer == 1)","\t\t{","\t\t\tcout << \"What Value do you want to add to the End? \\n\";","\t\t\tcin >> value;","\t\t\tcommandLineList.addEnd(value);","\t\t\tcout << \"Adding \" << value << \" to the end \\n\\n\";","\t\t\tcout << commandLineList << \"\\n\";","\t\t\tcout << \"Size of List = \" << commandLineList.size() << endl;\t\t\t\t","\t\t//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";","\t\t\t","\t\t}","\t\telse if(answer == 2)","\t\t{","\t\t\tcout << \"What Value do you want to add to the Front? \\n\";","\t\t\tcin >> value;","\t\t\tcommandLineList.addStart(value);","\t\t\tcout << \"Adding \" << value << \" to the Front \\n\\n\";","\t\t\tcout << commandLineList << \"\\n\";","\t\t\tcout << \"Size of List = \" << commandLineList.size() << endl;\t\t\t\t\t","\t\t//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";","\t\t}","\t\telse if(answer == 3)","\t\t{","\t\t\tcommandLineList.removeEnd();","\t\t\tcout << \"Removing from the End \\n\\n\";","\t\t\tcout << commandLineList << \"\\n\";","\t\t\tcout << \"Size of List = \" << commandLineList.size() << endl;\t\t\t\t\t","\t\t//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";","\t\t}","\t\telse if(answer == 4)","\t\t{","\t\t\tcommandLineList.removeStart();","\t\t\tcout << \"Removing from the Front \\n\\n\";","\t\t\tcout << commandLineList << \"\\n\";","\t\t\tcout << \"Size of List = \" << commandLineList.size() << endl;\t\t\t\t\t","\t\t//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";","\t\t}","\t\telse","\t\t{","\t\t\tcout << \"-----------------------------------------------------------\\n\";","\t\t\tcout << commandLineList << \"\\n\";","\t\t\tcout << \"Size of List = \" << commandLineList.size() << endl;\t\t\t\t\t","\t\t//\tcout << \"Total Capacity = \" << commandLineList.getCapacity() << \"\\n\";","\t\t\tcout << \"-----------------------------------------------------------\\n\";","\t\t}","\t}while(answer != 6);","\t","}","","","int main() ","{","\topenTestFile();","//\tcommandLineTest();","//\tfirstTest();","","  return 0;","}","","\t","","",""],"id":1}]]},"ace":{"folds":[],"scrolltop":0,"scrollleft":0,"selection":{"start":{"row":18,"column":34},"end":{"row":18,"column":34},"isBackwards":false},"options":{"guessTabSize":true,"useWrapMode":false,"wrapToView":true},"firstLineState":0},"timestamp":1476828593636}