
let trueFn = (x, y) => x;
let falseFn = (x, y) => y;

let notFn = (f) => f(falseFn, trueFn);
console.log("NOT True", notFn(trueFn));
console.log("NOT False", notFn(falseFn));
let identity = (a) => a;
console.log("IDENTITY True", identity(trueFn));
console.log("IDENTITY False", identity(falseFn));

let andFn = (a, b) => b(a, falseFn);

console.log("AND True True", andFn(trueFn, trueFn));
console.log("AND True False", andFn(trueFn, falseFn));
console.log("AND False True", andFn(falseFn, trueFn));
console.log("AND False False", andFn(falseFn, falseFn));


let orFn = (a, b) => a(trueFn, b)

console.log("OR True True", orFn(trueFn, trueFn));
console.log("OR True False", orFn(trueFn, falseFn));
console.log("OR False True", orFn(falseFn, trueFn));
console.log("OR False False", orFn(falseFn, falseFn));


let nandFn = (a, b) => notFn(andFn(a, b));
console.log("NAND True True", nandFn(trueFn, trueFn));
console.log("NAND True False", nandFn(trueFn, falseFn));
console.log("NAND False True", nandFn(falseFn, trueFn));
console.log("NAND False False", nandFn(falseFn, falseFn));

let nandFn2 = (a, b) => a(b(falseFn, trueFn), trueFn);
console.log("NAND True True", nandFn2(trueFn, trueFn));
console.log("NAND True False", nandFn2(trueFn, falseFn));
console.log("NAND False True", nandFn2(falseFn, trueFn));
console.log("NAND False False", nandFn2(falseFn, falseFn));

let xorFn = (a, b) => a(b(falseFn, trueFn), b(trueFn, falseFn));
console.log("XOR True True", xorFn(trueFn, trueFn));
console.log("XOR True False", xorFn(trueFn, falseFn));
console.log("XOR False True", xorFn(falseFn, trueFn));
console.log("XOR False False", xorFn(falseFn, falseFn));

let xnorFn = (a, b) => a(b(trueFn, falseFn), b(falseFn, trueFn));
console.log("XNOR True True", xnorFn(trueFn, trueFn));
console.log("XNOR True False", xnorFn(trueFn, falseFn));
console.log("XNOR False True", xnorFn(falseFn, trueFn));
console.log("XNOR False False", xnorFn(falseFn, falseFn));

let xnorFn2 = (a, b) => a(b(a, b), b(falseFn, trueFn));
console.log("XNOR True True", xnorFn2(trueFn, trueFn));
console.log("XNOR True False", xnorFn2(trueFn, falseFn));
console.log("XNOR False True", xnorFn2(falseFn, trueFn));
console.log("XNOR False False", xnorFn2(falseFn, falseFn));


