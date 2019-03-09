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
  //lee el csv ya procesado por python  
  while (getline(fin, line))
    {
      istringstream ss(line);
      Person person;
      //extrae las columnas y las guarda en la struct en su correspondiente espacio
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
  
  int words[persons.size()+1];
  while(cin>>word){
    if(word=="/"){
      return 0;
    }
    //Cambia la palabra a lower ya que el dataset fue cambiado a minusculas
  transform(word.begin(),word.end(),word.begin(),::tolower);
  cout<<"Word searched is : "<<word<<endl;
  string str;
  string str2;
  //Itero por cada objeto de la struct para poder hacer el count de cada palabra por content y titulo
  for(unsigned int i=0;i<persons.size();i++){
  str = persons[i].content;
  str2 = persons[i].title;
  //cout<<str<<endl;
  stringstream ss(str);
  stringstream ss2(str2);
  int cont=0;
  //Verifica en el content cuantas repeticiones de la palabra encuentra
  while(ss>>str){
    cout << "Str in content is: " << str << endl;
    if(word==str)
    cout << "Word : "<<word<<  " match with Str " <<endl; 
      cont++;    
  }
  //Verifica en el title cuantas repeticiones de la palabra encuentra
  while(ss2>>str2){
    cout << "Str in title is: " << str2 << endl;
    if(word==str2){
      cout << "Word : "<<word<<  " match with Str " <<endl; 
      cont++;
    }
  }
  //Almacena en un arreglo para luego poder hacer el sort
  words[i]=cont;
  }

  for(int i = 0; i <= (sizeof(words)/sizeof(*words)); i++){
    cout << "Word " << i << " : "<<words[i] << "\n";
  }
  //Busca el elemento mas grande del arreglo, pero dudo del count
 cout << "The largest element is "  << *std::max_element(words,words+7) << '\n';
  }
}

