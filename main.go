package main

import (
	"errors"
	"fmt"
)

type TokenType int32;
const (
    Illegal TokenType = iota
    Number 
    Plus 
    Minus
    Divide
    Subtract
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
        for ; char >= '0' && char <= '9' ; lexer.position++ {
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
    } else {
        token := Token {
            tokenType: Illegal,
            pos: lexer.position,
            literal: string(char),
        }
        lexer.tokens = append(lexer.tokens, token)
        return errors.New("Illegal token") 
    }
    fmt.Println(lexer)
    return tokenize(lexer)
}

func main() {
    lexer := Lexer { 
        text : " 10 20 +" ,
        position : 0,
        tokens: []Token{},
    }

    err := tokenize(&lexer)
    if err != nil {
        fmt.Println("inside error",lexer)
        fmt.Printf("err: %v\n", err)
    }
}
