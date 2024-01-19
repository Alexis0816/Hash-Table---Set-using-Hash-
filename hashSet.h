#include <iostream>
#include <vector>
#include <forward_list>
#include <random>
#include <algorithm>
#include <cassert>

/*Integrantes:
- Daniel Casquino Paz
- Gabriel Blanco Gutierrez
- Alexis Raza Estrada*/

// Implementacion en C++ del HashSet de Python (y las operaciones del Set del ppt)
class HashSet
{
private:
    std::random_device rd; // Genera ints ranom para el int a
    int r;                 // Fill ratio
    unsigned long long sz = 0;
    uint32_t p = (1u << 31) - 1; // ´Numero primo usado para la hash function (2^31) - 1
    int a;                       // Constante arbitraria para cada hashSet
    int upper, lower;            // Limites para el size al momento de hacer resize al insertar
    mutable std::vector<std::forward_list<int>> buckets;
    // Buckets, vector de linked lists en vez de array para poder crear sets sin elementos

    int Hash(int key, int m) const
    {
        return ((a * key) % p) % m;
    } // Funcion hash

    void ComputeBounds()
    {
        upper = buckets.size();
        lower = buckets.size() * 100 * 100 / (r * r);
    } // Recalcula los limites del size en relacion a la cantidad elementos

    void Resize(int n)
    {
        if (lower >= n || n >= upper)
        {
            int fillFactor = r / 100;
            if (r % 100)
            {
                ++fillFactor;
            }
            int m = n > 1 ? n * fillFactor : fillFactor;

            std::vector<std::forward_list<int>> newBuckets(m);
            for (const auto &bucket : buckets)
            {
                for (const auto &key : bucket)
                {
                    int h = Hash(key, m);
                    newBuckets[h].push_front(key);
                }
            }

            buckets = std::move(newBuckets);
            ComputeBounds();
        }
    } // Cambia la cantidad de buckets de acuerdo a la cantidad de elementos para cumplir con el fillRatio

public:
    HashSet(int r = 200) : r(r) // El fill ratio es 0.5 por default (100/200)
    // Los buckets se crean cuando la cantidad de elementos se pasa del fillRatio * la cantidad de buckets
    {
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, p - 1);
        a = distrib(gen);
        ComputeBounds();
    } // Constructor

    HashSet(HashSet &&other) noexcept
        : r(std::move(other.r)), sz(std::move(other.sz)),
          p(std::move(other.p)), a(std::move(other.a)),
          upper(std::move(other.upper)), lower(std::move(other.lower)),
          buckets(std::move(other.buckets))
    {
        other.sz = 0;
    } // Move constructor

    HashSet(const HashSet &other)
        : r(other.r), sz(other.sz), p(other.p), a(other.a), upper(other.upper), lower(other.lower), buckets(other.buckets)
    {
    } // Copy constructor

    HashSet &operator=(const HashSet &other)
    {
        if (this != &other) // Avoid self-assignment
        {
            r = other.r;
            sz = other.sz;
            p = other.p;
            a = other.a;
            upper = other.upper;
            lower = other.lower;
            buckets = other.buckets;
        }
        return *this;
    } //  Copy assignment

    HashSet &operator=(HashSet &&other) noexcept
    {
        if (this != &other)
        {
            r = std::move(other.r);
            sz = std::move(other.sz);
            p = std::move(other.p);
            a = std::move(other.a);
            upper = std::move(other.upper);
            lower = std::move(other.lower);
            buckets = std::move(other.buckets);
            other.sz = 0;
        }
        return *this;
    } //  Move assignment

    ~HashSet() {} // Destructor chiquito

    int Size() { return sz; } // Devuelve la cantidad de elementos (no la cantidad de buckets)

    bool Find(const int &key) const
    {
        int h = Hash(key, buckets.size());
        const auto &bucket = buckets[h];
        return std::find(bucket.begin(), bucket.end(), key) != bucket.end();
    } // True si encuentra el valor, false si no

    bool Insert(int key)
    {
        Resize(sz + 1);
        int h = Hash(key, buckets.size());

        if (!Find(key))
        {
            buckets[h].push_front(key);
            ++sz;
            return true;
        }
        return false;
    } // Inserta un valor, devuelve true si se logro insertar, false de lo contrario

    bool Remove(int key)
    {
        assert(sz > 0);
        int h = Hash(key, buckets.size());
        if (Find(key))
        {
            buckets[h].remove(key);
            --sz;
            Resize(sz);
            return true;
        }
        return false;
    } // Elimina un valor y devuelve true si se pudo, false si no

    void Display()
    {
        int idx{};
        for (const auto &bucket : buckets)
        {
            std::cout << "Bucket " << idx << ": ";
            for (const int ls : bucket)
            {
                std::cout << ls << ", ";
            }
            ++idx;
            std::cout << std::endl;
        }
    } // Imprime todos los buckets(y elementos) de un hashSet

    HashSet Union(const HashSet &other) const
    {
        HashSet result = *this; // Copy the current hash set
        for (const auto &bucket : buckets)
        {
            for (auto &element : bucket)
            {
                result.Insert(element);
            }
        }
        return result;
    } // Devuelve un la union en un rvalue

    HashSet Intersect(const HashSet &other) const
    {
        HashSet result;
        for (const auto &bucket : buckets)
        {
            for (auto &element : bucket)
            {
                if (other.Find(element))
                {
                    result.Insert(element);
                }
            }
        }
        return result;
    } // Devuelve la interseccion en un rvalue

    HashSet Difference(const HashSet &other) const
    {
        HashSet result = *this; // Copy the current hash set
        for (const auto &bucket : buckets)
        {
            for (auto &element : bucket)
            {
                if (other.Find(element))
                {
                    result.Remove(element);
                }
            }
        }
        return result;
    } // Devuelve la diferencia en un rvalue

    // Iteradores bonitos (me estaba complicando demasiado)
    auto begin()
    {
        return buckets.begin();
    }

    auto end()
    {
        return buckets.end();
    }
};

// me duele la mano