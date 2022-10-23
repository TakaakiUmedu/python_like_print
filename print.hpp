// https://github.com/TakaakiUmedu/python_like_print

#ifndef PYTHON_LIKE_PRINT
#define PYTHON_LIKE_PRINT

/*** ��������include�̎d��

// ����������
//#include <atcoder/segtree>
//#include <atcoder/lazysegtree>
//#include <atcoder/fenwicktree>
//#include <atcoder/modint>
//#include <atcoder/dsu>

#include <string>
#include <vector>
#include <iostream>
#include <tuple>
#ifdef USE_PYTHON_LIKE_PRINT
#include "print.hpp"
#else
template<char SEP = ' ', char END = '\n', class... A> inline void print(A&& ...){}
template<char SEP = ' ', char END = '\n', class... A> inline void printe(A&& ...){}
template<char SEP = ' ', char END = '\n', class... A> inline void printo(A&& ...){}
#define define_print(...)
#define define_print_with_names(...)
#define define_to_tuple(...)
#endif
// �������܂�

 ***/

#include <algorithm>

// Python���ɕW���o�͂ɏo�͂���B
// SEP�AEND��t����Ƃ��̂悤�ɏo�͂����B
// SEP��''���w�肷��ƃG���[�ɂȂ�B�w�肵�����ꍇ�ɂ͑����print<'\0'>()�Ƃ���B
// END��t�������ꍇ�ASEP�͏ȗ��ł��Ȃ��B
template<char SEP = ' ', char END = '\n', typename ...A> inline void print(const A& ...args);

// �������̂̕W���G���[�o�[�W����
template<char SEP = ' ', char END = '\n', typename ...A> inline void printe(const A& ...args);

// �������̂̏o�͐掩�R�o�[�W����
template<char SEP = ' ', char END = '\n', typename ...A> inline void printo(std::ostream& out, const A& ...args);

// class��struct�̒��g�� ostream& �ւ� << �ŏo�͂ł���悤�ɂ���B
// class��struct�� ** ��`�̒� ** �ŁA
//
//    define_print( �\���������������o���J���}��؂�ōD���Ȃ���, ...)
//
// �Ƃ���ƁAprint(���̃N���X�̃I�u�W�F�N�g); ���ł���悤�ɂȂ�(���̃I�u�W�F�N�g���܂�vector��map�������l)�B
//
//    define_print_with_names(�\���������������o���J���}��؂�ōD���Ȃ���, ...)
//
// �Ƃ���ƁA���ꂼ��u�����o��: �l�v�ƕ\������Ă�茩�₷���Ȃ�B
// �����֐����`���Ă���̂ŁA���������Ɏw�肷��ƃG���[�B
#define define_print(...) \
	inline void python_like_print(std::ostream& out) const{ \
		out << "{"; \
		python_like_print::print_members(out, __VA_ARGS__); \
		out << "}"; \
	}

#define define_print_with_names(...) \
	inline static const std::vector<std::string>& python_like_print_member_names(){ \
		static std::vector<std::string> member_names = python_like_split_and_strip::split_and_strip(#__VA_ARGS__, ','); \
		return member_names; \
	}\
	inline void python_like_print(std::ostream& out) const{ \
		out << "{"; \
		python_like_print::print_members_with_names<0>(out, python_like_print_member_names(), __VA_ARGS__); \
		out << "}"; \
	}


// �����悤�ȃm���ŁA�w�肵�������o�[��tuple�ɕϊ�����֐����`����
#define define_to_tuple(...) \
	inline decltype(std::make_tuple(__VA_ARGS__)) to_tuple() const{\
		return (decltype(std::make_tuple(__VA_ARGS__)))(*this);\
	}\
	inline operator decltype(std::make_tuple(__VA_ARGS__))() const{\
		return std::make_tuple(__VA_ARGS__);\
	}


// �錾��Z�߂�
namespace python_like_print{
	using std::string, std::basic_ostream, std::ostream, std::cout, std::endl;
	using std::pair, std::tuple, std::vector, std::tuple_size, std::get;
	using std::declval, std::enable_if_t, std::is_same_v, std::remove_reference_t, std::is_lvalue_reference_v, std::true_type, std::false_type;
	static bool print_char_as_int = false;
	
	// T i��++i�ł��āA*i�ł���ꍇ�ɁA*i��Ԃ����ƂŁAiterator�Ȃ炻�̎w���^���`�F�b�N���邽�߂̊֐�
	template<typename T> inline auto iterchecker(T iter){ return *++iter; }
	
	// iterator�̗ނ̂悤�ɁAbegin()��end()���t���Ă���ꍇ�ɁA*begin()�̌^���擾���邽�߂̃N���X
	template<typename T,
	         typename B = decltype(declval<T>().begin()),
	         typename E = decltype(declval<T>().end()),
	         typename C = decltype(static_cast<bool>(declval<B>() != declval<E>())),
             typename V = decltype(iterchecker(declval<B>()))>
	struct iterator_return_type{
		using type = enable_if_t<is_same_v<C, bool>, V>;
	};
	
	// pair���ǂ�������
	template<typename T> struct is_pair: false_type{};
	template<typename F, typename S> struct is_pair<pair<F, S>>: true_type{};
	
	                                          inline void print_item(ostream& out, const string& value);
	                                          inline void print_item(ostream& out, const char* value);
	                                          inline void print_item(ostream& out, char value);
	template<typename First, typename Second> inline void print_item(ostream& out, const pair<First, Second>& pair);
	template<typename ...T>                   inline void print_item(ostream& out, const tuple<T...>& tuple);
	                                          inline void print_item(ostream& out, bool value);
	template<typename T, typename V = typename iterator_return_type<T>::type>
	                                          inline void print_item(ostream& out, const T& t);

	template<typename T, typename X = decltype(declval<T>().python_like_print(cout))>
	                                          inline ostream& operator<<(ostream& out, const T& value);
	
	template<typename T, typename S = decltype(cout << declval<T>()), typename R = enable_if_t<is_same_v<remove_reference_t<S>, decltype(cout)> && is_lvalue_reference_v<S>>>
	                                          inline void print_item(ostream& out, const T& value);
	
	template<typename IteratorB, typename IteratorE> inline void print_all(ostream& out, IteratorB begin, IteratorE end);
	template<size_t N = 0, typename ...T> inline void print_items_in_tuple(ostream &out, const tuple<T...>& tuple);
	template<size_t N, typename T>                inline void print_members_with_names(ostream& out, const vector<string>& names, const T& arg);
	template<size_t N, typename F, typename ...T> inline void print_members_with_names(ostream& out, const vector<string>& names, const F& arg, T ...args);
	
	template<char SEP, char END>                            inline void print_to_stream(ostream& out);
	template<char SEP, char END, typename T, typename ...A> inline void print_to_stream(ostream& out, const T& value, const A& ...args);
}

//int x = std::cout;
//int z = std::enable_if_t<false, void>();
//int y = std::enable_if_t<std::is_same_v<std::remove_reference_t<decltype(std::cout << 1)>, decltype(std::cout)>, void>();

// �W���o�͂ɏo�͂���֐��̎���
template<char SEP, char END, typename ...A> inline void print(const A& ...args){
	printo<SEP, END, A...>(std::cout, args...);
}

// �������̂̕W���G���[�o�[�W����
template<char SEP, char END, typename ...A> inline void printe(const A& ...args){
	printo<SEP, END, A...>(std::cerr, args...);
}

// �������̂̏o�͐掩�R�o�[�W����
template<char SEP, char END, typename ...A> inline void printo(std::ostream& out, const A& ...args){
	python_like_print::print_to_stream<SEP, END, A...>(out, args...);
}

// AtCoder���C�u�����̃N���X��get(index)���\�b�h��Ato_vector()���\�b�h�𐶂₵�Aprint()�ŕ\���ł���悤�ɂ��邽�߂̊g���E�u�������p�N���X
namespace python_like_print{
	// get(index)�𐶂₷
	template<class S, class C, S _get(C&, int)> class gettable_collection: public C{
		public:
			gettable_collection(): C(){
			}
			gettable_collection(int size): C(size){
			}
			S get(int index){
				return _get(*this, index);
			}
	};
	
	// to_vector()�̂��߁Asize()�𐶂₷(_n��private�ŊO����A�N�Z�X�o���Ȃ�����)
	template<class S, class C> class vectorizable_collection: public C{
		private:
			int _size;
		public:
			vectorizable_collection(): C(), _size(0){
			}
			vectorizable_collection(int size): C(size), _size(size){
			}
			inline vector<S> to_vector() const{
				// AtCoder���C�u�����̒l���擾���郁�\�b�h����const�ȏꍇ������̂ŁA�R�s�[������Ă���A�N�Z�X����K�v������(�����)
				C collection_tmp(*this);
				vector<S> vec(_size);
				for(int i = 0; i < _size; i ++){
					vec[i] = collection_tmp.get(i);
				}
				return vec;
			}
			template<class T2, class C2> friend ostream& operator<<(ostream& out, vectorizable_collection<T2, C2> value);
	};
	template<class S, class C> inline ostream& operator<<(ostream& out, vectorizable_collection<S, C> value){
		print_item(out, value.to_vector());
		return out;
	}
}

// AtCoder���C�u������Fenwick Tree�ALazy Segment Treee�ASegment Treee��print()�ŕ\���ł���悤�ɂ���
// ���̂܂܂ł͖����Ȃ̂ŁAvectorizable�ȃo�[�W�������`���āAdefine�Œu�������Ă��܂�
namespace atcoder{
#ifdef ATCODER_FENWICKTREE_HPP
	template<class S> S get_fenwick_tree_value(fenwick_tree<S>& ft, int index){
		return ft.sum(index, index + 1);
	}
	template<class S> using fenwick_tree_vectorizable = python_like_print::vectorizable_collection<S, python_like_print::gettable_collection<S, fenwick_tree<S>, get_fenwick_tree_value<S>>>;
#define fenwick_tree fenwick_tree_vectorizable
#endif
#ifdef ATCODER_LAZYSEGTREE_HPP
	template <class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
		using lazy_segtree_vectorizable = python_like_print::vectorizable_collection<S, lazy_segtree<S, op, e, F, mapping, composition, id>>;
#define lazy_segtree lazy_segtree_vectorizable
#endif
#ifdef ATCODER_SEGTREE_HPP
	template <class S, S (*op)(S, S), S (*e)()> using segtree_vectorizable = python_like_print::vectorizable_collection<S, segtree<S, op, e>>;
#define segtree segtree_vectorizable
#endif
}
// ModInt�₻��vector�Ȃǂ�print()�ł���悤�ɂ���B
namespace python_like_print{
#ifdef ATCODER_MODINT_HPP
	template<int m> inline ostream& operator<<(ostream& out, const atcoder::static_modint<m>& value){
		return out << value.val();
	}
	template<int id> inline ostream& operator<<(ostream& out, const atcoder::dynamic_modint<id>& value){
		return out << value.val();
	}
#endif
#ifdef ATCODER_DSU_HPP
	inline ostream& operator<<(ostream& out, const atcoder::dsu& value){
		print_item(out, atcoder::dsu(value).groups());
		return out;
	}
#endif
}


// ����
namespace python_like_print{
	// �e��̒l������炵���o��
	
	// �������""�ň͂�
	inline void print_item(ostream& out, const string& value){
		out << '"' << value << '"';
	}
	inline void print_item(ostream& out, const char* value){
		out << '"' << value << '"';
	}
	
	// char��''�ň͂ށB���邢�͐����l�Ƃ��ďo��
	inline void print_item(ostream& out, char value){
		if(print_char_as_int){
			out << static_cast<int>(value);
		}else{
			out << '\'' << value << '\'';
		}
	}
	
	// pair�́A:�ŋ�؂���
	template<typename First, typename Second> inline void print_item(ostream& out, const pair<First, Second>& pair){
		print_item(out, pair.first);
		out << ": ";
		print_item(out, pair.second);
	}

// tuple�̒��g���o��
// constexpr��C++17�ȍ~�łȂ��Ǝg���Ȃ��̂ŁA���ꖢ���̃o�[�W�����ŃR���p�C�����ꂽ�ꍇ�͒�`���΂��B
#if defined(__cplusplus) && __cplusplus >= 201703L
	// 2�ڈȍ~�̗v�f��", "��t���ďo��
	template<size_t N, typename ...T> inline void print_items_in_tuple(ostream &out, const tuple<T...>& item){
		if constexpr(N < tuple_size<tuple<T...>>::value){
			out << ", ";
			print_item(out, get<N>(item));
			print_items_in_tuple<N + 1, T...>(out, item);
		}
	}
	
	// tuple�͒��g��", "�ŋ�؂���'('�`')'�ň͂��ď��ɏo��
	template<typename ...T> inline void print_item(ostream &out, const tuple<T...>& item){
		out << '(';
		if constexpr(tuple_size<tuple<T...>>::value > 0){
			print_item(out, get<0>(item));
			print_items_in_tuple<1, T...>(out, item);
		}
		out << ')';
	}
#endif
	
	// bool��"true"/"false"���o��
	inline void print_item(ostream& out, bool value){
		out << (value ? "true" : "false");
	}
	
	// .begin()��.end()���t���Ă�����̂͏��ɕ\���B���g��pair�̏ꍇ��map�̗ނƌ��􂵂�{}�ŁA����ȊO��[]�ň͂�
	template<typename T, typename V>
	inline void print_item(ostream& out, const T& iterable){
		if constexpr(is_pair<V>::value){
			out << '{';
		}else{
			out << '[';
		}
		print_all(out, iterable.begin(), iterable.end());
		if constexpr(is_pair<V>::value){
			out << '}';
		}else{
			out << ']';
		}
	}
	
	// ���̑��A<< �ŏo�͂ł������(�ł��Ȃ����̂̓G���[)
	template<typename T, typename S, typename R>
	inline void print_item(ostream& out, const T& value){
		out << value;
	}
	
	// pytho_like_print(ostream& out); ���\�b�h�������Ă�����̂� out << item; �ŏo�͂ł���悤��
	template<typename T, typename X>inline ostream& operator<<(ostream& out, const T& value){
		value.python_like_print(out);
		return out;
	}
	
	// .begin()����.end()�܂ł�", "��؂�ŏo�͂���
	template<typename IteratorB, typename IteratorE> inline void print_all(ostream& out, IteratorB begin, IteratorE end){
		if(begin != end){
			auto i = begin;
			print_item(out, *i);
			for(++ i; i != end; ++ i){
				out << ", ";
				print_item(out, *i);
			}
		}
	}
	
	// ����1��1���΂炯������
	// �΂炯������c�肪�������END�����o��
	template<char SEP, char END> inline void print_to_stream(ostream& out){
		if(END != '\0'){
			out << END;
		}
	}

	// �����╶����print()�̒��ڂ̈����̏ꍇ�́A""��''��t�����ɕ\������
	inline void print_value(ostream& out, const char* s){ out << s; }
	inline void print_value(ostream& out, const string& s){ out << s; }
	inline void print_value(ostream& out, char c){ out << c; }
	// ����ȊO�́A���ʂɏo��
	template<typename T> inline void print_value(ostream& out, const T& v){ print_item(out, v); }
	
	// 2�ȏ�̈���������ꍇ��1�ڂ��o�͂���SEP���o�͌�A�c����o��
	template<char SEP, char END, typename T, typename ...A> inline void print_to_stream(ostream& out, const T& value, const A& ...args){
		print_value(out, value);
		if constexpr(sizeof...(A) > 0){
			if(SEP != '\0'){
				out << SEP;
			}
			print_to_stream<SEP, END, A...>(out, args...);
		}else{
			print_to_stream<SEP, END>(out);
		}
	}
	
	// class��struct�̒��g��\�����邽�߂̃w���p�֐�
	template<typename T> inline void print_members(ostream& out, const T& arg){
		print_item(out, arg);
	}
	
	template<typename F, typename ...T> inline void print_members(ostream& out, const F& arg, T ...args){
		print_item(out, arg);
		out << ", ";
		print_members(out, args...);
	}
	
	template<size_t N, typename T> inline void print_members_with_names(ostream& out, const vector<string>& names, const T& arg){
		out << names[N] << ": ";
		print_item(out, arg);
	}
	
	template<size_t N, typename F, typename ...T> inline void print_members_with_names(ostream& out, const vector<string>& names, const F& arg, T ...args){
		out << names[N] << ": ";
		print_item(out, arg);
		out << ", ";
		print_members_with_names<N + 1>(out, names, args...);
	}
	
}

// split��strip�̎���
//
// �\�[�X�t�@�C���𕪊��R���p�C������ꍇ�A
// - static vector<string> split(const string& str, const string& delim)
// - static string strip(const string& str)
// ��2�́A���̂܂܂��Ɗ֐����d�����ă����N�����̂������ƁA�o�C�i���T�C�Y�����邩��?
//
namespace python_like_split_and_strip{
	using namespace std;
	
	// split(������, ��؂�) ���ꂱ��
	                                 static vector<string> split(const string& str, const string& delim);
	template<typename S, typename T> inline vector<string> split(const S& str, const T& delim);
	template<typename S>             inline vector<string> split(const S& str, const char delim);
	
	// strip(������)
	static string strip(const string& str);
	
	// split���Ă��炻�ꂼ���strip����
	template<typename S, typename T> vector<string> split_strip(const S& str, const T& delim);
	

	// string�̑����char*��char���n����悤��
	template<typename S, typename T> vector<string> split(const S& str, const T& delim){
		return split(string(str), string(delim));
	}
	template<typename S> vector<string> split(const S& str, const char delim){
		return split(string(str), string(1, delim));
	}
	
	// split������ł��ꂼ���strip����
	template<typename S, typename T> vector<string> split_and_strip(const S& str, const T& delim){
		auto result = split(str, delim);
		transform(result.begin(), result.end(), result.begin(), strip);
		return result;
	}
	
	static vector<string> split(const string& str, const string& delim){
		vector<string> result;
		size_t start = 0;
		while(start < str.size()){
			auto index = str.find(delim, start);
			if(index == string::npos){
				break;
			}
			result.push_back(str.substr(start, index - start));
			start = index + delim.size();
		}
		result.push_back(str.substr(start));
		return result;
	}
	static string strip(const string& str){
		if(str.size() == 0){
			return str;
		}else{
			const auto start = str.find_first_not_of(" \t\r\n");
			if(start == string::npos){
				return string("");
			}else{
				const auto end = str.find_last_not_of(" \t\r\n");
				return str.substr(start, end - start + 1);
			}
		}
	}
}


#endif
