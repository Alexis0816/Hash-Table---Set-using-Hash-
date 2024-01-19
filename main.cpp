#include <iostream>
#include <string>
#include <forward_list>
#include "hash.h"

/*Integrantes:
- Daniel Casquino Paz
- Gabriel Blanco Gutierrez
- Alexis Raza Estrada*/

// Tests con el hashTable
int main(){
    HashTable<string, int> hash(5);
    hash.insert("luis", 10);
    hash.insert("fernanda", 1);
    hash.insert("roberto", 5);
    
    hash.insert("rubith", 7);
    hash.insert("raul", 6);
    hash.insert("gerardo", 4);
    hash.insert("alexis", 2);

    cout << "roberto: " << hash["roberto"] << endl;

    hash.insert("luis", 3);
    hash.insert("sally", 6);
    hash.insert("diego", 8);

    hash._delete("roberto");

    cout << "rubith: " << hash.find("rubith") << endl; // con función find()
    cout << "sally: " << hash["sally"] << endl<<endl; // con operador sobrecargado

    cout << "HashTable - contenido de la lista:\n\n";
    for (int i = 0; i < hash._size(); ++i){
        cout << "Lista #" << i << " contenido:";
        for (auto local_it = hash._begin(i); local_it != hash._end(i); ++local_it)
            cout << " " << local_it->key << ": " << local_it->value;
        cout << endl;
    }
    cout<< "Tamanio: "<<hash._size()<<endl;
    cout<< "Tamanio de la lista #"<<10<<": "<<hash.bucket_size(10)<<endl;

    
    return 0;
}
