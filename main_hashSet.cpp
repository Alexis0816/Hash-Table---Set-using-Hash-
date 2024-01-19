#include <iostream>
#include "hashSet.h"

/*Integrantes:
- Daniel Casquino Paz
- Gabriel Blanco Gutierrez
- Alexis Raza Estrada*/

// Tests con el hashSet
int main()
{
    HashSet h1(200);
    HashSet h2(200);
    HashSet h3(200);

    for (int i = 0; i < 5; ++i)
    {
        h1.Insert(i);
    }

    for (int i = 4; i < 10; ++i)
    {
        h2.Insert(i);
    }
    std::cout << "H1:" << std::endl;
    h1.Display();
    std::cout << std::endl;

    std::cout << "H2:" << std::endl;
    h2.Display();
    std::cout << std::endl;

    h3 = h1.Intersect(h2);
    std::cout << "H3, Interseccion de H1 y H2:" << std::endl;
    h3.Display();
    std::cout << std::endl;

    h3.Remove(4);
    std::cout << "H3 vacio" << std::endl;
    h3.Display();
    std::cout << std::endl;
    return 0;
}
