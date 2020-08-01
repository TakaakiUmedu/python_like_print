#ifndef PYTHON_LIKE_PRINT
#define PYTHON_LIKE_PRINT

/*** 推奨するincludeの仕方

// ↓ここから
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
// ↑ここまで

 ***/

// Python風に標準出力に出力する。
// SEP、ENDを付けるとそのように出力される。
// SEPに''を指定するとエラーになる。指定したい場合には代わりにprint<'\0'>()とする。
// ENDを付けたい場合、SEPは省略できない。
template<char SEP = ' ', char END = '\n', typename ...A> inline void print(A ...args);

// 同じものの標準エラーバージョン
template<char SEP = ' ', char END = '\n', typename ...A> inline void printe(A ...args);

// 同じものの出力先自由バージョン
template<char SEP = ' ', char END = '\n', typename ...A> inline void printo(std::ostream& out, A ...args);

// classやstructの中身を表示できるようにする。
// classやstructの ** 定義の中 ** で、
//
//    define_print( 表示させたいメンバをカンマ区切りで好きなだけ, ...)
//
// とすると、print(そのクラスのオブジェクト); ができるようになる(そのオブジェクトを含むvectorやmap等も同様)。
//
//    define_print_with_names(表示させたいメンバをカンマ区切りで好きなだけ, ...)
//
// とすると、それぞれ「メンバ名: 値」と表示されてより見やすくなる。
// 同じ関数を定義しているので、両方同時に指定するとエラー。
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

// 同じようなノリで、指定したメンバーをtupleに変換する関数を定義する
#define define_to_tuple(...) \
	inline decltype(std::make_tuple(__VA_ARGS__)) to_tuple() const{\
		return (decltype(std::make_tuple(__VA_ARGS__)))(*this);\
	}\
	inline operator decltype(std::make_tuple(__VA_ARGS__))() const{\
		return std::make_tuple(__VA_ARGS__);\
	}


// 宣言を纏めて
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

// 標準出力に出力する関数の実体
template<char SEP = ' ', char END = '\n', typename ...A> inline void print(A ...args){
	printo<SEP, END, A...>(std::cout, args...);
}

// 同じものの標準エラーバージョン
template<char SEP = ' ', char END = '\n', typename ...A> inline void printe(A ...args){
	printo<SEP, END, A...>(std::cerr, args...);
}

// 同じものの出力先自由バージョン
template<char SEP = ' ', char END = '\n', typename ...A> inline void printo(std::ostream& out, A ...args){
	python_like_print::print_to_stream<SEP, END, A...>(out, args...);
}

// 実装
namespace python_like_print{
	// 複合型としてを出力
	
	// 文字列は""で囲む
	inline void print_complex_item(ostream& out, const string& value){
		out << '"' << value << '"';
	}
	inline void print_complex_item(ostream& out, const char* value){
		out << '"' << value << '"';
	}
	
	// charは''で囲む
	inline void print_complex_item(ostream& out, char value){
		out << '\'' << value << '\'';
	}
	
	// pairは、:で区切って
	template<typename First, typename Second> inline void print_complex_item(ostream& out, const pair<First, Second>& pair){
		print_complex_item(out, pair.first);
		out << ": ";
		print_complex_item(out, pair.second);
	}

// tupleの中身を出力
// constexprはC++17以降でないと使えないので、それ未満のバージョンでコンパイルされた場合は定義を飛ばす。
#if defined(__cplusplus) && __cplusplus >= 201703L
	// 2個目以降の要素を", "を付けて出力
	template<size_t N = 0, typename ...T> inline void print_items_in_tuple(ostream &out, const tuple<T...>& item){
		if constexpr(N < tuple_size<tuple<T...>>::value){
			out << ", ";
			print_complex_item(out, get<N>(item));
			print_items_in_tuple<N + 1, T...>(out, item);
		}
	}
	
	// tupleは中身を", "で区切って'{'～'}'で囲って順に出力
	template<typename ...T> inline void print_complex_item(ostream &out, const tuple<T...>& item){
		out << '{';
		if constexpr(tuple_size<tuple<T...>>::value > 0){
			print_complex_item(out, get<0>(item));
			print_items_in_tuple<1, T...>(out, item);
		}
		out << '}';
	}
#endif
	
	// boolは"true"/"false"を出力
	inline void print_complex_item(ostream& out, bool value){
		out << (value ? "true" : "false");
	}
	
	// mapのように、.begin()と.end()が付いていて、.begin()->firstと.begin()->secondがあるもの
	template<template<typename, typename, typename...> class Map, typename Key, typename Value, typename ...Parameters, typename C = Map<Key, Value, Parameters...>, typename BeginFirstType = decltype(declval<C>().begin()->first), typename BeginSecondType = decltype(declval<C>().begin()->second), typename EndType = decltype(declval<C>().end())>
	inline void print_complex_item(ostream& out, const Map<Key, Value, Parameters...>& iterable){
		out << '{';
		print_all(out, iterable.begin(), iterable.end());
		out << '}';
	}
	
	// その他、.begin()と.end()が付いているもの
	template<template<typename...> class Iterable, typename ...Parameters, typename C = Iterable<Parameters...>, typename BeginType = decltype(declval<C>().begin()), typename EndType = decltype(declval<C>().end())>
	inline void print_complex_item(ostream& out, const Iterable<Parameters...>& iterable){
		out << '[';
		print_all(out, iterable.begin(), iterable.end());
		out << ']';
	}
	
	// その他、<< で出力できるもの(できないものはエラー)
	template<typename T> inline void print_complex_item(ostream& out, const T& value){
		out << value;
	}
	
	// pytho_like_print(ostream& out); メソッドが生えているものを out << item; で出力できるように
	template<typename T, typename X = decltype(declval<T>().python_like_print(cout))>inline ostream& operator<<(ostream& out, const T& value){
		value.python_like_print(out);
		return out;
	}
	
	// .begin()から.end()までを", "区切りで出力する
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
	
	// 単品で文字列、文字を表示する場合は""や''で囲まずにそのまま出力
	inline void print_item(ostream& out, const string& value){
		out << value;
	}
	inline void print_item(ostream& out, const char* value){
		out << value;
	}
	inline void print_item(ostream& out, const char value){
		out << value;
	}
	
	// それ以外は複合型として[]や{}や""や''を付けて処理
	template<typename T> inline void print_item(ostream& out, const T value){
		print_complex_item(out, value);
	}
	
	// 引数1つ1つをばらけさせる
	// ばらけさせる残りが無ければENDだけ出力
	template<char SEP, char END> inline void print_to_stream(ostream& out){
		out << END;
	}
	// 1つ以上の引数がある場合は1つ目を出力してSEPを出力後、残りを出力
	template<char SEP, char END, typename T, typename ...A> inline void print_to_stream(ostream& out, const T& value, A ...args){
		print_item(out, value);
		if(SEP != '\0'){
			out << SEP;
		}
		print_to_stream<SEP, END, A...>(out, args...);
	}
	
	// classやstructの中身を表示するためのヘルパ関数
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


// splitとstripの実装
//
// ソースファイルを分割コンパイルする場合、
// - static vector<string> split(const string& str, const string& delim)
// - static string strip(const string& str)
// の2つは、このままだと関数が重複してリンクされるのを避けると、バイナリサイズが減るかも?
//
namespace python_like_split_and_strip{
	using namespace std;
	
	// split(文字列, 区切り) あれこれ
	                                 static vector<string> split(const string& str, const string& delim);
	template<typename S, typename T> inline vector<string> split(const S& str, const T& delim);
	template<typename S>             inline vector<string> split(const S& str, const char delim);
	
	// strip(文字列)
	static string strip(const string& str);
	
	// splitしてからそれぞれをstripする
	template<typename S, typename T> vector<string> split_strip(const S& str, const T& delim);
	

	// stringの代わりにchar*やcharも渡せるように
	template<typename S, typename T> vector<string> split(const S& str, const T& delim){
		return split(string(str), string(delim));
	}
	template<typename S> vector<string> split(const S& str, const char delim){
		return split(string(str), string(1, delim));
	}
	
	// splitした上でそれぞれをstripする
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
