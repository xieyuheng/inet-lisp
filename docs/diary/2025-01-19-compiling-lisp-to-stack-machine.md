---
title: compiling lisp to stack machine
date: 2025-01-19
---

vm opcodes:

```scheme
(get def)
(apply arity)
(literal value)
(local-get index)
(local-set index)
```

compiling:

```scheme
(f (a) (b)) =>
a/0
b/0
f/2

(f (g (a)) (g (b))) =>
a/0
g/1
b/0
g/1
f/2

(f (a) (b)) (=> x y) =>
a/0
b/0
f/2
(local-set (index-of y))
(local-set (index-of x))
```

[2025-01-22]
后缀表达式的 `(=> x y)` 要求 top-level application 要可以返回多个 value。
而 application expression 中每个 sub expression 都必须返回一个 value。

- 这就要求检查 sub expression 是否返回一个 value。
  不检查肯定会引起很难找到的 bug。

方案 A：区分 top-level application。
如果不是 top-level application，就要求必须返回一个值。
- 对于 node 来说，所給 arity 比所声明的 arity 少一。
- 对于 function 来说，所給 arity 和所声明的 arity 一致。
  - 这又假设了所有函数都必须返回一个值。
    因此又需要 infer function body 的 arity。
    所以是不行的。

方案 B：从 arg list 计算 arity。

方案 C：设计特殊的 opcode，在运行时处理 arity。

- 这是最简单的。
