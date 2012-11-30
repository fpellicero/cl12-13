#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: cl.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define INPUTEND 1
#define PROGRAM 2
#define ENDPROGRAM 3
#define VARS 4
#define ENDVARS 5
#define PROCEDURE 6
#define ENDPROCEDURE 7
#define FUNCTION 8
#define ENDFUNCTION 9
#define RETURN 10
#define INT 11
#define BOOL 12
#define STRUCT 13
#define ENDSTRUCT 14
#define ARRAY 15
#define OF 16
#define WRITELN 17
#define WRITE 18
#define READ 19
#define IF 20
#define THEN 21
#define ELSE 22
#define ENDIF 23
#define WHILE 24
#define DO 25
#define ENDWHILE 26
#define VAL 27
#define REF 28
#define COMMA 29
#define PLUS 30
#define MINUS 31
#define TIMES 32
#define DIV 33
#define OPENPAR 34
#define CLOSEPAR 35
#define OPENBRA 36
#define CLOSEBRA 37
#define ASIG 38
#define DOT 39
#define GT 40
#define LT 41
#define EQ 42
#define AND 43
#define OR 44
#define NOT 45
#define True 46
#define False 47
#define IDENT 48
#define STRING 49
#define INTCONST 50
#define COMMENT 51
#define WHITESPACE 52
#define NEWLINE 53
#define LEXICALERROR 54

#ifdef __USE_PROTOS
void program(AST**_root);
#else
extern void program();
#endif

#ifdef __USE_PROTOS
void dec_vars(AST**_root);
#else
extern void dec_vars();
#endif

#ifdef __USE_PROTOS
void l_dec_vars(AST**_root);
#else
extern void l_dec_vars();
#endif

#ifdef __USE_PROTOS
void dec_var(AST**_root);
#else
extern void dec_var();
#endif

#ifdef __USE_PROTOS
void l_dec_blocs(AST**_root);
#else
extern void l_dec_blocs();
#endif

#ifdef __USE_PROTOS
void dec_bloc(AST**_root);
#else
extern void dec_bloc();
#endif

#ifdef __USE_PROTOS
void proc_header(AST**_root);
#else
extern void proc_header();
#endif

#ifdef __USE_PROTOS
void func_header(AST**_root);
#else
extern void func_header();
#endif

#ifdef __USE_PROTOS
void l_dec_input(AST**_root);
#else
extern void l_dec_input();
#endif

#ifdef __USE_PROTOS
void dec_input(AST**_root);
#else
extern void dec_input();
#endif

#ifdef __USE_PROTOS
void ret(AST**_root);
#else
extern void ret();
#endif

#ifdef __USE_PROTOS
void constr_type(AST**_root);
#else
extern void constr_type();
#endif

#ifdef __USE_PROTOS
void field(AST**_root);
#else
extern void field();
#endif

#ifdef __USE_PROTOS
void l_instrs(AST**_root);
#else
extern void l_instrs();
#endif

#ifdef __USE_PROTOS
void instruction(AST**_root);
#else
extern void instruction();
#endif

#ifdef __USE_PROTOS
void l_expr(AST**_root);
#else
extern void l_expr();
#endif

#ifdef __USE_PROTOS
void expr(AST**_root);
#else
extern void expr();
#endif

#ifdef __USE_PROTOS
void term(AST**_root);
#else
extern void term();
#endif

#ifdef __USE_PROTOS
void term2(AST**_root);
#else
extern void term2();
#endif

#ifdef __USE_PROTOS
void term3(AST**_root);
#else
extern void term3();
#endif

#ifdef __USE_PROTOS
void term4(AST**_root);
#else
extern void term4();
#endif

#ifdef __USE_PROTOS
void term5(AST**_root);
#else
extern void term5();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType setwd1[];
extern SetWordType zzerr3[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType setwd2[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType zzerr10[];
extern SetWordType setwd3[];
extern SetWordType zzerr11[];
extern SetWordType zzerr12[];
extern SetWordType zzerr13[];
extern SetWordType zzerr14[];
extern SetWordType setwd4[];
extern SetWordType zzerr15[];
extern SetWordType zzerr16[];
extern SetWordType zzerr17[];
extern SetWordType zzerr18[];
extern SetWordType setwd5[];
