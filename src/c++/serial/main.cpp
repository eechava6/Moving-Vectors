#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
 

struct Person {
  
  //id,title,publication,author,date,year,month,url,content

  int id;
  std::string title;
  std::string content;
};

using namespace std;

int main()
{
  ifstream fin("results.csv");
  if (!fin)
    {
      cout << "File not open\n";
      return 1;
    }
    
  vector<Person> persons;
    
  string line;
  string word;
  string token, mystring;
  const char delim = ';';
  int i = 0;
  //bool bandera = true;  
  while (getline(fin, line))
    {
      istringstream ss(line);
      Person person;
      //ss >> person.id; ss.ignore(delim);
      while(token != line){
	token = line.substr(0,line.find_first_of(delim));
	line = line.substr(line.find_first_of(delim) + 1);
	ss >> person.id; ss.ignore(10, delim); 
        getline(ss, person.title,delim);
	getline(ss, person.content,delim);
        if (ss)
	  persons.push_back(person);
      }
    
    }
  cout<<persons.size()<<endl;
  int words[persons.size()+1];
  while(cin>>word){
    if(word=="/"){
      return 0;
    }
  transform(word.begin(),word.end(),word.begin(),::tolower);
  cout<<word<<endl;
  string str;
  string str2;
  for(unsigned int i=0;i<persons.size();i++){
  str = persons[i].content;
  str2 = persons[i].title;
  //cout<<str<<endl;
  stringstream ss(str);
  stringstream ss2(str2);
  int cont=0;
  while(ss>>str){
    if(word==str)
      cont++;    
  }
  while(ss2>>str2){
    if(word==str2){
      cont++;
    }
  }
  words[i]=cont;
  }
// vector<int> myvector (words, words+8);
//   sort(myvector.begin(), myvector.end(), greater<int>());
//   std::cout << "myvector contains:";
//   for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
//     std::cout << ' ' << *it;
  // for (unsigned int i=0; i< 1; i++){
  //   cout << persons[i].id<<endl;
  //   cout << persons[i].title<<endl;
  //   cout<< persons[i].content<<endl;
  // }
  }
}

