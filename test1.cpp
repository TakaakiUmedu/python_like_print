#include <iostream>
#include <vector>
#include <map>
#include <list>


// 以下のようにして print.h を #include しておくと、
//
// c++ -O3 -std=c++17 -DUSE_PYTHON_LIKE_PRINT print-test.cpp
//
// のように、USE_PYTHON_LIKE_PRINT が #define されるようなオプションを付けてコンパイルすると、print.*()が使われ、
//
// c++ -O3 -std=c++17 print-test.cpp
//
// のように、付けずにコンパイルすると、print.*()は自動的に無かったことになる。
// すなわち、print.h が存在しない場合でもエラーにならず、print.*() など、
// print.h で定義されたものの呼び出しはコンパイルの前のプリプロセス時に消される。
//
// 競プロのジャッジ環境で、-DUSE_PYTHON_LIKE_PRINT 付きでコンパイルされることはないので、
// 回答の出力を、std::cout << answer << std::endl;でやっておけば、そのまま提出してもエラーにならない。
// デバッグ出力のためのパフォーマンス低下も無くて安心。
//
// ↓ここから
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
// ↑ここまで

// x, y, zを持つ型
struct Pos{
	int x, y, z;
	
	// std::make_tuple(x, y, z)する、to_tuple()メソッドを生やす
	define_to_tuple(x, y, z);
	
	// { x, y, z } を出力する、python_like_print(std::ostream& out)メソッドを生やす
//	define_print(x, y, z);
	
	// メンバーの名前付きの { x: x, y: y, z: z } を出力するpython_like_print(std::ostream& out)メソッドを生やす
	define_print_with_names(x, y, z);
};

// 名前と年齢とポジションリストを持った型
class Human{
	public:
		std::string name;
		long age;
		std::vector<Pos> positions;
		Human(const std::string& name, int age): name(name), age(age){
		}
		
		// std::make_tuple(name, age)を返す、to_tuple()メソッドを生やす
		define_to_tuple(name, age);
		
		// { age, name } を出力する、python_like_print(std::ostream& out)メソッドを生やす
//		define_print(age, name);
		
		// メンバーの名前付きの { age: age, name: name, positions: [...] } を出力するpython_like_print(std::ostream& out)メソッドを生やす
		define_print_with_names(age, name, positions);
};


// 自作のクラスでも、template引数が付いていて、begin()、end()が生えていればOK
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
			// for(auto i: range(10, 20, 3)){} とかに対応するための胡散臭いオーバーロード
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

// constexprを使っているため、tupleへの対応はC++17の場合のみ。コンパイル時に-std=c++17等と付ける。
// -std=c++17を付けずにコンパイルする場合は、tupleを含んだもののprint()はできない。
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