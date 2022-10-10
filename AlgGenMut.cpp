#include <iostream>
#include <time.h>
#include <windows.h>
using std::cout;
using std::string;
using std::endl;
string MPadres[10];

void generaraleatorios(string[]);
void cruzar(string[]);
string Chijo(string,string,int);
int contarceros(string);
void ordenar(string[]);
void imprimir(string[]);
string mutacion(string);

int main(){
    int gen;
    do{
        system("cls");
        cout<<"Ingresa el numero de generaciones que quieres que se generen ---> ";
        std::cin>>gen;
    }while(gen<=0||gen>1000);
    
    for(int i=0;i<gen;i++){
        cout<<"\nGeneracion "<<i+1<<endl<<endl;
        if(i==0){generaraleatorios(MPadres);}
        cout<<"Arreglo con los Padres Originales\n\n";
        //Sleep(100);
        ordenar(MPadres);
        imprimir(MPadres);
        cout<<endl;
        cruzar(MPadres);
        ordenar(MPadres);
        cout<<"Arreglo con los Hijos ordenado del mas al menos puro\n\n";
        //Sleep(100);
        imprimir(MPadres);
        cout<<endl;
        // system("pause");
        bool pureza = true;
        for(int i=0;i<10;i++){
            if(contarceros(MPadres[i])!=10){
                pureza = false;
            }
        }
        if(pureza == true){
            cout<<"Terminado en generacion "<<i+1<<endl;
            system("pause");
            return 0;
        }
    }
    cout<<"\nNo se obtuvo un elemento puro en ninguna generacion\n\n"<<endl;
    system("pause");
    return 0;
}

void generaraleatorios(string matriz[]){
    srand(time(NULL));
    for(int i=0;i<10;i++){
        matriz[i].resize(10); // les asignamos inicialmente un tamaño para que se pueda trabajar con ellos
        for(int j=0;j<10;j++){
            if((rand() %100) % 2 == 0){ //el valor del espacio dependerá del aleatorio de 0 a 100, si es par, se pone un 1, sino un 0
                matriz[i][j] = '1';
            }
            else{
                matriz[i][j] = '0';
            }
        }
    }
}

void cruzar(string matriz[]){
    int conteo=0;
    int pcruce;
    int final=0, inicial=0; //asignaremos indices aleatorios a los padres, no será el primero con el último
    int mutados=0;

    bool apartados[10];
    for(int i=0;i<10;i++){
        apartados[i] = false; //se genera un arreglo que cuenta las cadenas que ya fueron usadas
    } 
    
    //se declaran los valores iniciales de inicio y final, que están en posiciones opuestas
    string padre1,padre2,hijo1,hijo2; //se crean los padres e hijos

    while(conteo!=3){ //solo se puede hacer el cambio tres veces, de 0-2, solo se seleccionarán 6 padres de los 10 elementos en lugar de 10 padres con todos los elementos
        do{
            inicial = rand() % 10; //busca un valor alatorio entre 0 y 9
        }while(apartados[inicial]==true); //solo lo toma si su índice no ha sido usado antes
        apartados[inicial]=true; //en cuanto lo toma, ya no está disponible posteriormente

        do{
            final = rand() % 10;
        }while(apartados[final]==true); //buscaremos forzozamente un valor diferente al indice de inicial para el final y que no esté ya ocupado
        apartados[final]=true;

        padre1 = matriz[inicial]; //empezaremos a sustituir valores del padre1 y padre2, que serán el primer y último elemento de la cadena respectivamente de lo que valgan inicial y final
        padre2 = matriz[final];

        pcruce = rand()%10;
        hijo1 = Chijo(padre1,padre2,pcruce); //generamos el hijo a partir de p1 y p2
        hijo2 = Chijo(padre2,padre1,pcruce); //generamos el hijo a partir de p2 y p1

        // cout<<"Hijos de Elemento "<<inicial+1<<" y "<<final+1<<endl;
        // cout<<hijo1<<endl;
        // cout<<hijo2<<endl<<endl;
        
        // cout<<"Hijos mutados "<<inicial+1<<" y "<<final+1<<endl;
        // hijo1 = mutacion(hijo1); //mutamos el hijo, y retorna la mutación, si es que muta en primer lugar
        // hijo2 = mutacion(hijo2);

        // cout<<hijo1<<endl;
        // cout<<hijo2<<endl<<endl;

        if(contarceros(hijo1)>contarceros(padre1)){ //usamos contarceros para saber si el h1 tiene más 0's que el p1
            matriz[inicial] = hijo1;
            if(contarceros(hijo2)>contarceros(padre2)){ //si ocurre, ahora compararemos el h2 con el p2
                matriz[final] = hijo2;
            }
        }
        else if(contarceros(hijo1)>contarceros(padre2)){ //usamos contarceros para saber si el h1 tiene más 0's que el p2
            matriz[final] = hijo1;
            if(contarceros(hijo2)>contarceros(padre1)){ ///si ocurre, ahora compararemos el h2 con el p1
                matriz[final] = hijo2;
            }
        }

        if(inicial>4 && mutados!=2 && contarceros(matriz[inicial])!=10){ //escoge solo los últimos 5 elementos para mutar y solo puede escoger 2 que no sean puros
            matriz[inicial]=mutacion(matriz[inicial]);
            mutados++;
        }
        if(final>4 && mutados!=2 && contarceros(matriz[final])!=10){
            matriz[final]=mutacion(matriz[final]);
            mutados++;
        }

        conteo++;
    }
    for(int i=0;i<10;i++){
        apartados[i] = false;
    } 
}

string Chijo(string pa1, string pa2,int puntocruce){ //Crearhijo funciona tomando los primeros 4 caracteres de la primera cadena con los ultimos 6 c. de la 2da cadena
                                      //por eso se acomodan diferente para crear el 1er o 2do hijo respectivamente
    string hijo;
    hijo.resize(10);
    for(int i=0;i<10;i++){
        
        if(i<puntocruce){
            hijo[i] = pa1[i];
        }
        else{
            hijo[i] = pa2[i];
        }
    }
    return hijo; //te regresa la nueva cadena ya modificada
}

int contarceros(string elemento){
    int ceros = 0;
    for(int i=0;i<10;i++){  //recorre la cadena con un for para contar cuantos 0's tiene la cadena
        if(elemento[i] == '0')
            ceros++;
    }
    return ceros; //retorna ese conteo
}

void ordenar(string arreglo[]){ //usamos el método de burbuja para ordenar de mayor a menor, hablando de 0's, el arreglo
    string temp;
    for(int i=0;i<10;i++){
        for(int j=0;j<9;j++){
            if(contarceros(arreglo[j])<contarceros(arreglo[j+1])){
                temp = arreglo[j];
                arreglo[j] = arreglo[j+1];
                arreglo[j+1] = temp;
            }
        }
    }
    
}

void imprimir(string arreglo[]){ //se imprime el arreglo seleccionado
    for(int i=0;i<10;i++){
        cout<<"Elemento "<<i+1<<":\t"<<arreglo[i]<<" "<<contarceros(arreglo[i])<<endl;
        //Sleep(100);
    }
}

string mutacion(string arreglo){
    arreglo.resize(10);
    int tope = (rand()%4)+1; //aleatorio de 1 a 4, el límite de elementos a mutar
    int mutados =0;
    int aleatorio;

    bool apartados[10];
    for(int i=0;i<10;i++){
        apartados[i] = false; //se genera un arreglo que cuenta las cadenas que ya fueron usadas
    } 

    for(int i=0;i<10;i++){
        do{
            aleatorio = rand() % 10; //busca un valor alatorio entre 0 y 9
        }while(apartados[aleatorio]==true); //solo lo toma si su índice no ha sido usado antes
        apartados[aleatorio]=true; //en cuanto lo toma, ya no está disponible posteriormente

        if(mutados!=tope){ //si el aleatorio es menor o igual a tope*10, tope*10% de probabilidad de mutación
            if(arreglo[i] == '0'){
                arreglo[i]='1';
            }
            else{
                arreglo[i]='0';
            }
            mutados+=1; // en cuanto se muta, se impide que se muten nuevamente.
        }
    }
    return arreglo;
}
