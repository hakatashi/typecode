#include "../header/GV.h"

void type_input() {
	int i,j;
	char input;

	while((input=GetInputChar(TRUE))!=0) {
		if (input>=CTRL_CODE_CMP) {
			if (TypeInput[CODE_CHAR_MAX-2]=='\0') {
				for (i=CODE_CHAR_MAX-1;i>=ScreenCursor+1;i--) {
					TypeInput[i]=TypeInput[i-1];
				}
				TypeInput[ScreenCursor]=input;
				ScreenCursor++;
			}
		} else {
			switch (input) {
			case CTRL_CODE_DEL:
				if (TypeInput[ScreenCursor]!='\0') {
					for (i=ScreenCursor;i<=CODE_CHAR_MAX-2;i++) {
						TypeInput[i]=TypeInput[i+1];
					}
				}
				break;
			case CTRL_CODE_BS:
				if (ScreenCursor>0) {
					for (i=ScreenCursor-1;i<=CODE_CHAR_MAX-2;i++) {
						TypeInput[i]=TypeInput[i+1];
					}
					ScreenCursor--;
				}
				break;
			case CTRL_CODE_RIGHT:
				if (TypeInput[ScreenCursor]!='\0') ScreenCursor++;
				break;
			case CTRL_CODE_LEFT:
				if (ScreenCursor-1>=0) ScreenCursor--;
				break;
			}
		}
		CursorCnt=0;
	}
}

void type_main() {
	type_input();
}