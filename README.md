# My_vector
My academic realisation vector!

#Ru:

# УЧЕБНАЯ реализация вектора(динамического массива)
Данная реализация была учебной.Для глубокого понимания как работает память в C++.И для понимания в целом как работает дин.массив изнутри.

# МЕТОДЫ
push_back() - добавляет элемент в конец массива

length() - возращает количество имеющихся объектов

operator[] - оператор индексации,для того что бы обратится выборочно к элементу по индексу

pop_back() - удаляет элемент с конца

at() - более безопасное обращаение по индексу,так как учитывает исключения

emplace_back() - принимает в качестве аргумента,все аргументы конструктора класса,и рождает объект основываясь на них.

# Особенности 
Есть динамического расширение (X2 коэффицент)

Поддержка шаблонов

также хорошая формула для расчета объема в capacity  : capacity == 0 ? 1 : capacity * 2

Итератор(практически со всеми операциями нужными для него)

Разделение выделение памяти и рождения объекта.


# Eng:

# Academic realization vector(dynamic array)
This realization was academic.For order to form deep knowledge how work with memory in C++.And for order to form knowledge in general how work's dynamic array.

# methods

push_back() - add element in end array

length() - return size(live object) array

operator[] - operator index,for order to call index element

pop_back() - delete element in end array

at() - more safe method,for order to call index element.Why more safe?Cuz consider exception.

emplace_back() - method,which accept arguments constructor and placement object,in vector.

# Features

have dynamic expension(X2 coff)

Support template

Good formula for calculation capacity array

Iterator

separation memory alloc and born object


