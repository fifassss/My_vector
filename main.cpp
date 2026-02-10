#include "My_vector.hpp" // сам класс моего вектора(дин.массива)
#include <cassert> // для тестов.
#include <iostream> // для cout

// функция для тестов.
void test_push_back()
{
	Myvector<int> v;
	v.push_back(1);
	v.push_back(2);
	assert(v.length() == 2);
	assert(v[0] == 1);
	assert(v[1] == 2);
	std::cout << "Test Push_back: OK" << '\n';
}

int main()
{
	test_push_back(); // тест для push_back
	
	// ниже тесты самого вектора,его операторов присванивание(копирующих),операторов индексации,метода length(),метода push_back(),и метода at() который безопасен что бы проходится по индексам вектора ведь,он может выкидывать исключения как я и показал ниже.
	Myvector<int> asd;
    asd.push_back(10);
    asd.push_back(20);
    asd.push_back(30);

    std::cout << "Element [0] = " << asd[0] << '\n';
    std::cout << "Length = " << asd.length() << '\n';

    Myvector<int> v = asd;
    v[0] = 999;
    std::cout << "Element v [0] = " << v[0] << '\n';
    std::cout << "Element asd[0] = " << asd[0] << '\n';
    try
    {
        v.at(100);
    }
    catch(const std::exception& e)
    {
        std::cout << "error " << e.what() << "\n";
    }
	
	return 0;
	

}
