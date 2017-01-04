//Eric Leschinski..
//boggle game implementation
#pragma hdrstop

#include"stack.h"
#include"boggle.h"
#include<iostream.h>
#include<fstream.h>
#include<string.h>
#include<iomanip>
#include<stdlib.h>
#include<ctype.h>

using namespace std;

boggle::boggle(){
     path = new stack<char>(1000);
}
//==============================================================================
void boggle::loadvertexs(char * first, char * second, char * third, char * fourth)
{
M = new vertex[17];           //a pointer to 16 structures of vertexes.
answercounter = 0;

int e = 0;
for(int c = 0; c < 4; c++){
     for(int d = 0; d < 4; d++){
          M[e].x = c;
          M[e].y = d;
          e++;
     }
}
for (int b = 0; b < 4; b++){            //this puts what the user typed into
     M[b].value = first[b];          //the array of vectors called M.
     M[b+4].value = second[b];       //M of 0 through 16 gets characters
     M[b+8].value = third[b];
     M[b+12].value = fourth[b];
}
int z = 0;
int array[129] = {
-2,-2,-2,-2,-2,1,5,4,   -2,-2,0,4,-2,2,6,5,
-2,-2,1,5,-2,3,7,6,     -2,-2,2,6,-2,-2,-2,7,
0,-2,-2,-2,1,5,9,8,     1,0,4,8,2,6,10,9,
2,1,5,9,3,7,11,10,      3,2,6,10,-2,-2,-2,11,
4,-2,-2,-2,5,9,13,12,   5,4,8,12,6,10,14,13,
6,5,9,13,7,11,15,14,    7,6,10,14,-2,-2,-2,15,
8,-2,-2,-2,9,13,-2,-2,  9,8,12,-2,10,14,-2,-2,
10,9,13,-2,11,15,-2,-2, 11,10,14,-2,-2,-2,-2,-2 };

answerarray = new char*[1000];
for (int a = 0; a < 1000; a++)
        answerarray[a] = new char[16];

for (int a = 0; a < 16; a++)
     for (int b = 0; b < 8; b++){
          M[a].neighbor[b] = array[z];
          z++;
     }
}
//==============================================================================
void boggle::confirminput(){
     cout<<"make sure you inputted the letters correctly: "<<endl;

for (int a = 0; a < 16; a++){
          cout<<M[a].value<<" ";
          if (((a+1) % 4) == 0 )
               cout<<endl;
}
}
//==============================================================================

int boggle::findwords(){

char n[40];
ifstream data;
int z = 0;

data.open("englishrevise.txt", ios::in);     //split them up
if (!data)
data.open("wordsfile.txt", ios::in);     //split them up
if (!data)
data.open("wordsfile.txt.txt", ios::in);     //split them up
if (!data)
data.open("wordsfile", ios::in);     //split them up
if (!data)
        return 1;

while (!data.eof()){
     data>>n;
     z++;
}
sizeofenglish = z;

english = new listwords[sizeofenglish+2];
cout<<"There are: "<<z<<" things in this text file."<<endl;
data.close();
data.open("englishrevise.txt", ios::in);     
if (!data)
data.open("wordsfile.txt", ios::in);    
if (!data)
data.open("wordsfile.txt.txt", ios::in);    
if (!data)
data.open("wordsfile", ios::in);     
int x = 0;

while (!data.eof()){
     data >> english[x].chrword;
     x++;
}

for (int s = 0; s < 16; s++){
clearedges();

visit(s, -1);   //-1 signifies that we beamed down, we didnt arrive from
path->pop();                      //anywhere.
}

data.close();
return 0;
}
//=====================================================================
int boggle::visit(int place, int camefrom)
{
     path->push(M[place].value);      //arrive at a spot, put it on the stack.

     int result = binarysearch();    //see if this is a good word or not.

     if (result == 1){
          //cout<<path->getstringstack()<<"  ";
          strcpy(answerarray[answercounter], path->getstringstack());
          answercounter++;                                 //was a match

          int b = 7;
               for(int c = 0; c < 8; c++){
                    if (M[place].neighbor[c] != -2)
                         M[M[place].neighbor[c]].edge[b] = 0;
                    b--;
               }
     }
     else if (result == 0){
         // cout<<path->getstringstack()<<":   their may be a match later."<<endl;
          int b = 7;
               for(int c = 0; c < 8; c++){
                    if (M[place].neighbor[c] != -2)
                         M[M[place].neighbor[c]].edge[b] = 0;
                    b--;
               }
     }
     else if (result == -1){
          //cout<<path->getstringstack()<<":   no words start with this."<<endl;
          int b = 7;
               for(int c = 0; c < 8; c++){
                    if (M[place].neighbor[c] != -2)
                         M[M[place].neighbor[c]].edge[b] = 1;
                    b--;
               }
          //path->pop();
          return 0;
     }
     for (int i = 0; i < 8; i++){

          if (M[place].edge[i] == 1){

               visit(M[place].neighbor[i], i);
               path->pop();
          }
     }
               int b = 7;
               for(int c = 0; c < 8; c++){
                    if (M[place].neighbor[c] != -2)
                         M[M[place].neighbor[c]].edge[b] = 1;
                    b--;
               }
}
//=====================================================================
char *boggle::trunkateword(char *word, int amt)
{
char *passback = new char[amt+1];
strncpy(passback, word, amt);
passback[amt] = '\0';
//cout<<"passback from trunkateword is: "<<passback<<endl;
return passback;
}
//=====================================================================

int boggle::binarysearch()
{
char *word = new char[path->count() + 1];     //'word' may hold a whole valid word,
                                              //or just a part of a word, it will be
                                              //checked against the english via
                                              //binary search.  path->count makes the
                                              //size of the array exactly as large as
                                              //it needs to be.
//cout<<"the size of the stack is: "<<(path->count())<<endl;

bool keepgoing = false;
int trunkateletters = path->count();    //when we are looking for a word such as
                                        // 'supp' we will not find this in the
                                        //englishwords, we need to trunkate the
                                        //english words, so we will continue looking
                                        //for words such as 'supply'.  however
                                        //we do not want to waste energy looking
                                        //for words that start with 'zxpim" because
                                        //there are none.  trunkate handles this.

word = path->getstringstack();     //from our stack of characters,
                                   //getstringstack assembles the chars
                                   //and puts it into a string.

//cout<<"test: "<<word<<endl;

int lowindex = 0, highindex = sizeofenglish, endsearch, testnumber;
                      //lowindex and highindex is how the computer skipps most
                      //of the 40,000 items, the computer looks half way from
                      //top to bottom and checks if the word that it wants is up or down,
                      //if up, then it cuts that in half, and checks the middle.



//cout<<"sizeofenglish is: "<<sizeofenglish<<endl;
//testnumber = lowindex + ((highindex - lowindex)/2);
                       //test number is the spot in the 'englishwords' array that
                       //it is currently checking, this equation cuts the run in
                       //half, cutting the workload from N to Log base 2(n).
while (true)
{
     testnumber = lowindex + ((highindex - lowindex)/2);
     endsearch = highindex + lowindex;
     //cout<<"our test number is: "<<testnumber<<", and our word is: "<<english[testnumber].chrword<<endl;
                         //strcmp takes char * only, when the left is smaller than
                         //the right, the return is less than zero.

//cout<<"compare this: "<<trunkateword(english[testnumber].chrword, trunkateletters)<<"."<<endl;
//cout<<"to this: "<<word<<"."<<endl;

	if (strcmp(trunkateword(english[testnumber].chrword, trunkateletters), word) == 0)
		 keepgoing = true;

	if (strcmp(word, english[testnumber].chrword) == 0){

         //cout<<word<<" WAS found a perfect match"<<endl;
         return 1;
     }
     else if (strcmp(english[testnumber].chrword, word) > 0){
          highindex = testnumber;//(highindex - (highindex - lowindex) / 2);
          //cout<<" We had to shuffle down"<<endl;
     }
     else if (strcmp(english[testnumber].chrword, word) < 0){
          lowindex = testnumber; //lowindex + ((highindex - lowindex) / 2);
          //cout<<"   We had to shuffle up"<<endl;
     }
     else{
          cout<<"you thucked up numba six"<<endl;
     }
     if (endsearch == (highindex + lowindex)){
          if (keepgoing)
               return 0;
          //cout<<"   Endsearch has been reached, no words start with this, nor any matches"<<endl;
          return -1;
               //if highindex or lowindex didnt change then the letters are not here
     }
}
}
//=====================================================================
void boggle::displayboard(){

for (int a = 0; a < 16; a=a+4){
for(int b = a; b < a+4; b++){
     cout<<M[b].edge[1]<<" ";
     cout<<M[b].edge[0]<<" ";
     cout<<M[b].edge[4]<<"        ";
}
cout<<endl;

for(int b = a; b < a+4; b++){
     cout<<M[b].edge[2]<<" ";
     cout<<M[b].value<<" ";
     cout<<M[b].edge[5]<<"        ";
}
cout<<endl;
for(int b = a; b < a+4; b++){
     cout<<M[b].edge[3]<<" ";
     cout<<M[b].edge[7]<<" ";
     cout<<M[b].edge[6]<<"        ";
}
cout<<endl<<endl;
}
for(int x = 0; x < 16; x=x+4){
     checkrelations(x, 1, 0, 4);
     checkrelations(x, 2, 20, 5);
     checkrelations(x, 3, 7, 6);
     cout<<endl;
}
}
//=====================================================================
void boggle::checkrelations(int startfrom, int position1, int position2, int position3)
{
     for (int x = startfrom; x < startfrom+4; x++){
          if (M[x].neighbor[position1] != -2){
               if (M[x].neighbor[position1] > 9)
                    cout<<M[x].neighbor[position1]<<" ";
               else
                    cout<<M[x].neighbor[position1]<<"  ";
          }
          else{
               cout<<"  ";
          }
          if (position2 == 20)
               cout<<M[x].value<<"  ";
          else
          if (M[x].neighbor[position2] != -2){
               if (M[x].neighbor[position2] > 9)
                    cout<<M[x].neighbor[position2]<<" ";
               else
                    cout<<M[x].neighbor[position2]<<"  ";
          }
          else{
               cout<<"  ";
          }
          if (M[x].neighbor[position3] != -2){
               if (M[x].neighbor[position3] > 9)
                    cout<<M[x].neighbor[position3]<<" ";
               else
                    cout<<M[x].neighbor[position3]<<"  ";
          }
          else{
               cout<<"  ";                       //between letters,
          }
          cout<<"  ";       //between groups of 3,
     }
cout<<endl;
}
//=================================================
int boggle::ReturnAllWords(char **&allwords)
{
ifstream data;
char *n = new char[30];
int z = 0;
data.open("englishrevise.txt", ios::in);     //split them up
if (!data)
data.open("wordsfile.txt", ios::in);     //split them up
if (!data)
data.open("wordsfile.txt.txt", ios::in);     //split them up
if (!data)
data.open("wordsfile", ios::in);     //split them up
if (!data)
        return -1;

char **answers = new char*[180000];
for(int a = 0; a < 180000; a++)
        answers[a] = new char[16];

int x = 0;

while (!data.eof()){
     data >> answers[x];
     x++;
}
for (int y = 0; y < x; y++)
{
     strcpy(allwords[y], answers[y]);
}
return x;
}
//==================================================
void boggle::clearedges()
{
for(int a = 0; a < 16; a++)
     for(int b = 0; b < 8; b++)
          M[a].edge[b] = 1;
for (int a = 0; a < 16; a++){
if (M[a].x == 0){
     M[a].edge[0] = 0;
     M[a].edge[4] = 0;
     M[a].edge[1] = 0;
}
if (M[a].x == 3){
     M[a].edge[3] = 0;
     M[a].edge[7] = 0;
     M[a].edge[6] = 0;
}
if (M[a].y == 0){
     M[a].edge[1] = 0;
     M[a].edge[2] = 0;
     M[a].edge[3] = 0;
}
if (M[a].y == 3){
     M[a].edge[4] = 0;
     M[a].edge[5] = 0;
     M[a].edge[6] = 0;
}
}
}
//============================================
char **boggle::getanswers(char **&answers)
{
        for(int x = 0; x < answercounter; x++)
        {
                strcpy(answers[x], answerarray[x]);
        }
}
//============================================
int boggle::getnumanswers()
{
        return answercounter;
}
//==============================================================
void boggle::sortanswerwords()
{
char **temparray;
int smallest;
int sz, location;

temparray = new char *[1000];
for(int x = 0; x < 1000; x++)
{
        temparray[x] = new char[16];
}
for(int x = 0; x < answercounter; x++)
{
        smallest = 100;
        for(int y = 0; y < answercounter; y++)
        {
                sz = SizeWord(answerarray[y]);
                if ((sz != 0) && (sz < smallest))
                {
                        smallest = sz;
                        location = y;
                }
        }
        strcpy(temparray[x], answerarray[location]);
        strcpy(answerarray[location], "");
}
for(int x = 0; x < answercounter; x++)
{
        strcpy(answerarray[x], temparray[x]);
}
}
int boggle::SizeWord(char *word)
{
string n;
n = word;
return n.length();
}