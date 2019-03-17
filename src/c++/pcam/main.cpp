#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
#include <ctime>
#include "mpi.h"
#include <omp.h>
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
  //int i = 0;
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

//Funcion que hace el conteo de la palabra enviada
int* conteo(vector<Columns> filtered,string word){
  int words[filtered.size()+1];
  int indexes[filtered.size()+1];
  //Cambia la palabra a lower ya que el dataset fue cambiado a minusculas
  transform(word.begin(),word.end(),word.begin(),::tolower);

  int size = filtered.size();
  
  //Itero por cada objeto de la struct para poder hacer el count de cada palabra por content y titulo
  #pragma omp parallel 
  {
  int i = 0;
  int cont = 0;
  string str = "";
  string str2 = "";

  //Por cada articulo se revisa la cantidad de palabras
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

  //Llena un arreglo con los indices de cada articulo (Es decir la posición en el arreglo)
  for(unsigned int i = 0; i < filtered.size(); i++){
    indexes[i] = i;
  }

  /*Se Ejecuta el algoritmo Selection Sorts, ordena de forma Max - Min
  el arreglo words tendrá en orden ascendente los valores de la palabra buscada
  mientras que indexes tendrá los "Intercambios" de posición
  que permiten relacionar los valores con un articulo.*/
  selectionSort(words,indexes,filtered.size());
    
  //Se imprime en orden ascendente los 10 primeros resultados con el articulo.
  int* results = new int[20];
  for(int i = 0; i < 10;i++){
    results[i] = indexes[i];
  }
  for(int i = 10; i < 20;i++){
    results[i] = words[i-10];
  }

  return results;
}



int main(int argc, char *argv[])
{
  //Control Variables
  char inmsg[30];
  string word;
  int i=0;
  int taskid,numtasks,len,numworkers,dest,source;
  char name[MPI_MAX_PROCESSOR_NAME];
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Get_processor_name(name, &len);
  numworkers = numtasks-1;
  //Para después jeje const 
  /*
    clock_t begin_time = clock();
    float total = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
      //imprime los resultados
      cout << "\n \n Total time was : " << total << endl;

      //PENDIENTE POR BORRAR
      cout <<" What word do you want to search for? \n";
  */
  //Si es master haga: 
  if (taskid == 0) {
    
    cout << "What word do you want to search for? \n";
    cin>>word;
    //Si '/' acabe el programa
    if(word=="/"){
      return 0;
    }
    double start = MPI_Wtime();
    //Calcule la longitud para luego crear un arreglo de Chars del string
    int n = word.length();  
    char char_array[n + 1];  

    //Copia el contenido del string en el arreglo de caracteres
    strcpy(char_array, word.c_str()); 

    //Por cada Slave envie la palabra al slave
    for (dest=1; dest<=numworkers; dest++){
      MPI_Send(&char_array,strlen(char_array),MPI_CHAR,dest,1,MPI_COMM_WORLD);
    }
    //Haga el conteo del archivo "Results1.csv"
    vector<Columns> filtered;
    vector<Columns> filtered2;
    vector<Columns> filtered3;

    filtered = archivos("results1.csv");
    filtered2 = archivos("results2.csv");
    filtered3 = archivos("results3.csv");
    
    //Recibe los 10 primeros mayores
    
    int* indexes = new int[20];
    indexes=conteo(filtered,word);
    
    //Para los resultados totales. 
    string names[30];
    int* counts = new int[30];
    int* resultIndexes = new int[30];

    //Espera hasta que se le envien los resultados
    for (i=1; i<=2; i++){
      int* b = new int[20];
      source = i;
      MPI_Recv(b, 20, MPI_INT, source, 2, MPI_COMM_WORLD, &status);

      if(i == 1){
        for(int j = 0; j < 10; j++){
          //Guarda en diferentes posiciones los valores del conteo.
          resultIndexes[j] = j;
          counts[j]= b[j+10];
          names[j] = filtered2[b[j]].title;
       }
      }
 
      if(i == 2){
        for(int j = 0; j < 10; j++){
          //Guarda en diferentes posiciones los valores del conteo.
          resultIndexes[j+10] = j+10;
          counts[j+10]= b[j+10];
          names[j+10] = filtered3[b[j]].title;
       }
      }
    }

    //Guarda los 10 resultados del archivo1 en el total.
    for(int i = 0; i < 10; i++){
      resultIndexes[i+20] = i+20;
      counts[i+20] = indexes[i+10];
      names[i+20] = filtered[indexes[i]].title;
    }

    //Ordena los 10 primeros
    selectionSort(counts,resultIndexes,30);

    //Imprime los resultados
    for(int i = 0; i < 10; i++){
          //Imprime todo
          cout << counts[i] << " times found in : " << names[resultIndexes[i]] << endl;
    }
    printf("TOTAL time: %lf\n", MPI_Wtime() - start);

  }

  else if (taskid == 1) {
    //Reservar el tamaño para el mensaje
    memset(inmsg, 0, 30);
    //Reciba el mensaje y ubiquelo en la posición reservada.
    MPI_Recv(&inmsg,30, MPI_CHAR, 0, 1, MPI_COMM_WORLD,  MPI_STATUS_IGNORE);
    printf("Slave 1 on processor %s listening for Tag2 received this  message:\n   %s\n",name,inmsg);
    //Haga el conteo del archivo "Results2.csv"
    vector<Columns> filtered;
    filtered = archivos("results2.csv");
    //Recibe los 10 primeros mayores
    string word_slaveo =inmsg;
    int *indexes;
    indexes=conteo(filtered,word_slaveo);
    //Envio de resultados
    cout<<"Sending to the master from 1"<<endl;
    MPI_Send(indexes, 20, MPI_INT, 0, 2, MPI_COMM_WORLD);    
  }
  else if (taskid == 2) {
    //Reservar el tamaño para el mensaje
    memset(inmsg, 0, 30);
    //Reciba el mensaje y ubiquelo en la posición reservada.
    MPI_Recv(&inmsg,30, MPI_CHAR, 0, 1, MPI_COMM_WORLD,  MPI_STATUS_IGNORE);
    printf("Slave 2 on processor %s listening for Tag2 received this  message:\n   %s\n",name,inmsg);
    //Haga el conteo del archivo "Results2.csv"
    vector<Columns> filtered;
    filtered = archivos("results3.csv");
    //Recibe los 10 primeros mayores
    string word_slaveo =inmsg;
    int *indexes;
    indexes=conteo(filtered,word_slaveo);
    cout<<"Sending to the master from 2"<<endl;
    MPI_Send(indexes, 20, MPI_INT, 0, 2, MPI_COMM_WORLD);  
  }
MPI_Finalize();

}  
