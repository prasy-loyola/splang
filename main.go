package main

import (
	"errors"
	"fmt"
	"strconv"
)

type TokenType int32;
const (
    Illegal TokenType = iota
    Number 
    Plus 
    Minus
    Divide
    Subtract
    Dot
    EOF
)

type Token struct {
    tokenType TokenType
    literal string
    pos int
}

type Lexer struct {
    text string
    position int
    tokens []Token
}

func tokenize(lexer *Lexer) error {
    if lexer.position >= len(lexer.text) {
        return nil
    }
    char := lexer.text[lexer.position]
    if char == 0 {
        token := Token {
            tokenType: EOF,
            pos: lexer.position,
            literal: "",
        }
        lexer.tokens = append(lexer.tokens, token)
        return nil
    } else if char == ' ' {
        lexer.position++
    } else if char >= '0' && char <= '9' {
        token := Token {
            tokenType: Number,
            pos: lexer.position,
            literal: "",
        }
        for ; lexer.text[lexer.position] >= '0' && lexer.text[lexer.position] <= '9' ; lexer.position++ {
            char = lexer.text[lexer.position]
            token.literal = token.literal + string(char)
            
        }

        lexer.tokens = append(lexer.tokens, token)
    } else if char == '+' {
        token := Token {
            tokenType: Plus,
            pos: lexer.position,
            literal: "+",
        }
        lexer.tokens = append(lexer.tokens, token)
        lexer.position++
    } else if char == '.' {
        token := Token {
            tokenType: Dot,
            pos: lexer.position,
            literal: ".",
        }
        lexer.tokens = append(lexer.tokens, token)
        lexer.position++
    } else {
        token := Token {
            tokenType: Illegal,
            pos: lexer.position,
            literal: string(char),
        }
        lexer.tokens = append(lexer.tokens, token)
        return errors.New("Illegal token " + string(char)) 
    }
    //fmt.Println(lexer)
    return tokenize(lexer)
}

type Interpreter struct {
    stack []int
}

func (i *Interpreter) interpret(lexer *Lexer) error {


    for _ , token := range lexer.tokens {

        if token.tokenType == Number {
            num, err := strconv.Atoi(token.literal)
            if err != nil {
                fmt.Println(err)
                return err
            }
            i.stack = append(i.stack, num)
        } else if token.tokenType == Plus {

            if len(i.stack) < 2 {
                errors.New("Too little items on the stack. Need at least two for plus operator")
            }

            a , b := i.stack[len(i.stack)-1] , i.stack[len(i.stack)-2]
            i.stack = i.stack[:len(i.stack)-2]
            i.stack = append(i.stack, a + b)
        } else if token.tokenType == Dot {

            if len(i.stack) < 1 {
                errors.New("Too little items on the stack. Need at least two for plus operator")
            }
            a := i.stack[len(i.stack)-1]
            i.stack = i.stack[:len(i.stack)-1]
            fmt.Println(a)
        } else {

            fmt.Println(token)

        }

    }
    return nil

}

func main() {
    lexer := Lexer { 
        text : "10 20 + 100 + ." ,
        position : 0,
        tokens: []Token{},
    }

    err := tokenize(&lexer)
    if err != nil {
        fmt.Println("inside error",lexer)
        fmt.Printf("err: %v\n", err)
        return
    }

    interpreter := Interpreter {
        stack : []int{},
    }
    err = interpreter.interpret(&lexer)

    if err != nil {
        fmt.Println("inside error",lexer)
        fmt.Printf("err: %v\n", err)
        return
    }

    if len(interpreter.stack) > 0 {
        fmt.Println("ERROR: elements still left in stack", interpreter.stack)
    }
}
