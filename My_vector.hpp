#include <stdexcept> // для std::out_of_range 


// сам класс вектора(динамического массива)
template<class T>
class Myvector
{
private:
    T* data; // указатель на первый элемент
    int capacity; // объем
    int size; // размер
public:
    Myvector() : size(0), capacity(0),data(nullptr) {}
    Myvector(const Myvector& obj);
    void push_back(T value);
    ~Myvector() { delete[] data; }
    int length() const { return size; }
    int get_capacity() const { return capacity; }
    T& operator[](int i);
    Myvector& operator=(const Myvector& obj);
    void pop_back();
    T& at(int index);
};

// добавление в конец.
template<class T>
void Myvector<T>::push_back(T value)
{
    // проверяем нужно ли увеличивать объем?
    if(size >= capacity) 
    {
        // увеличиваем объем в два раза больше.Но если объем = 0 то делаем = 1. 
        int new_capacity = (capacity == 0) ? 1 : capacity * 2;

        T* new_data = new T[new_capacity];

        // копируем старые данные
        for (int i{};i < size; i++)
        {
            new_data[i] = data[i];
        }
        // удаляем старый кусок 
        delete[] data;
        // перенаправляем указатель
        data = new_data;
        // обновляем объем
        capacity = new_capacity;
    }
    // эта часть выполняется всегда
    data[size] = value;
    size++;
}

// оператор индексирования
template<class T>
T& Myvector<T>::operator[](int i)
{
    return data[i];
}

// удаление с конца
template<class T>
void Myvector<T>::pop_back()
{
    if (size == 0)
    {
        return;
    }
    size--;
}

// конструктор копирования
template<class T>
Myvector<T>::Myvector(const Myvector& obj)
{
    capacity = obj.capacity;
    size = obj.size;

    if (size > capacity)
    {
        capacity = size;
    }

    data = new T[capacity];

    for (int i{}; i < size; i++)
    {
        data[i] = obj.data[i];
    }
}

// оператор присваивания,который копирует
template<class T>
Myvector<T>& Myvector<T>::operator=(const Myvector& obj)
{
    if (this == &obj)
    {
        return *this;
    }
    delete[] data;
    size = obj.size;
    capacity = obj.capacity;
    data = new T[capacity];
    for (int i = 0; i < size; i++)
    {
        data[i] = obj.data[i];
    }
    return *this;
}

// безопасное обращение по индексу с проверкой.
template<class T>
T& Myvector<T>::at(int index)
{
    if (index >= 0 && index < size)
    {
        return data[index];
    }
    else
    {
        throw std::out_of_range("INDEX out of range");
    }
}