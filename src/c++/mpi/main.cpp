#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
#include <ctime>
#include "mpi.h"
#include <ctime>

MPI_Status status;

using namespace std;

struct Columns {
  //Del CSV original conservamos unicamente ID, título y contenido.

  int id;
  string title;
  string content;
};


void swap(int *xp, int *yp) 
{
  //Intercambia entre 2 elementos
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
void selectionSort(int arr[],int indexes[], int n) 
{ 
    int i, j, max = 0; 
    //Solo ordenamos hasta tener los 10 primeros valores más grandes
    for (i = 0; i < 10; i++) 
    { 
        // Encuentra el valor más grande en un arreglo sin ordenar 
        max = i; 
        for (j = i+1; j < n; j++){
          if (arr[j] > arr[max]) {
            max = j; 
          }
        } 
        // Cambia el máximo con el primero.
        swap(&arr[max], &arr[i]); 
        swap(&indexes[max], &indexes[i]); 
    } 
    
}
//Funcion creada para la lectura del archivo csv enviado 
vector<Columns> archivos(string nombre){
vector<Columns> filtered;  
 ifstream fin(nombre.c_str());
  if (!fin)
    {
      cout << "File not open\n";
      return filtered;
    }    
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
  return filtered;
}
//Funcion que hace el conteo de la paabra enviada
int * conteo(vector<Columns> filtered,string word){
int words[filtered.size()+1];
  int indexes[filtered.size()+1];
    //Cambia la palabra a lower ya que el dataset fue cambiado a minusculas
  transform(word.begin(),word.end(),word.begin(),::tolower);
  cout<<"Starting count for word : '"<<word << "' ...."<<endl;

  //Comienza a contar el tiempo
  const clock_t begin_time = clock();
  int size = filtered.size();
  
  //Itero por cada objeto de la struct para poder hacer el count de cada palabra por content y titulo
  int i = 0;
  int cont = 0;
  int tid = 0;
  string str = "";
  string str2 = "";

  //Por cada articulo se revisa la cantidad de palabras
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
  

  //Llena un arreglo con los indices de cada articulo (Es decir la posición en el arreglo)
  for(int i = 0; i < filtered.size(); i++){
    indexes[i] = i;
  }

  /*Se Ejecuta el algoritmo Selection Sorts, ordena de forma Max - Min
  el arreglo words tendrá en orden ascendente los valores de la palabra buscada
  mientras que indexes tendrá los "Intercambios" de posición
  que permiten relacionar los valores con un articulo.*/
  selectionSort(words,indexes,filtered.size());
  
  //Se imprime en orden ascendente los 10 primeros resultados con el articulo.
  for(int i = 0; i < 10; i++){
     cout << words[i] <<" times found in : '"<< filtered[indexes[i]].title <<"'"<< endl;
  }

  //Termina el tiempo para el ordenamiento.
 
  float total = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
  //imprime los resultados
  cout << "\n \n Total time was : " << total << endl;
  return words;
  cout <<" What word do you want to search for? \n";
}



int main(int argc, char *argv[])
{
  char inmsg[30];
  string line;
  string word;
  int i=0;
  int taskid,numtasks,len,numworkers,dest;
  char name[MPI_MAX_PROCESSOR_NAME];
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Get_processor_name(name, &len);
  numworkers = numtasks-1;
  //cout<<numworkers<<endl;
  cout << "What word do you want to search for? \n";
  //cin >> word;
   if (taskid == 0) {
    cout<<"Im in 0"<<endl;
    cin>>word;
      if(word=="/")
	return 0;
    int n = word.length();  
    char char_array[n + 1];  
    strcpy(char_array, word.c_str()); 
    for (dest=1; dest<=numworkers; dest++)
    MPI_Send(&char_array,strlen(char_array),MPI_CHAR,dest,1,MPI_COMM_WORLD);
    vector<Columns> filtered;
    filtered = archivos("results1.csv");
   }
  else if (taskid == 1) {
    //cout<<"Im in 1"<<endl;
    memset(inmsg, 0, 30);
    MPI_Recv(&inmsg,30, MPI_CHAR, 0, 1, MPI_COMM_WORLD,  MPI_STATUS_IGNORE);
    printf("Slave 1 on processor %s listening for Tag2 received this  message:\n   %s\n",name,inmsg);
    vector<Columns> filtered;
filtered = archivos("results2.csv");
 int *numeros;
 string word_slaveo =inmsg;
 numeros=conteo(filtered,word_slaveo);
  }
  else if (taskid == 2) {
    //cout<<"Im in 2"<<endl;
    memset(inmsg, 0, 30);
    MPI_Recv(&inmsg,30, MPI_CHAR, 0, 1, MPI_COMM_WORLD,  MPI_STATUS_IGNORE);
    printf("Slave 2 on processor %s listening for Tag2 received this  message:\n   %s\n",name,inmsg);
    vector<Columns> filtered;
filtered = archivos("results2.csv");
}
MPI_Finalize();

}  
