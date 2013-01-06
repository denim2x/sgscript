
#ifndef SGS_PROC_H_INCLUDED
#define SGS_PROC_H_INCLUDED


#include "sgs_util.h"
#include "sgs_fnt.h"


#define CONSTENC( x ) ((x)|0x8000)
#define CONSTDEC( x ) ((x)&0x7fff)

typedef enum sgs_Instruction_e
{
	SI_NOP = 0,

	SI_PUSH,	/* (i16 src)			push register/constant */
	SI_PUSHN,	/* (u8 N)				push N nulls */
	SI_POPN,	/* (u8 N)				pop N items */
	SI_POPR,	/* (i16 out)			pop item to register */

	SI_RETN,	/* (u8 N)				exit current frame of execution, preserve N output arguments */
	SI_JUMP,	/* (i16 off)			add to instruction pointer */
	SI_JMPF,	/* (i16 src, off)		jump (add to instr.ptr.) if false */
	SI_CALL,	/* (u8 args, expect, i16 src)	call a variable */

	SI_GETVAR,	/* (i16 out, name)		<varname> => <value> */
	SI_SETVAR,	/* (i16 name, src)		<varname> <value> => set <value> to <varname> */
	SI_GETPROP,	/* (i16 out, var, name)	<var> <prop> => <var> */
	SI_SETPROP,	/* (i16 var, name, src)	<var> <prop> <value> => set a <prop> of <var> to <value> */
	SI_GETINDEX,/* -- || -- */
	SI_SETINDEX,/* -- || -- */

	/* operators */
	/*
		A: (i16 out, s1, s2)
		A1: (xormask, i16 out, s1, s2)
		B: (i16 out, s1)
	*/
	SI_SET,		/* B */
	SI_COPY,	/* B */
	SI_CONCAT,	/* A */
	SI_BOOL_AND,/* A */
	SI_BOOL_OR,	/* A */
	SI_NEGATE,	/* B */
	SI_BOOL_INV,/* B */
	SI_INVERT,	/* B */

	SI_INC,		/* B */
	SI_DEC,		/* B */
	SI_ADD,		/* A */
	SI_SUB,		/* A */
	SI_MUL,		/* A */
	SI_DIV,		/* A */
	SI_MOD,		/* A */

	SI_AND,		/* A */
	SI_OR,		/* A */
	SI_XOR,		/* A */
	SI_LSH,		/* A */
	SI_RSH,		/* A */

	SI_SEQ,		/* A1 */
	SI_EQ,		/* A1 */
	SI_LT,		/* A1 */
	SI_LTE,		/* A1 */

	/* specials */
	SI_ARRAY,	/* (u8 args, i16 out) */
	SI_DICT,	/* -- || -- */

	/* xor masks */
	SI_SNEQ = SI_SEQ | 0x80,
	SI_NEQ = SI_EQ | 0x80,
	SI_GT = SI_LT | 0x80,
	SI_GTE = SI_LTE | 0x80,
}
sgs_Instruction;


typedef struct funct_s
{
	char*	bytecode;
	int32_t	instr_off;
	int32_t	size;
}
funct;



struct _sgs_Variable
{
	int	refcount;
	uint8_t type;
	uint8_t redblue; /* red or blue? mark & sweep */
	uint8_t destroying; /* whether the variable is already in a process of destruction. for container circ. ref. handling. */
	union _sgs_Variable_data
	{
		/* 32/64 bit sizes, size of union isn't guaranteed to be max(all). */
		int32_t		B;	/* 4 */
		sgs_Integer	I;	/* 8 */
		sgs_Real	R;	/* 8 */
		StrBuf		S;	/* 12/16 */
		funct		F;	/* 12/16 */
		sgs_CFunc	C;	/* 4/8 */
		sgs_VarObj	O;	/* 8/16 */
	} data;
	sgs_Variable *prev, *next;
};

/*
sgs_Variable* sgsVM_VarCreate( SGS_CTX, int type );
void sgsVM_VarDestroy( SGS_CTX, sgs_Variable* var );
sgs_Variable* sgsVM_VarCreateString( SGS_CTX, const char* str, int32_t len );
*/
sgs_Variable* make_var( SGS_CTX, int type );
sgs_Variable* var_create_str( SGS_CTX, const char* str, int32_t len );
void destroy_var( SGS_CTX, sgs_Variable* var );
#define sgsVM_VarCreate make_var
#define sgsVM_VarDestroy destroy_var
#define sgsVM_VarCreateString var_create_str


int sgsVM_VarSize( sgs_Variable* var );
void sgsVM_VarDump( sgs_Variable* var );

void sgsVM_StackDump( SGS_CTX );

int sgsVM_ExecFn( SGS_CTX, const void* code, int32_t codesize, const void* data, int32_t datasize );


sgs_Variable* sgsVM_VarMake_Dict();

int sgsVM_RegStdLibs( SGS_CTX );


#endif /* SGS_PROC_H_INCLUDED */
