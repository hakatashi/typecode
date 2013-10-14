//GLOBAL_INSTANCE���`�����ꍇ�A���C���֐��p��exturn�������s��
#define GLOBAL_INSTANCE
//���t�@�C���̃C���N���[�h
#include "../header/GV.h"

//���[�v�ŕK���s���R�又��
int ProcessLoop(){
	if (ProcessMessage()!=0) return -1; //�v���Z�X�������G���[�Ȃ�-1��Ԃ�
	if (ClearDrawScreen()!=0) return -1; //��ʃN���A�������G���[�Ȃ�-1��Ԃ�
	GetHitKeyStateAll_2(); //���݂̃L�[���͏������s��
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){ //WindowsAPI�̒�^��
	ChangeWindowMode(TRUE); //�E�B���h�E���[�h�ŋN��
	if(DxLib_Init() == -1 || SetDrawScreen( DX_SCREEN_BACK )!=0) return -1; //�������Ɨ���ʉ�

	func_state=0; //func_state�̏�����
	SetDrawMode(DX_DRAWMODE_ANISOTROPIC); //�ٕ����t�B���^�����O�@�ŕ`�悷��
	launch_ini(); //�N�����������֐�

	while(ProcessLoop()==0){ //���C�����[�v

		//func_state�ɂ���ď󋵂�U�蕪��
		switch(func_state) {
		case -1: //�G���[
			break;
		case 0: //�N������
			//�ǂݍ��ݏ���
			if (load()==-1) { 
				//�ǂݍ��ݎ��s
				func_state=-1; //�G���[�ɑ���
			} else {
				//�ǂݍ��ݐ���
				gamestart_ini(); //�Q�[���J�n������������
				func_state=1; //�Q�[�����C���ɑ���
			}
			break;
		case 1: //�Q�[�����C��
			type_main(); //�Ō�����
			graph_main(); //�`��
			break;
		case 2: //Clear�\��
			graph_main(); //�`��
			break;
		}
		
		control_main(); //�e��֐�����

		if ( CheckStateKey(KEY_INPUT_ESCAPE) == 1 ) break; //�G�X�P�[�v�����͂��ꂽ��u���C�N
		ScreenFlip(); //����ʔ��f
	}

	DxLib_End(); //�c�w���C�u�����I������
	return 0; //�v���O�����I��
}