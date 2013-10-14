#include "../header/GV.h"

void launch_ini() {
	/*int MultiFlag,i,j;
	char TestCase[64];*/

	/*for (i=0;i<10;i++) {
		switch(i) {
		case 0:
			strcpy_s(TestCase,"A");
			break;
		case 1:
			strcpy_s(TestCase,"あ");
			break;
		case 2:
			strcpy_s(TestCase,"AB");
			break;
		case 3:
			strcpy_s(TestCase,"あい");
			break;
		case 4:
			strcpy_s(TestCase,"点A");
			break;
		case 5:
			strcpy_s(TestCase,"K点");
			break;
		case 6:
			strcpy_s(TestCase,"†");
			break;
		case 7:
			strcpy_s(TestCase,"岡崎最高");
			break;
		case 8:
			strcpy_s(TestCase,"　 ");
			break;
		case 9:
			strcpy_s(TestCase,"TPP参加表明");
			break;
		}
		MultiFlag=MultiByteCharCheck(TestCase,DX_CHARSET_SHFTJIS);
		printfDx("%s : %s\n",TestCase,(MultiFlag==TRUE)?"TRUE":"FALSE");
		j=0;
		while(TestCase[j]!='\0') {
			printfDx("%u ",TestCase[j]);
			j++;
		}
		printfDx("\n");
		j=0;
		while(TestCase[j]!='\0') {
			printfDx("%X ",TestCase[j]);
			j++;
		}
		printfDx("\n");
	}*/
}

void gamestart_ini() {
	int i,j;

	TypingLine=0;
	TypeCursor=0;
	ScreenCursor=0;
	CursorCnt=0;
	InputSeekX=0.0;
	InputSeekY=0.0;
	InputScreenSeekX=0.0;
	InputScreenSeekY=0.0;
	OKflag=FALSE;
	for (i=0;i<CODE_CHAR_MAX;i++) TypeInput[i]='\0';
	for (i=0;i<CODE_LINE_MAX;i++) TypeStack[i][0]='\0';
	StageTime=0;

	//最初の行の一つ手前の行までTypeStackに付加
	for ( i=0 ; i<sourceTypeParallel[0] ; i++) {
		for ( j=0 ; source[i][j]!='\0' ; j++ ) TypeStack[i][j]=source[i][j];
		TypeStack[i][j]='\0';
	}
	//最初の行のインデントを入力
	for ( i=0 ; i<sourceTypeIndent[0] ; i++ )
		TypeStack[ sourceTypeParallel[0] ][ i ]
			= source[ sourceTypeParallel[0] ][ i ];
	TypeStack[ sourceTypeParallel[0] ][ i ] = '\0';
}