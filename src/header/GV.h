#include "../../Library/DxLib.h"
//#include "define.h"

#ifdef GLOBAL_INSTANCE
#define GLOBAL
#else
#define GLOBAL extern 
#endif

#include "function.h"

//画像用変数宣言部
GLOBAL int img_bg;       //キャラクタ画像9枚分　X2(変身用)
GLOBAL int img_char[6][2][95];

GLOBAL int func_state;

GLOBAL char source[CODE_LINE_MAX][CODE_CHAR_MAX];

GLOBAL char sourceType[CODE_LINE_MAX][CODE_CHAR_MAX];

GLOBAL unsigned int sourceTypeParallel[CODE_LINE_MAX];

GLOBAL unsigned int sourceTypeIndent[CODE_LINE_MAX];

GLOBAL char TypeStack[CODE_LINE_MAX][CODE_CHAR_MAX];

GLOBAL char TypeInput[CODE_CHAR_MAX];

GLOBAL char Keyword[KEYWORD_KIND_MAX][KEYWORD_MAX][KEYWORD_CHAR_MAX];

GLOBAL int KeywordNum[KEYWORD_KIND_MAX];

GLOBAL int TypeCursor;

GLOBAL int ScreenCursor;

GLOBAL int CursorCnt;

GLOBAL int CodeLength;

GLOBAL int CodeTypeLength;

GLOBAL int TypingLine;

GLOBAL int StageTime;

GLOBAL double InputSeekX;

GLOBAL double InputSeekY;

GLOBAL double InputScreenSeekX;

GLOBAL double InputScreenSeekY;

GLOBAL int OKflag;