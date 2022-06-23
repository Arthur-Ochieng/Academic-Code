from sys import modules
import ply.lex as lex
import token_rules

# Build the lexer
lexer = lex.lex(module=token_rules)

def scanner(data):
  print("\n\n*** SCANNER OUTPUT ***\n\n ")

  # Give the lexer some input
  lexer.input(data)
  #print(f"TOKEN \t\t VALUE \t\t LINE")
  # Tokenize
  while True:
      tok = lexer.token()
      if not tok:
          break      # No more input
      print(f"{tok.type} {tok.value} {tok.lineno}")
      #print(tok.type, tok.value, tok.lineno, tok.lexpos)
  print("\n\n")



if __name__ == "__main__":
  with open('code.nib', 'r') as inp:
      scanner(inp.read())

