# inet-lisp

This project is a **single-threaded** implementation of
[interaction nets](https://en.wikipedia.org/wiki/Interaction_nets)
as a [lisp-like language](https://en.wikipedia.org/wiki/lisp_(programming_language)).

Single-threaded environment allow us
to support multiple principal ports,
with which rules can be expressed by subgraph matching.

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
value!   value!        value
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
     value             value            value
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
     value          value         value
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

(inspect-run (add (two) (two)))
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

(inspect-run
  (append
    (cons (sole) (cons (sole) (cons (sole) (null))))
    (cons (sole) (cons (sole) (cons (sole) (null))))))
```

### More

For more examples, please see the [examples/](examples/) directory.

## Docs

- [Programming with Interaction Nets](docs/articles/programming-with-interaction-nets.md)
- [反应网编程](docs/articles/反应网编程.md)

## Community

- [Discord / concatenative #inet](https://discord.gg/EcUfwRkbdx)

## Install

Dependencies:

- debian/ubuntu: `sudo apt install libx11-dev`

Compile:

```
git clone https://github.com/cicada-lang/inet-lisp
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
  play -- play files
  info -- print system info
  test-self -- run self test
  test-modules -- run test for modules
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

Using [tsan (ThreadSanitizer)](https://github.com/google/sanitizers/wiki/threadsanitizercppmanual)
to test data race in parallel program:

```shell
make clean && TSAN=true make -j
```

## Implementations

- [inet-cute](https://github.com/cicada-lang/inet-cute)
- [inet-js](https://github.com/cicada-lang/inet-js)
- [inet-forth](https://github.com/cicada-lang/inet-forth)
- [inet-lisp](https://github.com/cicada-lang/inet-lisp)

## References

**Papers**:

- [1990-interaction-nets](./docs/references/1990-interaction-nets.pdf)
- [1997-interaction-combinators](./docs/references/1997-interaction-combinators.pdf)

**Books**:

- [scalable c](https://github.com/booksbyus/scalable-c)

## Contributions

To make a contribution, fork this project and create a pull request.

Please read the [STYLE-GUIDE.md](STYLE-GUIDE.md) before you change the code.

Remember to add yourself to [AUTHORS](AUTHORS).
Your line belongs to you, you can write a little
introduction to yourself but not too long.

## License

[GPLv3](LICENSE)
