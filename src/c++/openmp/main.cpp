#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
#include <ctime>
#include <omp.h>

using namespace std;

struct Columns {
  
  //id,title,publication,author,date,year,month,url,content

  int id;
  string title;
  string content;
};

//Código tomado de : https://stackoverflow.com/questions/34752333/parallelize-selection-sort-using-openmp
struct Compare { int val; int index; };
#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)
void selectionSort(int* arr,int* indexes, int size)
{
  for (int i = size - 1; i > 0; --i)
    {
        struct Compare max;
        max.val = arr[i];
        max.index = i;
        #pragma omp parallel for reduction(maximum:max)
        for (int j = i - 1; j >= 0; --j)
        {
            if (arr[j] > max.val)
            {
                max.val = arr[j];
                max.index = j;
            }
        }
        int tmp = arr[i];
        int tmp2 = indexes[i];
        arr[i] = max.val;
        indexes[i] = max.val;

        arr[max.index] = tmp;
        indexes[max.index] = tmp2;
    }
  } 

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
  int indexes[filtered.size()+1];
  cout << "What word do you want to search for? \n";
  while(cin>>word){
    if(word=="/"){
      return 0;
    }
    //Cambia la palabra a lower ya que el dataset fue cambiado a minusculas
  transform(word.begin(),word.end(),word.begin(),::tolower);
  cout<<"Starting count for word : '"<<word << "' ...."<<endl;
  //Comienza a contar el tiempo
  double start_time = omp_get_wtime();
  int size = filtered.size();
  
  //Itero por cada objeto de la struct para poder hacer el count de cada palabra por content y titulo
  #pragma omp parallel 
  {
  int i = 0;
  int cont = 0;
  int tid = 0;
  string str = "";
  string str2 = "";
  
  #pragma omp for 
  for(i=0;i < size;i++){
    str = filtered[i].content;
    str2 = filtered[i].title;
    stringstream ss(str);
    stringstream ss2(str2);
    cont=0;
    //Verifica en el content cuantas repeticiones de la palabra encuentra
      while(ss>>str){
        if(word==str)
           cont++;   
      }
    //Verifica en el title cuantas repeticiones de la palabra encuentra
      while(ss2>>str2){
        if(word==str2){
           cont++;   
        }
  }
  //Almacena en un arreglo para luego poder hacer el sort
  words[i]=cont;
  }
  }
  //Calcula el tiempo final para el conteo.
  double countTime = omp_get_wtime() - start_time;

  
  //Comienza la captura de tiempo para el ordenamiento.
  cout << "Starting ordering for articles vector"<<endl;
  double startOrder_time = omp_get_wtime();

  //Llena un arreglo con los indices de cada articulo (Es decir la posición en el arreglo)
  for(int i = 0; i < filtered.size(); i++){
    indexes[i] = i;
  }

  /*Se Ejecuta el algoritmo Selection Sorts, ordena de forma Min - Max 
  el arreglo words tendrá en orden ascendente los valores de la palabra buscada
  mientras que indexes tendrá los "Intercambios" de posición
  que permiten relacionar los valores con un articulo.*/
  selectionSort(words,indexes,filtered.size());
  
  //Se imprime en orden ascendente los resultados con el articulo.
  for(int i = filtered.size()-1; i >= filtered.size()-10; i--){
     cout << words[i] <<" times found in : '"<< filtered[indexes[i]].title <<"'"<< endl;
  }

  //Termina el tiempo para el ordenamiento.
  double orderTime = omp_get_wtime() - startOrder_time;

  //imprime los resultados
  cout << "Count time was : " <<countTime << endl;
  cout << "Ordering time was :" <<orderTime << endl;
  cout << "Total time was : " << orderTime + countTime << endl;
  cout <<" \n \n What word do you want to search for? \n";
  }
}

