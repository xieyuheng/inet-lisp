---
title: different from scheme
date: 2025-01-13
---

与 scheme 的不同点：

- application 的参数个数不够时，自动处理成 multiple-return-value。
- `let` 可以绑定 multiple-return-value。

如果没有 inet 构造 graph 的需求，
application 的参数个数不够时可能应该处理成 lambda 的 curry。

还好 scheme 本身没有自动的 curry，
所以目前新增的语义与 scheme 也不冲突。

问题：如何处理 closure 和 lambda？

方案 A：不支持 closure，因此也没有 lambda。

方案 B：支持 closure，但是需要明显给出所捕获的 linear variable 列表。

这种 closure 只能用一次，不能被复制，所以不能被传递到 list-map 函数中。

只有用 linear type 才能理解清楚：

```scheme
(claim list-map
  (! (forall ([A (! type-t)] [B (! type-t)])
       (-o (list-t A) (! (-o A B)) (list-t B)))))
```

在 inet-lisp 中，只要先不去实现 closure 就可以避免 linear variable 的问题。

此时：
- 所有顶层的定义都是带有 `!` 的。
- 所有局部变量都是 linear 的。
