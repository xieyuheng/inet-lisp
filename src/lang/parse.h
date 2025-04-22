#pragma once

exp_t *parse_exp(sexp_t *sexp);
stmt_t *parse_stmt(sexp_t *sexp);
list_t *parse_stmt_list(list_t *sexp_list);
