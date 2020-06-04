#include <iostream>
//#include "neuron.cpp"
#include "neuron.h"
  
using namespace std;

int main()
{
  
    vector < double > inputs (6);
    vector < double > vel_motores (2);
  
    float punto[3];
    float distancias[6];

//Vector con las capas y el numero de neuronas para construir la red neuronal
    vector< int > capas = {6,15,15,15,2};
  
    //Vectores de entrenamiento: input (entrenador1) y output deseado (entrenador2)
    //Orden de los sensores: {0,1,2,3,4,5}
    //Los valores del vector se corresponden a la distancia que marcan los sensores
    vector< vector< double > > entrenador1 = {
    {0.23,1,1,1,1,1},
    {0.23,0.23,1,1,1,1},
    {0.12,0.12,1,1,1,1},
    {1,1,1,1,1,1},
    {1,1,0.12,0.12,1,1},
    {1,1,0.23,0.23,1,1},
    {1,1,1,1,0.23,0.23},
    {1,1,1,1,1,0.23},
    {1,1,1,1,0.12,0.12},
    {1,1,1,0.23,1,1},
    {1,1,0.23,1,1,1},
    {1,1,1,0.12,0.12,1}};
    //Orden de los motores: {a,b}, a-> motor derecho, b->motor izquierdo
    //Los valores del vector se corresponden a la direccion de los motores
    vector< vector< double > > entrenador2 = {
    {1,0.5},
    {1,0.5},
    {1,0},
    {1,1},
    {0,0.75},
    {0.6,1},
    {0.5,1},
    {0.5,1},
    {0,1},
    {0,1},
    {0,1},
    {0,0.5}};

//Creamos la red
    cout << "Creando red..." << endl;
    Network red(capas);  
    cout << "Red creada" << endl;
 
    //Entrenamos la red con los ejemplos
    cout << "Entrenando red..." << endl;
    red.Aprendizaje_Prop_Atras(entrenador1, entrenador2);
    cout << "Red entrenada" << endl;
  
    red.Mostrar_Pesos(); //Mostramos los pesos definitivos
 
    //Mostrar los outputs:
    for(int i = 0; i < entrenador1.size(); i++)
    {
        red.Calcular_Output(entrenador1[i]);
        red.Mostrar_Output();
    }

   //Generamos el vector de inputs
            for(int i = 0; i < 6; i++)
            {
                inputs[i] = distancias[i];
            }
              
            vel_motores = red.Calcular_Output(inputs); //Calcular el output de la red neuronal
 
            //El output de las redes neuronales es un valor entre 0 y 1. Queremos mapearlo para que vaya de
            // -2 (motores giran hacia atras) a 2 (motores giran hacia delante).
            
   
    
  
    cout << "Simulacion finalizada" << endl; 
    return 0; 

}
