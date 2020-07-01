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


#define MAXINCLUDEFILE	256				// include できる file の数
#define	MAXTOKEN		128*1024		// 1 個の token の最大 size
#define	CALCMAXPUTTOKEN	128*1024		// calc の時の puttoken buffer
#define	CALCMAXREGIST	64*1024			// #calc の時の regist name
#define UNGETBUFFER		4*1024*1024		// ungetchar の buffer ( macro 展開用 )
#define MAX_MACROEXP	256*1024		// macro の para の展開用 buffer
#define MAX_REGISTNUM	2*1024*1024		// 定義できる最大の数
#define MAX_REGISTBUFF	32*1024*1024	// 定義の識別子の buffer ( 命令のキーワードも兼用 )
#define MAX_DEFINEBUFF	64*1024*1024	// macro 本体をおく buffer
#define MAX_MACROFILE	16*1024*1024	// macro の定義されている filename を保持する
#define MAXINCLUDEPATH	256				// path の数
#define MAXPATHNAME		256				// pathname の最大長
#define MAXIFNEST		512				// if の nest の数
#define MAXMACRONEST	4096			// macro の nest の数
#define YEN2ESC_BUFFER	65536			// yen2esc の buffer size
#define	MAXLINE			10				// 何 line 毎に出力するか
#define MAXDEFINEPARA	256				// macro でのパラメーターの最大数
#define MAXDEFINELOCAL	256				// macro での local パラメーターの最大数
#define PREBUFFERSIZE	4*1024*1024		// preprocessor file の出力バッファー
#define PREBUFFERLIMMIT	4*16*1024		// preprocessor file の出力バッファーの限界値
#define MAXPUTFILE		256				// 出力ファイルの最大数

#define MAX_OUTPUT		64*1024*1024	// 出力 buffer
#define MAX_LABEL0		16*1024*1024	// label data を入れるバッファー
#define MAX_LABEL1		16*1024*1024	// 前方参照を解決するためのバッファー
#define MAX_LABEL2		16*1024*1024	// ep0,ep1 の時の前方参照を解決するためのバッファー
#define MAX_LINEBUFF	128*1024		// 1 行分のバッファー
#define MAX_CALCMACBUFF	2*1024*1024		// cce の展開用バッファー

#define MAX_NUMSTACK	65536			// 数字 stack
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
void err_exit( void );						// error 終了時
extern int nWarning;						// warning


/* ==========================================================================
=============================================================================
		prototype
=============================================================================
========================================================================== */
