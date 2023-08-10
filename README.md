# splang
[S]imple [P]ostfix toy [Lang]uage 


## Language reference

`0-9+` digits - push number to stack

`a-zA-Z|\_+` characters - push the ascii code of characters, len to the stack

`|` - placeholder for new line

`\_` - placeholder for space


`+` - pop two numbers on top of the stack and push the sum to the stack

`-` - pop two numbers on top of the stack and push the difference to the stack

`*` - pop two numbers on top of the stack and push the product to the stack

`/` - pop two numbers on top of the stack and push the divident to the stack

`@` - duplicate the item on top of the stack

`.` - pop the number on top of the stack and print it 

`$` - pop the (chars..., length) from the stack and print the chars

`=` - pop the number on top of stack and jumps to the matching label if its zero

`!` - label for the matching conditional jump

`\#` - unconditional jump to the matching conditional jump


