/*===========================================================================
=============================================================================
		define
=============================================================================
===========================================================================*/
#define ASM_FLAG 1
#define UCHAR unsigned char
#define UINT unsigned int
#define FILE_NAME_MAX 256
#define FALSE 0
#define TRUE 1
#define USE_OCT 0


#define MAXINCLUDEFILE	256				// include �ł��� file �̐�
#define	MAXTOKEN		128*1024		// 1 �� token �̍ő� size
#define	CALCMAXPUTTOKEN	128*1024		// calc �̎��� puttoken buffer
#define	CALCMAXREGIST	64*1024			// #calc �̎��� regist name
#define UNGETBUFFER		4*1024*1024		// ungetchar �� buffer ( macro �W�J�p )
#define MAX_MACROEXP	256*1024		// macro �� para �̓W�J�p buffer
#define MAX_REGISTNUM	2*1024*1024		// ��`�ł���ő�̐�
#define MAX_REGISTBUFF	32*1024*1024	// ��`�̎��ʎq�� buffer ( ���߂̃L�[���[�h�����p )
#define MAX_DEFINEBUFF	64*1024*1024	// macro �{�̂����� buffer
#define MAX_MACROFILE	16*1024*1024	// macro �̒�`����Ă��� filename ��ێ�����
#define MAXINCLUDEPATH	256				// path �̐�
#define MAXPATHNAME		256				// pathname �̍ő咷
#define MAXIFNEST		512				// if �� nest �̐�
#define MAXMACRONEST	4096			// macro �� nest �̐�
#define YEN2ESC_BUFFER	65536			// yen2esc �� buffer size
#define	MAXLINE			10				// �� line ���ɏo�͂��邩
#define MAXDEFINEPARA	256				// macro �ł̃p�����[�^�[�̍ő吔
#define MAXDEFINELOCAL	256				// macro �ł� local �p�����[�^�[�̍ő吔
#define PREBUFFERSIZE	4*1024*1024		// preprocessor file �̏o�̓o�b�t�@�[
#define PREBUFFERLIMMIT	4*16*1024		// preprocessor file �̏o�̓o�b�t�@�[�̌��E�l
#define MAXPUTFILE		256				// �o�̓t�@�C���̍ő吔

#define MAX_OUTPUT		64*1024*1024	// �o�� buffer
#define MAX_LABEL0		16*1024*1024	// label data ������o�b�t�@�[
#define MAX_LABEL1		16*1024*1024	// �O���Q�Ƃ��������邽�߂̃o�b�t�@�[
#define MAX_LABEL2		16*1024*1024	// ep0,ep1 �̎��̑O���Q�Ƃ��������邽�߂̃o�b�t�@�[
#define MAX_LINEBUFF	128*1024		// 1 �s���̃o�b�t�@�[
#define MAX_CALCMACBUFF	2*1024*1024		// cce �̓W�J�p�o�b�t�@�[

#define MAX_NUMSTACK	65536			// ���� stack
#define MAX_STRSTACK	32*1024			// string stack
#define MAXSTRSTACKAREA 256*1024		// stack area


//
//	hash
//
#define HASHTABLESIZE	(65536*2)

/*===========================================================================
=============================================================================
		macro
=============================================================================
===========================================================================*/


/* ==========================================================================
=============================================================================
		extern
=============================================================================
========================================================================== */
void err_exit( void );						// error �I����
extern int nWarning;						// warning


/* ==========================================================================
=============================================================================
		prototype
=============================================================================
========================================================================== */
