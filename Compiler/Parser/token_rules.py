# List of token names.   This is always required
tokens = (
    'NUMBER',
    'PLUS',
    'MINUS',
    'TIMES',
    'DIVIDE',
    'LPAREN',
    'RPAREN',
    'MOD',
    'LESSEQUAL',
    'GREATEQUAL',
    'LESS',
    'GREATER',
    'EQUALTO',
    'COLON',
    'EQUAL',

    'PRINT',
    'INPUT',
    'WHILE',
    'FOR',
    'IF',
    'ELSE',
    'STRINGVARIBABLE',
    'INTEGERVARIBABLE',
    'STRING',
    

)

# Regular expression rules for simple tokens
t_PLUS = r'\+'
t_MINUS = r'-'
t_TIMES = r'\*'
t_DIVIDE = r'/'
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_MOD = r'\%'

t_EQUALTO = r'\=='
t_EQUAL = r'\='
t_LESSEQUAL = r'\<='
t_GREATEQUAL = r'\>='
t_LESS = r'\<'
t_GREATER = r'\>'
t_COLON = r'\:'


t_PRINT = r'print'
t_INPUT = r'input'
t_WHILE = r'while'
t_FOR = r'for'
t_IF = r'if'
t_ELSE = r'else'
# A regular expression rule with some action code


def t_NUMBER(t):
    r'\d+'
    t.value = int(t.value)
    return t

def t_INTEGERVARIBABLE(t):
    r'n_[a-zA-z]+'
    return t

def t_STRINGVARIBABLE(t):
    r's_[a-zA-z]+'
    return t

# Track line numbers
def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)

def t_STRING(t):
    r'"((?:""|[^"])*)"'
    return t

# A string containing ignored characters (spaces and tabs)
t_ignore = ' \t'

# Error handling rule


def t_error(t):
    print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)
