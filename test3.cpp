#include <cmath>
#include <atcoder/fenwicktree>
#include <atcoder/lazysegtree>
#include <atcoder/segtree>

#include <string>
#include <vector>
#include <iostream>
#ifdef USE_PYTHON_LIKE_PRINT
#include "print.h"
#else
template<char SEP = ' ', char END = '\n'> inline void print(...){}
template<char SEP = ' ', char END = '\n'> inline void printe(...){}
template<char SEP = ' ', char END = '\n'> inline void printo(...){}
#define define_print(...)
#define define_print_with_names(...)
#define define_to_tuple(...)
#endif

typedef int S;
typedef int F;
S op(S l, S r) { return l + r; }
S e() { return 0; }
S mapping(F l, S r) { return l + r; }
F composition(F l, F r) { return l + r; }
F id() { return 0; }

int op2(int a, int b) { return std::max(a, b); }
int e2() { return -1; }

int main(void){
	atcoder::fenwick_tree<int> ft(10);
//	print(ft.size());
	
	ft.add(3, 9);
//	print(ft.size());
	ft.add(4, 2);
//	print(ft.size());
	
	print(ft.sum(0, 10));
	
//	print(ft.size());
//	print(ft.to_vector());
	print(ft);
	
	atcoder::lazy_segtree<S, op, e, F, mapping, composition, id> lst(10);
	
	lst.apply(3, 9, 5);
	lst.apply(4, 7, 3);
	lst.set(7, 2);
	
	print(lst);
	print(lst.prod(0, 10));
	
	atcoder::segtree<S, op2, e2> st(10);
	st.set(4, 5);
	st.set(3, 8);
	st.set(8, 2);
	print(st);
	print(st.all_prod());
	
	return 0;
}


