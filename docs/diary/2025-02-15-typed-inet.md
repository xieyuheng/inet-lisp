---
title: typed inet
date: 2025-02-15
---

```scheme
(define-type list-t
  (element-t type-t)
  (value! type-t))
```

假设 define-type 所定义的是一种特殊的 node，
type node 和自身反应会消除自身并且形成 unification，
就像 interaction combinators 和自身反应一样。
那么类型检查就可以理解为在定义 rule 的时候，
构造网络并且让网络反应，来获得检查的结果。

```scheme
(define-node null
  (value! (list-t :A)))

(define-node cons
  (head :A)
  (tail (list-t :A))
  (value! (list-t :A)))

(define-node append
  (target! (list-t :A))
  (rest (list-t :A))
  (result (list-t :A)))
```

`define-rule` 不变：

```scheme
(define-rule (append (null) rest result)
  (connect rest result))

(define-rule (append (cons head tail) rest result)
  (cons head (append tail rest) result))

(define-node sole value!)

(inspect-run
 (append
  (cons (sole) (cons (sole) (cons (sole) (null))))
  (cons (sole) (cons (sole) (cons (sole) (null))))))
```
