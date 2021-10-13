// «‚±‚±‚©‚ç
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
// ª‚±‚±‚Ü‚Å

#include <tuple>

struct hoge{
	int x, y, z;
	public:
	define_to_tuple(x, y, z);
};

int main(void){
	const hoge x = {0, 1, 2};
	print(x.to_tuple());
	return 0;
}