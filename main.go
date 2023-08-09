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
	typ TokenType
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
			typ: EOF,
			pos:       lexer.position,
			literal:   "",
		}
		lexer.tokens = append(lexer.tokens, token)
		return nil
	} else if char == ' ' || char == '\n' || char == '\r' {
		lexer.position++
	} else if isNumber(char) {
		token := Token{
			typ: Number,
			pos:       lexer.position,
			literal:   "",
		}
		for ; lexer.position < len(lexer.text) && isNumber(lexer.text[lexer.position]); lexer.position++ {
			char = lexer.text[lexer.position]
			token.literal = token.literal + string(char)
		}

		lexer.tokens = append(lexer.tokens, token)
	} else if isAlphaCharacter(char) {
		token := Token{
			typ: StringLiteral,
			pos:       lexer.position,
			literal:   "",
		}
		for ; lexer.position < len(lexer.text) && isAlphaCharacter(lexer.text[lexer.position]); lexer.position++ {
			char = lexer.text[lexer.position]
			token.literal = token.literal + string(char)

		}

		lexer.tokens = append(lexer.tokens, token)
	} else if char == '+' {
		token := Token{
			typ: Plus,
			pos:       lexer.position,
			literal:   "+",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else if char == '-' {
		token := Token{
			typ: Minus,
			pos:       lexer.position,
			literal:   "-",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else if char == '*' {
		token := Token{
			typ: Asterisk,
			pos:       lexer.position,
			literal:   "*",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else if char == '/' {
		token := Token{
			typ: ForwardSlash,
			pos:       lexer.position,
			literal:   "/",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else if char == '.' {
		token := Token{
			typ: Dot,
			pos:       lexer.position,
			literal:   ".",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else if char == '$' {
		token := Token{
			typ: Dollar,
			pos:       lexer.position,
			literal:   "$",
		}
		lexer.tokens = append(lexer.tokens, token)
		lexer.position++
	} else {
		token := Token{
			typ: Illegal,
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

func (itp *Interpreter) interpret(instructions *[]Instruction) error {

	for _, instr := range *instructions {

		if TokenTypeCount != 10 {
			return errors.New(fmt.Sprint("Expected number of TokenTypes is 10 , but found ", TokenTypeCount))
		}
		if instr.typ == PushInt {
			itp.stack = append(itp.stack, instr.operand)
		} else if instr.typ == IntrinsicPlus {
			if len(itp.stack) < 2 {
				return errors.New("Too little items on the stack. Need at least two for addition")
			}
			a, b := itp.stack[len(itp.stack)-2], itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-2]
			itp.stack = append(itp.stack, a+b)
		} else if instr.typ == IntrinsicSubract {

			if len(itp.stack) < 2 {
				return errors.New("Too little items on the stack. Need at least two for subtraction")
			}
			a, b := itp.stack[len(itp.stack)-2], itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-2]
			itp.stack = append(itp.stack, a-b)
		} else if instr.typ == IntrinsicDivide {
			if len(itp.stack) < 2 {
				return errors.New("Too little items on the stack. Need at least two for division")
			}
			a, b := itp.stack[len(itp.stack)-2], itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-2]
			itp.stack = append(itp.stack, a/b)
		} else if instr.typ == IntrinsicMultiplication {
			if len(itp.stack) < 2 {
				return errors.New("Too little items on the stack. Need at least two for multiplication")
			}
			a, b := itp.stack[len(itp.stack)-2], itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-2]
			itp.stack = append(itp.stack, a*b)
		} else if instr.typ == IntrinsicPrintInt {
			if len(itp.stack) < 1 {
				return errors.New("Too little items on the stack. Need at least one item for print")
			}
			a := itp.stack[len(itp.stack)-1]
			itp.stack = itp.stack[:len(itp.stack)-1]
			fmt.Println(a)
		} else if instr.typ == IntrinsicPrintStr {
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
			return errors.New("Unsupported token " + fmt.Sprint(instr))
		}

	}
	return nil
}

type InstructionType int 

const (

    IntrinsicPlus InstructionType = iota
    IntrinsicSubract
    IntrinsicDivide
    IntrinsicMultiplication
    IntrinsicPrintInt
    IntrinsicPrintStr

    PushInt

    InstructionTypeCount
)

type Instruction struct {
    typ InstructionType
    operand int
}

type Parser struct {
    instructions []Instruction
}


func (parser *Parser) parse(lexer *Lexer) error {
	for _, token := range lexer.tokens {
        
		if TokenTypeCount != 10 {
			return errors.New(fmt.Sprint("Expected number of TokenTypes is 10 , but found ", TokenTypeCount))
		}
		if token.typ == Number {
			if num, err := strconv.Atoi(token.literal); err != nil {
				return err
			} else {
                parser.instructions = append(parser.instructions, Instruction{ typ : PushInt,  operand: num} )
			}
		} else if token.typ == StringLiteral {
            for i := len(token.literal) - 1 ; i >=0 ; i-- {
                parser.instructions = append(parser.instructions, Instruction{ typ : PushInt,  operand: int(token.literal[i])} )
            }
            parser.instructions = append(parser.instructions, Instruction{ typ : PushInt,  operand: len(token.literal)} )
		} else if token.typ == Plus {
            parser.instructions = append(parser.instructions, Instruction{ typ : IntrinsicPlus,  operand: 0} )
		} else if token.typ == Minus {
            parser.instructions = append(parser.instructions, Instruction{ typ : IntrinsicSubract,  operand: 0} )
		} else if token.typ == ForwardSlash {
            parser.instructions = append(parser.instructions, Instruction{ typ : IntrinsicDivide,  operand: 0} )
		} else if token.typ == Asterisk {
            parser.instructions = append(parser.instructions, Instruction{ typ : IntrinsicMultiplication,  operand: 0} )
		} else if token.typ == Dot {
            parser.instructions = append(parser.instructions, Instruction{ typ : IntrinsicPrintInt,  operand: 0} )
		} else if token.typ == Dollar {
            parser.instructions = append(parser.instructions, Instruction{ typ : IntrinsicPrintStr,  operand: 0} )
		} else {
			return errors.New("Unsupported token " + fmt.Sprint(token))
		}

	}

    return nil

}

func main() {
	lexer := Lexer{
		text:     `HelloWorld $ 10 10 * . percent $ works $`,
		position: 0,
		tokens:   []Token{},
	}

	if err := lexer.tokenize(); err != nil {
		fmt.Println("Error in lexing", lexer)
		fmt.Printf("err: %v\n", err)
		return
	} else {
		parser := Parser{
			instructions: []Instruction{},
		}
		if err = parser.parse(&lexer); err != nil {
			fmt.Println("Error in parsing", parser)
			fmt.Printf("err: %v\n", err)
			return
		}

		interpreter := Interpreter{
			stack: []int{},
		}

		if err = interpreter.interpret(&parser.instructions); err != nil {
			fmt.Println("Error in interpretting", interpreter)
			fmt.Printf("err: %v\n", err)
			return
		}

		if len(interpreter.stack) > 0 {
			fmt.Println("ERROR: elements still left in stack", interpreter.stack)
		}
	}
}
