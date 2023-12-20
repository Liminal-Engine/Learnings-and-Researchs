# parsing is often broken up in 2 stages :
# 1. Lexical analysis : break source input into simplest decomposable elements of a language called "tokens"
# 2. Syntatic analysiss : this is what is called "parsing" receives the lisk of tokens and tries to find patterns in them to
#     meet the language being parsed
#   lexer = analyseur syntaxique

# 1. Define constants :
JSON_COMMA = ','
JSON_COLON = ':'
JSON_LEFT_BRACKET = '['
JSON_RIGHT_BRACKET = ']'
JSON_LEFT_BRACE = '{'
JSON_RIGHT_BRACE = '}'
JSON_QUOTE ='"' #TODO : does JSON accecpt also single quotes '\''
JSON_WHITE_SPACE = [' ', '\t', '\b', '\n', '\r' ]
JSON_TOKENS = [JSON_COMMA, JSON_COLON, JSON_LEFT_BRACKET, JSON_RIGHT_BRACKET, JSON_LEFT_BRACE, JSON_RIGHT_BRACE]
FALSE_LEN = len('false')
TRUE_LEN = len('true')
NULL_LEN = len('null')
NUM_CHARS = [str(d) for d in range(0, 10)] + ['-', 'e', '.']

# 2. Define the lexer class

class JSONLexer:
    def __init__(self, wholeJSONString: str) -> None:
        self._string = wholeJSONString

    def lex(self):
        tokens = []
        tmp_token = ""

        while len(self._string):  # until the string still has a length
            # 1. strings
            tmp_token = self._lex_string()
            if tmp_token is not None:
                tokens.append(tmp_token)
                continue
            # 2. numbers
            tmp_token = self._lex_number()
            if tmp_token is not None:
                tokens.append(tmp_token)
                continue
            # 3. Booleans
            tmp_token = self._lex_bool()
            if tmp_token is not None:
                tokens.append(tmp_token)
                continue
            # 4. Nulls
            tmp_token = self._lex_null()
            if tmp_token is not None:
                tokens.append(tmp_token)
                continue
            # 5. If none of the previous token type is found :
            c = self._string[0]
            if c in JSON_WHITE_SPACE: # 5.1 If is a whitespace, ignore it
                self._string = self._string[1:]
            elif c in JSON_TOKENS: # 5.2 else, if it is a known JSON token, add it to the token list
                self._string=self._string[1:]
                tokens.append(c)
            else:
                raise Exception(f"Unexpected character : {c}")
        return tokens
    
    def _lex_string(self):
        res = ""
        if self._string[0] == JSON_QUOTE:
            self._string = self._string[1:]
        else:
            return None

        for c in self._string:
            if c == JSON_QUOTE: #e.g. if end of string
                self._string = self._string[len(res)+1:] # we modify the input string to squeeze what has been parsed
                return res # we return
            else:
                res += c # we increment the res
        raise Exception("Error while parsing string")
    
    def _lex_number(self):
        res = ""

        for c in self._string:
            if c in NUM_CHARS:
                res += c
            else:
                break
        rest = self._string[len(res)]
        if not len(res): #if nothing found, return None
            return None
        # if something found, first modify input string, then return corresponding number type e.g. int or float in our case
        self._string = self._string[len(res):]
        if '.' in res:
            return float(res)
        return int(res)

    def _lex_bool(self):
        res = ""
        string_len = len(self._string)
        if string_len >= TRUE_LEN and self._string[:TRUE_LEN] == 'true':
            self._string = self._string[TRUE_LEN:]
            return True
        elif string_len >= FALSE_LEN and self._string[:FALSE_LEN] == 'false':
            self._string = self._string[FALSE_LEN:]
            return False
        return None

    def _lex_null(self):
        string_len = len(self._string)
        
        if string_len > NULL_LEN and self._string[:NULL_LEN] == 'null':
            self._string = self._string[NULL_LEN:]
            return True
        return None

# 3. Define the parser class, which will use the lexer class to get the list of tokens
def parse_array(tokens):
    json_array = []

    t = tokens[0]
    if t == JSON_RIGHT_BRACKET:
        return json_array, tokens[1:]

    while True:
        json, tokens = parse(tokens)
        json_array.append(json)

        t = tokens[0]
        if t == JSON_RIGHT_BRACKET:
            return json_array, tokens[1:]
        elif t != JSON_COMMA:
            raise Exception('Expected comma after object in array')
        else:
            tokens = tokens[1:]

    raise Exception('Expected end-of-array bracket')


def parse_object(tokens):
    json_object = {}

    t = tokens[0]
    if t == JSON_RIGHT_BRACE:
        return json_object, tokens[1:]

    while True:
        json_key = tokens[0]
        if type(json_key) is str:
            tokens = tokens[1:]
        else:
            raise Exception('Expected string key, got: {}'.format(json_key))

        if tokens[0] != JSON_COLON:
            raise Exception('Expected colon after key in object, got: {}'.format(t))

        json_value, tokens = parse(tokens[1:])

        json_object[json_key] = json_value

        t = tokens[0]
        if t == JSON_RIGHT_BRACE:
            return json_object, tokens[1:]
        elif t != JSON_COMMA:
            raise Exception('Expected comma after pair in object, got: {}'.format(t))

        tokens = tokens[1:]

    raise Exception('Expected end-of-object bracket')

def parse(tokens, is_root=False):
    t = tokens[0]

    if is_root and t != JSON_LEFT_BRACE:
        raise Exception('Root must be an object')

    if t == JSON_LEFT_BRACKET:
        return parse_array(tokens[1:])
    elif t == JSON_LEFT_BRACE:
        return parse_object(tokens[1:])
    else:
        return t, tokens[1:]


# 4. Run it
json_str = ""
with open('test1.json', 'r') as f:
    json_str = f.read()

try:
    tokens = JSONLexer(json_str).lex()
    print(tokens)
    res = parse(tokens, True)
    print(res)

except Exception as e:
    print(f"An error occurred: {e}")