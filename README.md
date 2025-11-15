# inet-lisp

Moore's Law is broken.

![Figure 6.11: MIPS/Clock-Frequency Trend for Intel CPUs](assets/images/mips-clock-frequency-trend-for-intel-cpus.png)

(Figure 6.11: MIPS/Clock-Frequency Trend for Intel CPUs, from ["Is Parallel Programming Hard, And, If So, What Can You Do About It?"](https://www.kernel.org/pub/linux/kernel/people/paulmck/perfbook/perfbook.html), by Paul E. McKenney)

The exponential increase in single-threaded performance halted in about 2003.
Therefore, increasing performance will increasingly require parallelism.

How do we program a machine with many cores?

One possible solution is the graph-based computation model
-- [interaction nets](https://en.wikipedia.org/wiki/Interaction_nets) --
where a program in this computation model can automatically
make use of any number cores in the machine.

This project is an implementation of interaction nets
as a [lisp-like language](https://en.wikipedia.org/wiki/lisp_(programming_language)).

## Syntax

```
(define-node <name> <port-name> ...)
(define-rule <pattern> <exp> ...)
(define-rule* (<pattern> ...) <exp> ...)
(define <name> <exp>)
(define (<name> <arg-name> ...) <exp> ...)
```

## Examples

### Natural Number

Define three nodes `(zero)`, `(add1)` and `(add)`:

```
(define-node zero value!)
(define-node add1 prev value!)
(define-node add target! addend result)
```

```
value!   value!        result
  |        |             |
(zero)   (add1)        (add)
           |           /   \
          prev    target!  addend
```

The rule between `(add1)` and `(add)`:

```
(define-rule (add (add1 prev) addend result)
  (add1 (add prev addend) result))
```

```
     result            result           result
       |                 |                |
     (add)      =>                =>    (add1)
     /   \                 \              |
(add1)   addend           addend        (add)
   |                 |                  /   \
 prev              prev              prev   addend
```

The rule between `(zero)` and `(add)`:

```
(define-rule (add (zero) addend result)
  (connect addend result))
```

```
     result         result        result
       |              |             |
     (add)     =>             =>    |
     /   \              \            \
(zero)   addend        addend       addend
```

Example interaction:

```
       |                   |                   |              |
     (add)               (add1)              (add1)         (add1)
     /   \                 |                   |              |
(add1)    (add1)         (add)               (add1)         (add1)
   |        |    =>      /   \       =>        |        =>    |
(add1)    (add1)    (add1)    (add1)         (add)          (add1)
   |        |          |        |            /   \            |
(zero)    (zero)    (zero)    (add1)    (zero)   (add1)     (add1)
                                |                  |          |
                              (zero)             (add1)     (zero)
                                                   |
                                                 (zero)
```

The whole program with test:

```
(define-node zero value!)
(define-node add1 prev value!)
(define-node add target! addend result)

(define-rule (add (zero) addend result)
  (connect addend result))

(define-rule (add (add1 prev) addend result)
  (add1 (add prev addend) result))

(define (two) (add1 (add1 (zero))))

(add (two) (two))
```

### List

```
(define-node null value!)
(define-node cons head tail value!)
(define-node append target! rest result)

(define-rule (append (null) rest result)
  (connect rest result))

(define-rule (append (cons head tail) rest result)
  (cons head (append tail rest) result))

(define-node sole value!)

(append
 (cons (sole) (cons (sole) (cons (sole) (null))))
 (cons (sole) (cons (sole) (cons (sole) (null)))))
```

### More

For more examples, please see the [examples/](examples/) directory.

## Docs

- [Programming with Interaction Nets](docs/articles/programming-with-interaction-nets.md)
- [反应网编程](docs/articles/反应网编程.md)

## Community

- [Discord / concatenative #inet](https://discord.gg/EcUfwRkbdx)

## Install

Compile:

```
git clone https://github.com/xieyuheng/inet-lisp
cd inet-lisp
make -j
make test
```

The compiled binary `./bin/inet-lisp` is the command-line program.

```sh
$ ./bin/inet-lisp
inet-lisp 0.1.0

commands:
  run -- run files
  info -- print system info
  test-self -- run self test
  test-helpers -- run test for helpers
  version -- print version
  help -- print help message
```

For examples:

```sh
./bin/inet-lisp run examples/readme/nat.test.lisp
```

## Development

```shell
make -j       # compile src/ files to lib/ and bin/
make run      # compile and run the command-line program
make test     # compile and run test
make clean    # clean up compiled files
```

## Implementations

- [inet-cute](https://github.com/xieyuheng/inet-cute)
- [inet-js](https://github.com/xieyuheng/inet-js)
- [inet-forth](https://github.com/xieyuheng/inet-forth)
- [inet-lisp-st](https://github.com/xieyuheng/inet-lisp-st)
- [inet-lisp](https://github.com/xieyuheng/inet-lisp)

## References

**Papers**:

- [1990-interaction-nets](./docs/references/1990-interaction-nets.pdf)
- [1997-interaction-combinators](./docs/references/1997-interaction-combinators.pdf)

**Books**:

- [scalable c](https://github.com/booksbyus/scalable-c)

## License

[GPLv3](LICENSE)
