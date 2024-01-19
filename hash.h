#include <iostream>
#include <forward_list>
#include <iterator>
#include <exception>
#include <functional>
using namespace std;

const int maxColision = 3;
const float maxFillFactor = 0.5;

template<typename TK, typename  TV>
class HashTable{
private:
    struct Entry{
        TK key;
        TV value;
    };
    using Set_from_Seq = forward_list<Entry>;
    Set_from_Seq *array;

    int capacity; // tamaño del array
    double size; // cantidad de elemetos incluyendo colisionados

public:
    HashTable() : capacity(13), size(0){
        array = new Set_from_Seq[capacity]; 
    }

    HashTable(int _capacity) : capacity(_capacity), size(0){
        array = new Set_from_Seq[capacity];
    }

    void insert(TK key, TV value){
        if (fillFactor()>= maxFillFactor) {
            cout<<"Tamanio actual: "<< _size()<<" Rehashing ..."<<endl; 
            resize();
        }

        Entry entry = {key,value}; //entrada de 2 valores (key and value)
        int index = _hash(key);

        auto it0 = begin(array[index]);
        while (it0 != end(array[index])){
            if ((*it0).key == key){
                (*it0).value = value;
                return;
            }
            ++it0;
        }

        if(it0 == end(array[index])){
            array[index].push_front(entry);
            size+=1;
        }
    };

    TV find(TK key){        
        int index = _hash(key);
        auto it0 = array[index];
        auto it1 = begin(it0);
        
        while (it1 != end(it0)) {
            if ((*it1).key == key) {
                return (*it1).value;
            } else{ ++it1;}
        }

        throw runtime_error("There is no entry with the key you are searching for");
    };



    void _delete(TK key){
        size_t index = _hash(key);
        auto& it0 = array[index];
        auto it1 = it0.begin();
        auto prevIt1 = it0.before_begin();
        // Encuentra el elemento a borrar y su posición
        while (it1 != end(it0) && (*it1).key != key) {
            ++it1;
            ++prevIt1;
        }
        // Verifica si el elemento existe
        if (it1 == end(it0)) {
            throw ("La clave que intentas eliminar no existe");
        }
        // Borra el elemento de la lista
        it0.erase_after(prevIt1);
    };
    
    // sobrecarga de operador []
    TV& operator[](TK key){
        int index = _hash(key);
        auto it0 = array[index];
        auto it1 = begin(it0);
        
        while (it1 != end(it0)) {
            if ((*it1).key == key) {
                return (*it1).value;
            } else{ ++it1;}
        }

        throw ("there is no an entry with the key you are searching for");
    };

    int _size(){
        return capacity;
    }

    int bucket_size(int iList){
        return distance(begin(array[iList]), end(array[iList]));
    }; //Cantidad de elementos por lista

    auto _begin(int iList){
        return begin(array[iList]);
    };

    auto _end(int iList){
        return end(array[iList]);
    };

private:
    size_t _hash(TK key){
        hash<TK> ptr_hash;
        return ptr_hash(key) % capacity;
    }
    
    // FillFactor = # of elements / (capacity * k) --> size(# of elements) ; maxColision (k)
        double fillFactor(){
            return size/(capacity*maxColision);
        }

    void resize(){
        int prev_capacity = capacity;
        capacity = capacity*2;
        Set_from_Seq* prev_array = array;
        array = new Set_from_Seq[capacity];
        for(int i=0; i<prev_capacity; ++i){
            auto it0 = prev_array[i];
            auto it1= begin(it0);
            while (it1 != end(it0)) {
                insert((*it1).key, (*it1).value);
                ++it1;
            }
        } delete [] prev_array;
    }
};