#ifndef PYTHON_LIKE_PRINT
#define PYTHON_LIKE_PRINT

/*** ��������include�̎d��

// ����������
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
// �������܂�

 ***/

// Python���ɕW���o�͂ɏo�͂���B
// SEP�AEND��t����Ƃ��̂悤�ɏo�͂����B
// SEP��''���w�肷��ƃG���[�ɂȂ�B�w�肵�����ꍇ�ɂ͑����print<'\0'>()�Ƃ���B
// END��t�������ꍇ�ASEP�͏ȗ��ł��Ȃ��B
template<char SEP = ' ', char END = '\n', typename ...A> inline void print(A ...args);

// �������̂̕W���G���[�o�[�W����
template<char SEP = ' ', char END = '\n', typename ...A> inline void printe(A ...args);

// �������̂̏o�͐掩�R�o�[�W����
template<char SEP = ' ', char END = '\n', typename ...A> inline void printo(std::ostream& out, A ...args);

// class��struct�̒��g��\���ł���悤�ɂ���B
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
	inline static std::vector<std::string>& python_like_print_member_names(){ \
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
	using namespace std;
	                                          inline void print_complex_item(ostream& out, const string& value);
	                                          inline void print_complex_item(ostream& out, const char* value);
	                                          inline void print_complex_item(ostream& out, char value);
	template<typename First, typename Second> inline void print_complex_item(ostream& out, const pair<First, Second>& pair);
	template<typename ...T>                   inline void print_complex_item(ostream &out, const tuple<T...>& tuple);
	                                          inline void print_complex_item(ostream& out, bool value);
	template<template<typename, typename, typename...> class Map, typename Key, typename Value, typename ...Parameters, typename C = Map<Key, Value, Parameters...>, typename BeginFirstType = decltype(declval<C>().begin()->first), typename BeginSecondType = decltype(declval<C>().begin()->second), typename EndType = decltype(declval<C>().end())>
	                                          inline void print_complex_item(ostream& out, const Map<Key, Value, Parameters...>& iterable);
	template<template<typename...> class Iterable, typename ...Parameters, typename C = Iterable<Parameters...>, typename BeginType = decltype(declval<C>().begin()), typename EndType = decltype(declval<C>().end())>
	                                          inline void print_complex_item(ostream& out, const Iterable<Parameters...>& iterable);
	template<typename T>                      inline void print_complex_item(ostream& out, const T& value);
	
	template<typename T, typename X = decltype(declval<T>().python_like_print(cout))>
	                                          inline ostream& operator<<(ostream& out, const T& value);
	
	template<typename Iterator> inline void print_all(ostream& out, Iterator begin, Iterator end);
	template<size_t N = 0, typename ...T> inline void print_items_in_tuple(ostream &out, const tuple<T...>& tuple);
	template<size_t N, typename T> inline void print_members_with_names(ostream& out, vector<string> names, const T& arg);
	template<size_t N, typename F, typename ...T> inline void print_members_with_names(ostream& out, vector<string> names, const F& arg, T ...args);
	
	                     inline void print_item(ostream& out, const string& value);
	                     inline void print_item(ostream& out, const char* value);
	                     inline void print_item(ostream& out, const char value);
	template<typename T> inline void print_item(ostream& out, const T value);
	
	template<char SEP, char END>                            inline void print_to_stream(ostream& out);
	template<char SEP, char END, typename T, typename ...A> inline void print_to_stream(ostream& out, const T& value, A ...args);
}

// �W���o�͂ɏo�͂���֐��̎���
template<char SEP = ' ', char END = '\n', typename ...A> inline void print(A ...args){
	printo<SEP, END, A...>(std::cout, args...);
}

// �������̂̕W���G���[�o�[�W����
template<char SEP = ' ', char END = '\n', typename ...A> inline void printe(A ...args){
	printo<SEP, END, A...>(std::cerr, args...);
}

// �������̂̏o�͐掩�R�o�[�W����
template<char SEP = ' ', char END = '\n', typename ...A> inline void printo(std::ostream& out, A ...args){
	python_like_print::print_to_stream<SEP, END, A...>(out, args...);
}

// ����
namespace python_like_print{
	// �����^�Ƃ��Ă��o��
	
	// �������""�ň͂�
	inline void print_complex_item(ostream& out, const string& value){
		out << '"' << value << '"';
	}
	inline void print_complex_item(ostream& out, const char* value){
		out << '"' << value << '"';
	}
	
	// char��''�ň͂�
	inline void print_complex_item(ostream& out, char value){
		out << '\'' << value << '\'';
	}
	
	// pair�́A:�ŋ�؂���
	template<typename First, typename Second> inline void print_complex_item(ostream& out, const pair<First, Second>& pair){
		print_complex_item(out, pair.first);
		out << ": ";
		print_complex_item(out, pair.second);
	}

// tuple�̒��g���o��
// constexpr��C++17�ȍ~�łȂ��Ǝg���Ȃ��̂ŁA���ꖢ���̃o�[�W�����ŃR���p�C�����ꂽ�ꍇ�͒�`���΂��B
#if defined(__cplusplus) && __cplusplus >= 201703L
	// 2�ڈȍ~�̗v�f��", "��t���ďo��
	template<size_t N = 0, typename ...T> inline void print_items_in_tuple(ostream &out, const tuple<T...>& item){
		if constexpr(N < tuple_size<tuple<T...>>::value){
			out << ", ";
			print_complex_item(out, get<N>(item));
			print_items_in_tuple<N + 1, T...>(out, item);
		}
	}
	
	// tuple�͒��g��", "�ŋ�؂���'{'�`'}'�ň͂��ď��ɏo��
	template<typename ...T> inline void print_complex_item(ostream &out, const tuple<T...>& item){
		out << '{';
		if constexpr(tuple_size<tuple<T...>>::value > 0){
			print_complex_item(out, get<0>(item));
			print_items_in_tuple<1, T...>(out, item);
		}
		out << '}';
	}
#endif
	
	// bool��"true"/"false"���o��
	inline void print_complex_item(ostream& out, bool value){
		out << (value ? "true" : "false");
	}
	
	// map�̂悤�ɁA.begin()��.end()���t���Ă��āA.begin()->first��.begin()->second���������
	template<template<typename, typename, typename...> class Map, typename Key, typename Value, typename ...Parameters, typename C = Map<Key, Value, Parameters...>, typename BeginFirstType = decltype(declval<C>().begin()->first), typename BeginSecondType = decltype(declval<C>().begin()->second), typename EndType = decltype(declval<C>().end())>
	inline void print_complex_item(ostream& out, const Map<Key, Value, Parameters...>& iterable){
		out << '{';
		print_all(out, iterable.begin(), iterable.end());
		out << '}';
	}
	
	// ���̑��A.begin()��.end()���t���Ă������
	template<template<typename...> class Iterable, typename ...Parameters, typename C = Iterable<Parameters...>, typename BeginType = decltype(declval<C>().begin()), typename EndType = decltype(declval<C>().end())>
	inline void print_complex_item(ostream& out, const Iterable<Parameters...>& iterable){
		out << '[';
		print_all(out, iterable.begin(), iterable.end());
		out << ']';
	}
	
	// ���̑��A<< �ŏo�͂ł������(�ł��Ȃ����̂̓G���[)
	template<typename T> inline void print_complex_item(ostream& out, const T& value){
		out << value;
	}
	
	// pytho_like_print(ostream& out); ���\�b�h�������Ă�����̂� out << item; �ŏo�͂ł���悤��
	template<typename T, typename X = decltype(declval<T>().python_like_print(cout))>inline ostream& operator<<(ostream& out, const T& value){
		value.python_like_print(out);
		return out;
	}
	
	// .begin()����.end()�܂ł�", "��؂�ŏo�͂���
	template<typename Iterator> inline void print_all(ostream& out, Iterator begin, Iterator end){
		if(begin != end){
			auto i = begin;
			print_complex_item(out, *i);
			for(++ i; i != end; ++ i){
				out << ", ";
				print_complex_item(out, *i);
			}
		}
	}
	
	// �P�i�ŕ�����A������\������ꍇ��""��''�ň͂܂��ɂ��̂܂܏o��
	inline void print_item(ostream& out, const string& value){
		out << value;
	}
	inline void print_item(ostream& out, const char* value){
		out << value;
	}
	inline void print_item(ostream& out, const char value){
		out << value;
	}
	
	// ����ȊO�͕����^�Ƃ���[]��{}��""��''��t���ď���
	template<typename T> inline void print_item(ostream& out, const T value){
		print_complex_item(out, value);
	}
	
	// ����1��1���΂炯������
	// �΂炯������c�肪�������END�����o��
	template<char SEP, char END> inline void print_to_stream(ostream& out){
		out << END;
	}
	// 1�ȏ�̈���������ꍇ��1�ڂ��o�͂���SEP���o�͌�A�c����o��
	template<char SEP, char END, typename T, typename ...A> inline void print_to_stream(ostream& out, const T& value, A ...args){
		print_item(out, value);
		if(SEP != '\0'){
			out << SEP;
		}
		print_to_stream<SEP, END, A...>(out, args...);
	}
	
	// class��struct�̒��g��\�����邽�߂̃w���p�֐�
	template<typename T> inline void print_members(ostream& out, const T& arg){
		print_complex_item(out, arg);
	}
	
	template<typename F, typename ...T> inline void print_members(ostream& out, const F& arg, T ...args){
		print_complex_item(out, arg);
		out << ", ";
		print_members(out, args...);
	}
	
	template<size_t N, typename T> inline void print_members_with_names(ostream& out, vector<string> names, const T& arg){
		out << names[N] << ": ";
		print_complex_item(out, arg);
	}
	
	template<size_t N, typename F, typename ...T> inline void print_members_with_names(ostream& out, vector<string> names, const F& arg, T ...args){
		out << names[N] << ": ";
		print_complex_item(out, arg);
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
