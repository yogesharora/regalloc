#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s3inst.h"
#include "cmdline.h"
#include "RegisterAllocator.h"

extern int num_errors;
extern int yyerror(...);
extern "C" int yywrap();
extern int yyparse();
extern int cmdlex();
extern int k;

void c_regalloc(void);
void codegen_entry(FILE *fptr);
void codegen_exit(FILE *fptr);
void find_function(void);
void print_inst(FILE*, inst_t);
void print_list(FILE*, inst_t);

inst_t instList; /* list of instructions found by parser */

int main(int argc, char **argv)
{
	arglim = argv + argc;
	targv = argv + 1;

	cmdlex();

	if (outfile == NULL)
		outfile = strdup("out.asm");

	if (infile[0])
	{
		c_regalloc();
	}

	return 0;
}

void c_regalloc()
{
	/* file pointer to dump output code */
	yywrap();
	yyparse();

	if (num_errors > 0)
		return;

	if (verbose)
		print_list(stdout, instList);

	find_function(); /* remove extra instructions needed for simulation */

	/************************************************************************/
	/************************************************************************/
	/************************************************************************/
	/************************************************************************/
	/*    Call your implementation from here                                */
	RegisterAllocator reg( instList);
	if (k == 0)
		k = DEFAULT_K_VALUE;
	FILE *fptr = fopen(outfile, "w");
		codegen_entry(fptr);

	if (reg.allocateRegs(10, k, 10))
	{
		FILE *fptr = fopen(outfile, "w");
		codegen_entry(fptr);
		reg.printInstructions(fptr);
		fprintf(fptr, "\t.END\n");
	}
	else
	{
		fprintf(fptr, ";unable to find an allocation\n");
		print_list(fptr, instList);
		fprintf(stdout, "unable to find an allocation\n");

	}
	codegen_exit(fptr);
	fclose(fptr); /* close file */
}

/**************************************************************************
 *   Support for generating code
 */

void codegen_entry(FILE *fptr)
{
	fprintf(fptr, "\t.ORIG x2000\n");
	fprintf(fptr, "\tJSR main\n");
	fprintf(fptr, "\tHALT\n");
}

void codegen_exit(FILE *fptr)
{
	fprintf(fptr, "\t.END\n");
}

void find_function()
{
	/* Need to remove first three instructions */
	inst_t tmp, tmp1, tmp2;
	inst_t oldhead;
	int found = 0;

	if (instList->op == OP_ORIG)
	{
		tmp = instList->next;
		if (tmp->op == OP_JSR && strcmp(tmp->ops[0].label, "main") == 0)
		{
			tmp = tmp->next;
			if (tmp->op == OP_HALT)
			{
				found = 1;
				tmp = tmp->next;
				while (instList != tmp)
				{
					oldhead = instList;
					instList = instList->next;
					free(oldhead);
				}
			}
		}
	}

	if (!found)
	{
		printf("Warning: Beginning of input not in the expected format!\n");
	}

	/* Remove last instruction: END */
	tmp = instList;
	tmp1 = tmp;
	tmp2 = tmp1;
	while (tmp != NULL)
	{
		tmp2 = tmp1;
		tmp1 = tmp;
		tmp = tmp->next;
	}

	if (tmp1->op == OP_END && tmp2 != tmp1)
	{
		tmp2->next = NULL;
		free(tmp1);
	}
	else
	{
		/*printf("Warning: Did not find .END.\n");*/
	}
}

void print_cc(FILE *fptr, int ccode)
{
	if (ccode & CC_N)
		fprintf(fptr, "n");
	if (ccode & CC_Z)
		fprintf(fptr, "z");
	if (ccode & CC_P)
		fprintf(fptr, "p");

	fprintf(fptr, " ");
}

void print_op(FILE *fptr, struct operand op)
{
	enum op_type t = op.t;
	switch (t)
	{
		case op_reg :
			fprintf(fptr, "R%d", op.reg);
			break;
		case op_imm :
			fprintf(fptr, "#%d", op.imm);
			break;
		case op_label :
			fprintf(fptr, "%s", op.label);
			break;
	}
}

void print_inst(FILE* fptr, inst_t i)
{
	if (i->label)
	{
		fprintf(fptr, "%s:", i->label);
	}

	if (i->op == OP_BR)
	{
		fprintf(fptr, "\t%s", opnames[i->op]);
		print_cc(fptr, i->ccode);
	}
	else
		fprintf(fptr, "\t%s ", opnames[i->op]);

	switch (i->op)
	{

		/* 3 operands */
		case OP_ADD :
		case OP_AND :
		case OP_ANDL :
		case OP_DIV :
		case OP_LDR :
		case OP_MUL :
		case OP_OR :
		case OP_ORL :
		case OP_STR :
		case OP_SUB :
			print_op(fptr, i->ops[0]);
			fprintf(fptr, ", ");
			print_op(fptr, i->ops[1]);
			fprintf(fptr, ", ");
			print_op(fptr, i->ops[2]);
			break;
			/* 2 operands */
		case OP_BR :
		case OP_SET :
		case OP_ST :
		case OP_STI :
		case OP_LD :
		case OP_LDI :
		case OP_LEA :
		case OP_NOT :
		case OP_NOTL :
			print_op(fptr, i->ops[0]);
			fprintf(fptr, ", ");
			print_op(fptr, i->ops[1]);
			break;

			/* one operand */
		case OP_JSRR :
		case OP_BRA :
		case OP_JMP :
		case OP_JSR :
			print_op(fptr, i->ops[0]);

		default :
			break;
	}
	fprintf(fptr, "\n");
}

void print_list(FILE *fptr, inst_t head)
{
	while (head)
	{
		print_inst(fptr, head);
		head = head->next;
	}
}
