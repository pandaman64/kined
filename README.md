#kined
Kinect for Windows SDKのDバインディングです。[CombGig](http://mt-caret.github.io/events/)という勉強会で発表したものを公開しています。


#使い方

このライブラリはCラッパ部分とDヘッダ部分から成ります。Cラッパを事前にコンパイルして、Kinect10.libと一緒にリンクして下さい。

ライブラリにはポインタを要求する関数があります。それらのポインタは十分容量を確保されたメモリを指示している必要があります。メモリの妥当性はチェックされません。

#kined

D bindings for Kinect for Windows SDK. This library allows you to write Kinect apps in *D language*.

This library was introduced in [CombGig], and now this is public around the world.

#Usage

This library consists of C wrapper library and D header. You can compile C library and link it with your source and Kinect10.lib.

Some functions in this library requires pointers as its arguments. These pointers have to refer to an allocated memory which can contain all the elements. This library doesn't check whether the memory is valid.

