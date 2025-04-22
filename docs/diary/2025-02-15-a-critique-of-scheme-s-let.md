---
title: a critique of scheme's let
date: 2025-02-15
---

我认为 scheme 的 `let` 应该模仿 javascript：

```scheme
(define (main)
  (g a)
  (let x (f b))
  (j x)
  (let y (h x))
  (k y))
```

而不是为了让 `(let)` 能独立形成表达式而设计成：

```scheme
(define (main)
  (g a)
  (let ((x (f b)))
    (j x)
    (let ((y (h x)))
      (k y))))
```

需要表达式的时候可以用 `(begin)`：

```scheme
(begin
  (g a)
  (let x (f b))
  (j x)
  (let y (h x))
  (k y))
```

模仿 javascript 的缺点是函数体内部必须是 stmt 而不是 exp。

但是，如果函数体内部可以有多个 exp，
前面的带有副作用，返回最后一个，
那么就和区分 stmt 和 exp 没什么区别了。
