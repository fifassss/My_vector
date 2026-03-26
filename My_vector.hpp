#include <stdexcept> // для std::out_of_range 
#include <utility>

template<typename Myvector>
class VectorIterator
{
public:
    using ValueType = typename Myvector::ValueType;
    using PointerType = ValueType*;
    using ReferenceType = ValueType&;
private:
    PointerType m_ptr;
public:
    VectorIterator(PointerType _ptr) : m_ptr(_ptr) {}

    VectorIterator& operator++() noexcept // префикс
    {
        m_ptr++;
        return *this;
    }
    VectorIterator operator++(int) noexcept // постфиксный
    {
        VectorIterator iterator = *this;
        ++(*this);
        return iterator;
    }
    VectorIterator& operator--() noexcept // префикс
    {
        m_ptr--;
        return *this;
    }
    VectorIterator operator--(int) noexcept // постфиксный
    {
        VectorIterator iterator = *this;
        
        --(*this);
        return iterator;
    }
    ReferenceType operator[](int index) noexcept
    {
        return *(m_ptr + index);
    }
    PointerType operator->() noexcept
    {
        return m_ptr;
    }
    ReferenceType operator*() noexcept
    {
        return *m_ptr;
    }
    bool operator==(const VectorIterator& obj) const noexcept
    {
        return m_ptr == obj.m_ptr;
    }
    bool operator!=(const VectorIterator& obj) const noexcept
    {
        return !(*this == obj);
    }
};

// сам класс вектора(динамического массива)
template<class T>
class Myvector
{
private:
    T* data; // указатель на первый элемент
    int capacity; // объем
    int size; // размер
    void reallocate(int new_capacity); // функция на выделение сырой памяти.
public:
    using ValueType = T;
    using Iterator = VectorIterator<Myvector<T>>;
    Myvector() : size(0), capacity(0), data(nullptr) {}
    Myvector(const Myvector& obj);
    Myvector(Myvector&& obj) noexcept;
    void push_back(const T& value);
    void push_back(T&& value);
    template<class...Args> // пачка любых типов
    void emplace_back(Args&&...args); // принимает пачку аргументов
    ~Myvector() noexcept
    { 
        for (int i = 0; i < size; i++)
        {
            data[i].~T(); // уничтожаем каждый объект 
        }
        ::operator delete(data); // освобождаем память
    }
    int length() const { return size; }
    int get_capacity() const { return capacity; }
    T& operator[](int i);
    Myvector& operator=(const Myvector& obj);
    Myvector& operator=(Myvector&& obj) noexcept;
    void pop_back();
    Iterator begin() const
    {
        return Iterator(data);
    }
    Iterator end() const
    {
        return Iterator(data + size);
    }
    T& at(int index);
};

template<class T>
Myvector<T>& Myvector<T>::operator=(Myvector&& obj) noexcept
{
    if (this == &obj)
    {
        return *this;
    }

    for (int i = 0; i < size; i++)
    {
        data[i].~T(); // уничтожаем каждый объект 
    }
    ::operator delete(data); // освобождаем память

    size = obj.size;
    capacity = obj.capacity;
    data = obj.data;

    obj.data = nullptr;
    obj.size = 0;
    obj.capacity = 0;

    return *this;
}


template<class T>
Myvector<T>::Myvector(Myvector&& obj) noexcept
{
    data = obj.data;
    size = obj.size;
    capacity = obj.capacity;

    obj.data = nullptr;
    obj.size = 0;
    obj.capacity = 0;
}


template<class T>
template<class...Args> // пачка любых типов
void Myvector<T>::emplace_back(Args&&...args) // принимает пачку аргументов
{
    // проверяем нужно ли увеличивать объем?
    if (size >= capacity)
    {
        // увеличиваем объем в два раза больше.Но если объем = 0 то делаем = 1. 
        reallocate(capacity == 0 ? 1 : capacity * 2);
    }

    // мы передаем значения прямо в конструктор
    // std::forward прокидывает их 'как есть' - сохраняя типы ссылок
    new (data + size) T(std::forward<Args>(args)...);
    size++;
}

template<class T>
void Myvector<T>::reallocate(int new_capacity) // опущена безопасность исключений для более простой реализации алгоритма и понятной
{
    // выделяют СЫРУЮ ПАМЯТЬ(читать оттуда нельзя,пока мы туда что-то в память не положим что-то)
    T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

    // копируем старые данные
    for (int i{}; i < size; i++)
    {
        // используем конструктор перемещения что бы создать объект в новой памяти
        new (new_data + i) T(std::move_if_noexcept(data[i])); // std::move_if_noexcept делает перемещение если видит что у типа T перемещение подписано с ключевым словом noexcept.Для std::move_if_noexcept нужно -> <utility>

        // уничтожение старого объекта на прежнем месте
        data[i].~T();
    }
    // удаляем старый кусок(СЫРОЙ) 
    ::operator delete(data);
    // перенаправляем указатель
    data = new_data;
    // обновляем объем
    capacity = new_capacity;
}

template<class T>
void Myvector<T>::push_back(T&& value)
{
    // проверяем нужно ли увеличивать объем?
    if (size >= capacity)
    {
        // увеличиваем объем в два раза больше.Но если объем = 0 то делаем = 1. 
        reallocate(capacity == 0 ? 1 : capacity * 2);
    }
    // эта часть выполняется всегда
    // СОЗДАЕМ объект в сырой памяти.
    new(data + size) T(std::move(value)); // placement new синтаксис - new(адрес) T *тип* (значение)
    size++;
}

// добавление в конец.
template<class T>
void Myvector<T>::push_back(const T& value)
{
    // проверяем нужно ли увеличивать объем?
    if (size >= capacity)
    {
        // увеличиваем объем в два раза больше.Но если объем = 0 то делаем = 1. 
        reallocate(capacity == 0 ? 1 : capacity * 2);
    }
    // эта часть выполняется всегда
    // СОЗДАЕМ объект в сырой памяти.
    new(data + size) T(value); // placement new синтаксис - new(адрес) T *тип* (значение)
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
    if (size > 0)
    {
        size--;
        data[size].~T();
    }
    
}

// конструктор копирования
template<class T>
Myvector<T>::Myvector(const Myvector& obj)
{
    capacity = obj.capacity;
    size = obj.size;

    // выделяем сырую память
    data = static_cast<T*>(::operator new(capacity * sizeof(T)));

    // копируем через placement new
    for (int i{}; i < size; i++)
    {
        new (data + i) T(obj.data[i]);
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
    for (int i = 0; i < size; i++)
    {
        data[i].~T(); // уничтожаем каждый объект 
    }
    ::operator delete(data); // освобождаем память
    size = obj.size;
    capacity = obj.capacity;
    data = static_cast<T*>(::operator new(capacity * sizeof(T)));
    for (int i = 0; i < size; i++)
    {
        new (data + i) T(obj.data[i]);
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



// UPD
/*
    для чего я делал placement new и разделял выделение памяти(для хранение объектов) и размещение объекта в памяти.
    потому что например при new = Someclass[100]; // вызывает конструкторы по умолчанию у класса Someclass.То есть простой оператор new выделяет память и вызывает конструкторы по умолчанию.100 все раз.
    если у Someclass нету конструкторов по умолчанию,вектор не будет компилироваться,при учете того что используется обычный new.
    если я выделяю память под 100 элементов но кладу один я все ровно заплатил бы(если бы был простой new) за создание 100 пустых объектов.А потом когда я делаю push_back,я бы их перезаписовал,двойна работа была бы проще говоря.
    также если мы используем ::operator new(кол-во например объектов * sizeof(тип объекта)) (ТАКЖЕ НУЖНО приводить эту память сырую к тому типу который нужен,ведь new возвращает void* поэтому нужно приводить кастами к типу) то и удалять надо через ::operator delete(указатель);
    перемещение объекта так вообще пришлось делать также через placement new и перемещение.И плюсом еще в цикле вызывать в старом блоке памяти у каждого отдельного объекта свой деструктор вручную.
    for (int i = 0; i < size; i++)
    {
        data[i].~T(); // уничтожаем каждый объект
    }
    ::operator delete(data); // освобождаем память

    это нужно для более сложных типов.
    вызывая для каждого дестркутор отдельно.А после удаляя сам массив.
    плюсом обычный delete[] указатель; // 1 - смотрит сколько элементов было в массив.После вызывает деструктор для всех этих элементов.Это бы сработало хорошо если бы мы хранили только простые типы.Но есть же и сложные такие как строки и тд.У них(строк и прочих классов) уже свои ресурсы.Также смешивался бы деструктор(который удалял бы через delete[] указатель; А конструктор или выделение памяти было бы через ::operator new глобальный) 
*/
