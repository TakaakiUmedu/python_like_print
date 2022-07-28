Python風のprint()をC++で定義したもの ver.1.4

【使い方】
// ↓ここから
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
#include "print.h"
#else
template<char SEP = ' ', char END = '\n', class... A> inline void print(A&& ...){}
template<char SEP = ' ', char END = '\n', class... A> inline void printe(A&& ...){}
template<char SEP = ' ', char END = '\n', class... A> inline void printo(A&& ...){}
#define define_print(...)
#define define_print_with_names(...)
#define define_to_tuple(...)
#endif
// ↑ここまで

を使いたいソースコードの先頭に張り付ける(「print.h」の冒頭参照)。

c++ -O3 -std=c++17 -DUSE_PYTHON_LIKE_PRINT print-test.cpp

のように、「USE_PYTHON_LIKE_PRINT」が定義された状態でコンパイルすると、print()、printe()、printo()が使える。

print()は、std::vectorや、std::map、std::setなどを、なるべくPython風に出力する。
printe()は、標準エラー出力に出力する。
pythono()は、出力先を1つめの引数で指定する。

「USE_PYTHON_LIKE_PRINT」を定義しないよう、

c++ -O3 -std=c++17 print-test.cpp

のようにコンパイルした場合は、print()、printe()、printo()などの関数が空のinline関数に置き換わるので、
print()系の関数で出力仕様とした部分は、何も出力されなくなる。
(「print-test.cpp」は、print()のみを使っているので、実行しても何も表示されなくなる)

なお、std::tupleの表示にC++17以降の機能を使っているので、std::tupleが混ざったものを出力したい場合は、
コンパイル時に「-std=c++17」等、C++17としてコンパイルされるオプションを付ける。

また、構造体やクラスの定義のpublic:な部分に、

define_print(出力したいメンバー名);

か、

define_print_with_names(出力したいメンバー名);

のいずれかを書けば、その構造体やクラスも、print()で出力できる対象になる。
後者の場合は「メンバー名: 値」と表示されるようになる。両方を同時に書くとエラー。

struct Pos{
	int x, y, z;
	
	// std::make_tuple(x, y, z)する、to_tuple()メソッドを生やす
	define_to_tuple(x, y, z);
	
	// { x, y, z } を出力する、python_like_print(std::ostream& out)メソッドを生やす
//	define_print(x, y, z);
	
	// メンバーの名前付きの { x: x, y: y, z: z } を出力するpython_like_print(std::ostream& out)メソッドを生やす
	define_print_with_names(x, y, z);
}

【変更点】

ver.1.1
・#define print(...) で関数を消すと、何らかのクラスのメンバ関数などでprint()を定義しようとした時に、それも消されてエラーになるので、無効化の方針を変更
・あちこちtypoしていて、printe()などがまともにコンパイル通らなかったので修正
・SEPやENDについて、テンプレートのデフォルト値でバリエーションを表現できるので、複数あった関数・定義を集約して整理
・テストをいくつか追加

ver.1.2
・引数無しでprint();と呼び出した時にコンパイルエラーになっていたのを修正
・printe()の出力先を間違っていたのを修正
・「#includeの仕方」を調整して、競プロサイトへ投稿したら(-DUSE_PYTHON_LIKE_PRINTを付けずにコンパイルしたら)コンパイルエラーになる可能性を潰した

ver.1.3
・デバッグ

ver.1.4
・AtCoder Library( https://github.com/atcoder/ac-library )への対応を強化。Modint、DSU、Fenwick Tree、Lazy Segment Treee、Segment Treeeをprint()できるようにした。
　AtCoder Libraryの各ヘッダファイルをインクルードした後でprint.hをインクルードすると使えるようになる。
　ただしFenwick Tree、Lazy Segment Treee、Segment Treeeは、to_vector()メソッドを追加して、print()に対応させた拡張クラスを*_vectorizableとして定義し、
　#defineでソースコード上の元々のクラスの参照をそれらで置き換えるようにした強引な実装(実装に必要となる_nがprivateなので)。
　print.hをインクルードしなければ元通りの動作に戻る。
・*_vectorizableクラスからsize()メソッドを削除。デバッグでしか効かないものがあるのは紛らわしいので。

ver.1.4.1
・const& の付け忘れを追加
・begin()とend()の型が異なるiteratorに対応

ver.1.5
・ostream << objectが出来る型を絞り込んだり、begin()とend()がある型に対する絞り込みを付け加えたりSFINAEを改良。より多くの形式のobjectをprint()できるように
・using namespace std;を削除して個別にusing
・拡張子をhppに変更

ver.1.6
・mapの出力方針を変更。begin()、end()で取れるiteratorがpairを返す場合にmapの類と見做して「{」～「}」で囲んで出力する方針に

ver.1.6.1
・USE_PYTHON_LIKE_PRINTを定義しなかった場合のダミー関数が右辺値を受け付けずコンパイルエラーを生じていたのを修正
・get_fenwick_tree_value()の返り値が定義から抜けていたので追加

