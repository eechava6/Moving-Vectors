# Descripción del Algoritmo Serial

Se tiene un conjunto de noticias en texto libre, sobre el cual se desea calcular:

Índice Invertido, donde por cada palabra que se ingrese por teclado, se liste en orden descendente por frecuencia de palabra en el contenido <content> de la noticia, las noticias más relevantes. Listar máx 10 <frec,doc_id,title>.

        frec = frecuencia de la palabra en la noticia <id>
        id = id de la noticia
        title = título de la noticia.

# 1. Análisis
## 1.1 Descripción

El algoritmo como ya se menciono anteriormente pretende dado 3 documentos que contienen noticias, listar los 10 registros que contengan la palabra que se ingrese mas veces
por lo cual se empezo por leer los documentos csv, para lo cual se utilizo una tecnica de pre procesamiento en python ya que c++ no posee una herramienta
que facilite la lectura de estos, por tanto se procede a leer los archivos usar slo lascolumnas 1,2 y 9 que corresponden al id,title, content respectivamente
y luego title y content se transformaron a lowercase para que fuera mas facil la busqueda de la palabra, por ultimo se concatenaron todos en un mismo .csv.
Para el algorimo de c++ sehace la lectura del archivo y se procede a crear una struct que sera cada noticia que tiene como argumentos id, title,content
y se tiene un vector que almacena todas estas noticias leidas, luego se hace la lectura de la palabra y se pasa a lowercase, por consiguiente sigue 
el conteo de la palabra en title y content por cada noticia, para luego dar paso al ordenamiento y laimpresión de los resultados del algoritmo.

Este es el basico de funcionamiento del algoritmo entonces para OpenMP se agregaron el uso de procesamiento paralelo en la sección del conteo 
de las palabras, el cual es independiente de cualquiera de los otros procesos del algoritmo, por tanto es la unica parte donde se pudo realizar
elprocesamiento paralelo, para la lectura del archivo no se agrego porque se vio innecesario y además agregaba más tiempo de ejecución,
para elordenamiento es imposible porque es necesario el resultado anterior parapoder comparar cual es el mayor.
Por tanto:

* Lectura de archivo (Se puede paralelizar con OpenMP pero no muestra mejora).

* Conteo de palabras (Fue posible su paralelización con OpenMP y muestra mejora significativa).

* Ordenamiento (No se puede ralizar con OpenMP)
  
Estas son las fases del algoritmo de Text-Analytics.
## 1.2 ¿Cómo funciona el algoritmo?
 
 Se debe descargar del github el proyecto
        
        [user@hdpmaster]$ git clone https://github.com/eechava6/Text-Analytics.git

Luego se accede al directorio para empezar a trabajar sobre este
      
       [user@hdpmaster]$ cd Text-Analytics/
       
Aqui se deben hacer el pre procesamiento de los datos para que nuestro algoritmo funcione de la manera correcta para lo cual es necesario

       [user@hdpmaster Text-Analytics] $ python src/python/pre-process.py
       
Una vez ejecutado la anterior linea, nos quedara un archivo csv llamado results que contiene todas las noticias separadas por ";" y en minusculas
El siguiente paso es compilar el programa openmp en c++
      
       [user@hdpmaster Text-Analytics]$ g++ src/c++/openmp/main.cpp -o openmp -fopenmp

Y para ejecutarlo simplemente se ejecuta lo siguiente:

          [user@hdpmaster Text-Analytics]$ ./openmp
          
 
## 1.2 Definición de tecnología de desarrollo para el algoritmo:

* Lenguaje de Programación: C++
* Técnica Aplicada: OpenMP
