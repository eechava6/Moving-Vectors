#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
 

using namespace std;

struct Columns {
  
  //id,title,publication,author,date,year,month,url,content

  int id;
  string title;
  string content;
};

int main()
{
  ifstream fin("results.csv");
  if (!fin)
    {
      cout << "File not open\n";
      return 1;
    }
    
  vector<Columns> filtered;
    
  string line;
  string word;
  string token, mystring;
  const char delim = ';';
  int i = 0;
  //lee el csv ya procesado por python  
  while (getline(fin, line))
    {
      istringstream ss(line);
      Columns preFiltered;
      //extrae las columnas y las guarda en la struct en su correspondiente espacio
      while(token != line){
      token = line.substr(0,line.find_first_of(delim));
      line = line.substr(line.find_first_of(delim) + 1);
      ss >> preFiltered.id; ss.ignore(10, delim); 
            getline(ss, preFiltered.title,delim);
      getline(ss, preFiltered.content,delim);
            if (ss)
        filtered.push_back(preFiltered);
      }
    
    }
  
  int words[filtered.size()+1];
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
  for(unsigned int i=0;i<filtered.size();i++){
    str = filtered[i].content;
    str2 = filtered[i].title;

    stringstream ss(str);
    stringstream ss2(str2);
    int cont=0;
    //Verifica en el content cuantas repeticiones de la palabra encuentra
      while(ss>>str){
        if(word==str)
        cout << "Word : "<<word<<  " match with Str " <<endl; 
          cont++;    
      }
    //Verifica en el title cuantas repeticiones de la palabra encuentra
      while(ss2>>str2){
        if(word==str2){
          cout << "Word : "<<word<<  " match with Str " <<endl; 
          cont++;
        }
  }
  //Almacena en un arreglo para luego poder hacer el sort
  words[i]=cont;
  }
  int count = 0;
  int index = 0;
  for(int i = 0; i <= (sizeof(words)/sizeof(*words)); i++){
    if(words[i] >= count){
      index = i;
    }
  }

  cout << "Title : "<< filtered[index].title << " has : " << index << " times the word : " << word <<endl;
  //Busca el elemento mas grande del arreglo, pero dudo del count
 cout << "The largest element is "  << *std::max_element(words,words+7) << '\n';
  }
}

