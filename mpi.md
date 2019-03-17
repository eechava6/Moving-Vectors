# Descripción del Algoritmo Serial

Se tiene un conjunto de noticias en texto libre, sobre el cual se desea calcular:

Índice Invertido, donde por cada palabra que se ingrese por teclado, se liste en orden descendente por frecuencia de palabra en el contenido <content> de la noticia, las noticias más relevantes. Listar máx 10 <frec,doc_id,title>.

        frec = frecuencia de la palabra en la noticia <id>
        id = id de la noticia
        title = título de la noticia.

# 1. Análisis

Estas son las fases del algoritmo de Text-Analytics.
## 1.2 ¿Cómo funciona el algoritmo?
 
 Se debe descargar del github el proyecto
        
        [user@hdpmaster]$ git clone https://github.com/eechava6/Text-Analytics.git

Luego se accede al directorio para empezar a trabajar sobre este
      
       [user@hdpmaster]$ cd Text-Analytics/
       
Aqui se deben hacer el pre procesamiento de los datos para que nuestro algoritmo funcione de la manera correcta para lo cual es necesario

       [user@hdpmaster Text-Analytics] $ python src/python/pre-process-for-mpi.py
       
Una vez ejecutado la anterior linea, nos quedara un archivo csv llamado results que contiene todas las noticias separadas por ";" y en minusculas
El siguiente paso es compilar el programa MPI en c++
      
       [user@hdpmaster Text-Analytics]$ mpicc src/c++/mpi/main.cpp -lstdc++ -o mpi

Y para ejecutarlo simplemente se ejecuta lo siguiente:

          [user@hdpmaster Text-Analytics]$ mpirun -f hosts_mpi -np 4 ./mpi
          
 
## 1.2 Definición de tecnología de desarrollo para el algoritmo:

* Lenguaje de Programación: C++
* Técnica Aplicada: MPI
