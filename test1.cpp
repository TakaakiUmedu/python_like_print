#include <iostream>
#include <vector>
#include <map>
#include <list>


// �ȉ��̂悤�ɂ��� print.h �� #include ���Ă����ƁA
//
// c++ -O3 -std=c++17 -DUSE_PYTHON_LIKE_PRINT print-test.cpp
//
// �̂悤�ɁAUSE_PYTHON_LIKE_PRINT �� #define �����悤�ȃI�v�V������t���ăR���p�C������ƁAprint.*()���g���A
//
// c++ -O3 -std=c++17 print-test.cpp
//
// �̂悤�ɁA�t�����ɃR���p�C������ƁAprint.*()�͎����I�ɖ����������ƂɂȂ�B
// ���Ȃ킿�Aprint.h �����݂��Ȃ��ꍇ�ł��G���[�ɂȂ炸�Aprint.*() �ȂǁA
// print.h �Œ�`���ꂽ���̂̌Ăяo���̓R���p�C���̑O�̃v���v���Z�X���ɏ������B
//
// ���v���̃W���b�W���ŁA-DUSE_PYTHON_LIKE_PRINT �t���ŃR���p�C������邱�Ƃ͂Ȃ��̂ŁA
// �񓚂̏o�͂��Astd::cout << answer << std::endl;�ł���Ă����΁A���̂܂ܒ�o���Ă��G���[�ɂȂ�Ȃ��B
// �f�o�b�O�o�͂̂��߂̃p�t�H�[�}���X�ቺ�������Ĉ��S�B
//
// ����������
#include <string>
#include <vector>
#include <iostream>
#ifdef USE_PYTHON_LIKE_PRINT
#include "print.hpp"
#else
template<char SEP = ' ', char END = '\n'> inline void print(...){}
template<char SEP = ' ', char END = '\n'> inline void printe(...){}
template<char SEP = ' ', char END = '\n'> inline void printo(...){}
#define define_print(...)
#define define_print_with_names(...)
#define define_to_tuple(...)
#endif
// �������܂�

// x, y, z�����^
struct Pos{
	int x, y, z;
	
	// std::make_tuple(x, y, z)����Ato_tuple()���\�b�h�𐶂₷
	define_to_tuple(x, y, z);
	
	// { x, y, z } ���o�͂���Apython_like_print(std::ostream& out)���\�b�h�𐶂₷
//	define_print(x, y, z);
	
	// �����o�[�̖��O�t���� { x: x, y: y, z: z } ���o�͂���python_like_print(std::ostream& out)���\�b�h�𐶂₷
	define_print_with_names(x, y, z);
};

// ���O�ƔN��ƃ|�W�V�������X�g���������^
class Human{
	public:
		std::string name;
		long age;
		std::vector<Pos> positions;
		Human(const std::string& name, int age): name(name), age(age){
		}
		
		// std::make_tuple(name, age)��Ԃ��Ato_tuple()���\�b�h�𐶂₷
		define_to_tuple(name, age);
		
		// { age, name } ���o�͂���Apython_like_print(std::ostream& out)���\�b�h�𐶂₷
//		define_print(age, name);
		
		// �����o�[�̖��O�t���� { age: age, name: name, positions: [...] } ���o�͂���python_like_print(std::ostream& out)���\�b�h�𐶂₷
		define_print_with_names(age, name, positions);
};


// ����̃N���X�ł��Atemplate�������t���Ă��āAbegin()�Aend()�������Ă����OK
template<typename T = int> class range{
	class iterator{
		private:
			T i;
			T interval;
		public:
			iterator(T i, T interval): i(i), interval(interval){
			}
			inline iterator operator++(){
				i += interval;
				return *this;
			}
			inline const T operator*(){
				return i;
			}
			// for(auto i: range(10, 20, 3)){} �Ƃ��ɑΉ����邽�߂̌ӎU�L���I�[�o�[���[�h
			inline friend const T operator!=(const iterator& a, const iterator& b){
				if(a.interval > 0){
					return a.i < b.i;
				}else{
					return a.i > b.i;
				}
			}
	};
		const T b;
		const T e;
		const T interval;
	public:
		range(T e): b(0), e(e), interval(1){
		}
		range(T b, T e): b(b), e(e), interval(1){
		}
		range(T b, T e, T i): b(b), e(e), interval(i){
		}
		inline iterator begin() const{
			return iterator(b, interval);
		}
		inline iterator end() const{
			return iterator(e, interval);
		}
};

int main(void){
	python_like_print::print_char_as_int = true;
	
	std::vector<int> hoge = {1, 2, 3};
	std::list<std::string> fuga = {"x", "a", "z"};
	std::vector<std::vector<int>> moge = {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}};
	std::vector<char> y = {'&', '#', '%'};
	std::map<std::string, std::string> x1 = {{"aaa", "bbb"}, {"ccc", "ddd"}};
	std::map<int, int> x2 = {{10, 20}, {30, 40}};
	std::map<int, std::string> x3 = {{10, "20"}, {30, "40"}};
	std::map<std::string, int> x4 = {{"10", 20}, {"30", 40}};
	std::vector<int> w(0);
	auto z = std::make_tuple('_', 88, "Hello");
	
	auto zz = Human("hoge", 42);
	zz.positions.push_back({1, 2, 3});
	zz.positions.push_back({4, 5, 6});
	
	print(10);
	printe(20.f);
	print(3.14);
	printe(30l);
	print('x');
	
	
	printe(range<int>(15));
	print(range<int>(10, 20));
	printe(range<int>(10, -1, -1));
	print(range<long>(10, 20));
	
	printe(hoge);
	print(moge);
	printe(fuga);
	print(x1);
	printe(x2);
	print(x3);
	printe(x4);
	print(y);
	printe(zz);

// constexpr���g���Ă��邽�߁Atuple�ւ̑Ή���C++17�̏ꍇ�̂݁B�R���p�C������-std=c++17���ƕt����B
// -std=c++17��t�����ɃR���p�C������ꍇ�́Atuple���܂񂾂��̂�print()�͂ł��Ȃ��B
#ifdef USE_PYTHON_LIKE_PRINT
	print(std::make_tuple('t', 20, 20l, 10.0f));
	printe(z);
	print(zz.to_tuple());
#endif
	print(w);
	
	Pos a = {10, 20, 30};
	Pos b = {12, 524, 97};
	std::map<std::string, Pos> p = {{"first", a}, {"second", b}};
	printe(p);
	
	print(x1, x2, x3, y, z);
	printe<'/'>(x1, x2, x3, y, z);
	printe(x1, x2, x3, y, z);
	printe<'-'>(x1, x2, x3, y, z);
	
	printe<'#', '-'>(x1, x2, x3, y, z);
	print<'?', '\r'>(x1, x2, x3, y, z);
	printe("===");
	print();
	printe();
	
	print(Human("hoge", 1024));
	return 0;
}