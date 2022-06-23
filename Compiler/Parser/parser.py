import ply.yacc as yacc

# Get the token map from the lexer.  This is required.
from token_rules import tokens
from test_lexer import lexer, scanner


def p_statement_list_statement(p):
    'statement_list : statement'
    p[0] = p[1]

# Statements


def p_statement_store(p):
    'statement : store'
    p[0] = p[1]


def p_statement_read(p):
    'statement : read'
    p[0] = p[1]


def p_statement_write(p):
    'statement : write'
    p[0] = p[1]


def p_statement_for(p):
    'statement : for'
    p[0] = p[1]


def p_statement_while(p):
    'statement : while'
    p[0] = p[1]


def p_statement_if(p):
    'statement : if'
    p[0] = p[1]


def p_write_print_lparen_expression_rparen(p):
    'write : PRINT LPAREN expression RPAREN'
    p[0] = p[1]


def p_write_print_lparen_string_var_rparen(p):
    'write : PRINT LPAREN STRINGVARIBABLE RPAREN'
    p[0] = p[1]


def p_write_print_lparen_int_var_rparen(p):
    'write : PRINT LPAREN INTEGERVARIBABLE RPAREN'
    p[0] = p[1]


def p_write_print_lparen_int_rparen(p):
    'write : PRINT LPAREN NUMBER RPAREN'
    p[0] = p[1]


def p_write_print_lparen_string_rparen(p):
    'write : PRINT LPAREN STRING RPAREN'
    p[0] = p[1]


def p_if_expression(p):
    'if : IF expression COLON body'
    p[0] = p[1]


def p_if_expression_else(p):
    'if : IF expression COLON body ELSE COLON body'
    p[0] = p[1]


def p_while_term(p):
    'while : WHILE expression COLON body'
    p[0] = p[1]


def p_for_term(p):
    'for : FOR expression COLON body'
    p[0] = p[1]


def p_read_term(p):
    'read : INPUT LPAREN RPAREN'
    p[0] = p[1]


def p_store_term(p):
    'store : term'
    p[0] = p[1]


def p_store_expression(p):
    'store : expression'
    p[0] = p[1]


def p_body_statement_list(p):
    'body : statement_list'
    p[0] = p[1]


def p_body_statement(p):
    'body : statement'
    p[0] = p[1]


def p_expression_term(p):
    'expression : term'
    p[0] = p[1]


def p_expression(p):
    '''expression : expression PLUS term
                  | expression MINUS term
                  | expression TIMES term
                  | expression DIVIDE term
                  | expression MOD term
                  | expression LESS term
                  | expression LESSEQUAL term
                  | expression GREATER term
                  | expression GREATEQUAL term'''
    if p[2] == '+':
        p[0] = p[1] + p[3]

    elif p[2] == '-':
        p[0] = p[1] - p[3]

    elif p[2] == '*':
        p[0] = p[1] - p[3]

    elif p[2] == '/':
        p[0] = p[1] - p[3]

    elif p[2] == '%':
        p[0] = p[1] - p[3]

    elif p[2] == '<':
        p[0] = p[1] < p[3]

    elif p[2] == '<=':
        p[0] = p[1] > p[3]

    elif p[2] == '>':
        p[0] = p[1] > p[3]

    elif p[2] == '>=':
        p[0] = p[1] > p[3]


def p_term_times(p):
    'term : term TIMES factor'
    p[0] = p[1] * p[3]


def p_term_div(p):
    'term : term DIVIDE factor'
    p[0] = p[1] / p[3]


def p_term_mod(p):
    'term : term MOD factor'
    p[0] = p[1] / p[3]


def p_term_less(p):
    'term : term LESS factor'
    p[0] = p[1] < p[3]


def p_term_greater(p):
    'term : term GREATER factor'
    p[0] = p[1] < p[3]


def p_term_greatequal(p):
    'term : term GREATEQUAL factor'
    p[0] = p[1] >= p[3]


def p_term_equal(p):
    'term : term EQUAL factor'
    p[0] = p[1] = p[3]


def p_term_equalto(p):
    'term : term EQUALTO factor'
    p[0] = p[1] == p[3]


def p_term_factor(p):
    'term : factor'
    p[0] = p[1]


def p_factor_num(p):
    'factor : NUMBER'
    p[0] = p[1]


def p_factor_string(p):
    'factor : STRING'
    p[0] = p[1]


def p_factor_expr(p):
    'factor : LPAREN expression RPAREN'
    p[0] = p[2]


def p_factor_string(p):
    'factor : LPAREN STRINGVARIBABLE RPAREN'
    p[0] = p[2]


def p_factor_int(p):
    'factor : LPAREN INTEGERVARIBABLE RPAREN'
    p[0] = p[2]

# Error rule for syntax errors


def p_error(p):
    print(f"Parser Error : Line {p.lineno}")


# Build the parser
parser = yacc.yacc()

with open('code.nib', 'r') as inp:
    scanner(inp.read())

with open('code.nib', 'r') as inp:
    result = parser.parse(inp.read(), lexer=lexer)
    if result:
        print("Accepted by the grammar")
"""

while True:
    try:
        s = input('Input > ')
    except EOFError:
        break
    if not s:
        continue
    result = parser.parse(s, lexer=lexer)
    if result:
        print("Accepted by the grammar")


def main():
    while True:
        try:
            s = input('Input > ')
        except EOFError:
            break
        if not s: continue
        result = parser.parse(s,lexer=lexer)
        if result:
            print("Accepted by the grammar")
   
if __name__ == "__main__":
    main()
"""
