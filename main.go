package main

import (
	"errors"
	"fmt"
	"strconv"
)

type TokenType int32

const (
	Illegal TokenType = iota
	Number
	StringLiteral
	Plus
	Minus
	ForwardSlash
	Asterisk
	Dot
	Dollar
	EOF
	TokenTypeCount
)

type Token struct {
	tokenType TokenType
	literal   string
	pos       int
}

type Lexer struct {
	text     string
	position int
	tokens   []Token
}

func isAlphaCharacter(char byte) bool {
	return (char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z')
}
func isNumber(char byte) bool {
	return char >= '0' && char <= '9'
}
func (lexer *Lexer) tokenize() error {
	if lexer.position >= len(lexer.text) {
		return nil
	}
	char := lexer.text[lexer.position]

	if TokenTypeCount != 10 {
		return errors.New(fmt.Sprint("Expected number of TokenTypes is 10 , but found ", TokenTypeCount))
	}
	if char == 0 {
		token := Token{
			tokenType: EOF,
			pos:       lexer.position,
			literal:   "",
		}
		lexer.tokens = append(lexer.tokens, token)
		return nil
	} else if char == ' ' || char == '\n' || char == '\r' {
		lexer.position++
	} else if isNumber(char) {
		token := Token{
			tokenType: Number,
			pos:       lexer.position,
			literal:   "",
		}
		for ; isNumber(lexer.text[lexer.position]); lexer.position++ {
			char = lexer.text[lexer.position]
			token.literal = token.literal + string(char)
		}

		lexer.tokens = append(lexer.tokens, token)
	} else if isAlphaCharacter(char) {
		token := Token{
			tokenType: StringLiteral,
			pos:       lexer.position,
			literal:   "",
		}
		for ; isAlphaCharacter(lexer.text[lexer.position]); lexer.position++ {
			char = lexer.text[lexer.position]
			token.literal = token.literal + string(char)

		}

		lexer.tokens = append(lexer.tokens, token)
	} else if char == '+' {
		token := Token{
			tokenType: Plus,
			pos:       lexer.position,
			literal:   "+",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else if char == '-' {
		token := Token{
			tokenType: Minus,
			pos:       lexer.position,
			literal:   "-",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else if char == '*' {
		token := Token{
			tokenType: Asterisk,
			pos:       lexer.position,
			literal:   "*",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else if char == '/' {
		token := Token{
			tokenType: ForwardSlash,
			pos:       lexer.position,
			literal:   "/",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else if char == '.' {
		token := Token{
			tokenType: Dot,
			pos:       lexer.position,
			literal:   ".",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else if char == '$' {
		token := Token{
			tokenType: Dollar,
			pos:       lexer.position,
			literal:   "$",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else {
		token := Token{
			tokenType: Illegal,
			pos:       lexer.position,
			literal:   string(char),
		}
		lexer.tokens = append(lexer.tokens, token)
		return errors.New("Illegal token " + string(char))
	}
	//fmt.Println(lexer)
	return lexer.tokenize()
}

type Interpreter struct {
	stack []int
}

func (itp *Interpreter) interpret(lexer *Lexer) error {

	for _, token := range lexer.tokens {

		if TokenTypeCount != 10 {
			return errors.New(fmt.Sprint("Expected number of TokenTypes is 10 , but found ", TokenTypeCount))
		}
		if token.tokenType == Number {
			if num, err := strconv.Atoi(token.literal); err != nil {
				return err
			} else {
				itp.stack = append(itp.stack, num)
			}
		} else if token.tokenType == StringLiteral {

            for i := len(token.literal) - 1 ; i >=0 ; i-- {
				itp.stack = append(itp.stack, int(token.literal[i]))
            }
			itp.stack = append(itp.stack, len(token.literal))
		} else if token.tokenType == Plus {

			if len(itp.stack) < 2 {
				return errors.New("Too little items on the stack. Need at least two for addition")
			}
			a, b := itp.stack[len(itp.stack)-2], itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-2]
			itp.stack = append(itp.stack, a+b)
		} else if token.tokenType == Minus {

			if len(itp.stack) < 2 {
				return errors.New("Too little items on the stack. Need at least two for subtraction")
			}
			a, b := itp.stack[len(itp.stack)-2], itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-2]
			itp.stack = append(itp.stack, a-b)
		} else if token.tokenType == ForwardSlash {
			if len(itp.stack) < 2 {
				return errors.New("Too little items on the stack. Need at least two for division")
			}
			a, b := itp.stack[len(itp.stack)-2], itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-2]
			itp.stack = append(itp.stack, a/b)
		} else if token.tokenType == Asterisk {
			if len(itp.stack) < 2 {
				return errors.New("Too little items on the stack. Need at least two for multiplication")
			}
			a, b := itp.stack[len(itp.stack)-2], itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-2]
			itp.stack = append(itp.stack, a*b)
		} else if token.tokenType == Dot {
			if len(itp.stack) < 1 {
				return errors.New("Too little items on the stack. Need at least one item for print")
			}
			a := itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-1]
			fmt.Println(a)
		} else if token.tokenType == Dollar {
			if len(itp.stack) < 1 {
				return errors.New("Too little items on the stack. Need the number of items to print")
			}
			count := itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-1]

			if len(itp.stack) < count {
				return errors.New(fmt.Sprintf("Expected %d items on stack, but found only %d items", count, len(itp.stack)))

			}
			for i := 0; i < count; i++ {
				num := itp.stack[len(itp.stack)-1]
				itp.stack = itp.stack[:len(itp.stack)-1]
				fmt.Print(string(num))
			}
            fmt.Println()
		} else {
			return errors.New("Unsupported token " + fmt.Sprint(token))
		}

	}
	return nil
}

func main() {
	lexer := Lexer{
		text:     `HelloWorld $ 10 10 * .  percent $ works $`,
		position: 0,
		tokens:   []Token{},
	}

	if err := lexer.tokenize(); err != nil {
		fmt.Println("Error in lexing", lexer)
		fmt.Printf("err: %v\n", err)
		return
	} else {
		interpreter := Interpreter{
			stack: []int{},
		}

		if err = interpreter.interpret(&lexer); err != nil {
			fmt.Println("Error in interpretting", interpreter)
			fmt.Printf("err: %v\n", err)
			return
		}

		if len(interpreter.stack) > 0 {
			fmt.Println("ERROR: elements still left in stack", interpreter.stack)
		}
	}
}
