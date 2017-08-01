#include <stdio.h>
#include "error.h"
#include "print.h"
#include "eval.h"
#include <setjmp.h>

void scm_print_error(const char *info)
{
    printf("%s", info);
}

void scm_throw_error()
{
    longjmp(eval_error_jmp_buf, 1);
}

scm_object* scm_wrong_contract(const char *proc_name, const char *expected, int index, int argc, scm_object *argv[])
{
    scm_print_error(proc_name);
    scm_print_error(": contract violation\n");
    scm_print_error("  expected: ");
    scm_print_error(expected);
    scm_print_error("\n");
    scm_print_error("  given: ");
    scm_write(scm_stdout_port, argv[index]);
    scm_print_error("\n");
    if (argc > 1) {
        scm_print_error("  argument position: ");
        char buf[10] = {0};
        sprintf(buf, "%d\n", index + 1);
        scm_print_error(buf);

        scm_print_error("  other arguments...:\n");
        int argi;
        for (argi = 0; argi < argc; argi++) {
            if (argi == index)
                continue;
            scm_print_error("   ");
            scm_write(scm_stdout_port, argv[argi]);
            scm_print_error("\n");
        }
    }

    scm_throw_error();

    return NULL;
}

scm_object* scm_unmatched_arity(const char *proc_name, int is_atleast, const char *expected, int argc, scm_object *argv[])
{
    scm_print_error(proc_name);
    scm_print_error(": arity mismatch;\n");
    scm_print_error(" the expected number of arguments does not match the given number\n");
    scm_print_error("  expected: ");
    scm_print_error(is_atleast ? "at least " : "");
    scm_print_error(expected);
    scm_print_error("\n");
    scm_print_error("  given: ");
    char buf[10] = {0};
    sprintf(buf, "%d\n", argc);
    scm_print_error(buf);
    if (argc > 0) {
        scm_print_error("  arguments...:\n");
        int argi;
        for (argi = 0; argi < argc; argi++) {
            scm_print_error("   ");
            scm_write(scm_stdout_port, argv[argi]);
            scm_print_error("\n");
        }
    }
    
    scm_throw_error();

    return NULL;
}

scm_object* scm_undefined_identifier(scm_symbol *id)
{
    scm_print_error(SCM_SYMBOL_STR_VAL(id));
    scm_print_error(": undefined;\n cannot reference undefined identifier\n");
    
    scm_throw_error();
    
    return NULL;
}