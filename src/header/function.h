#include "define.h"

//graph.cpp
		//描画メイン
		GLOBAL void graph_main();

//key.cpp
		//現在のキー入力処理を行う
		GLOBAL int GetHitKeyStateAll_2();
		//受け取ったキー番号の現在の入力状態を返す
		GLOBAL int CheckStateKey(unsigned char Handle);

//laod.cpp
		//データのロード
		GLOBAL int load();

//ini.cpp
		//最初の初期化
		GLOBAL void launch_ini();
		//ゲーム開始時初期化
		GLOBAL void gamestart_ini();

//control.cpp
		//ゲームメイン制御
		GLOBAL void control_main();

//type.cpp
		//タイプ処理
		GLOBAL void type_main();