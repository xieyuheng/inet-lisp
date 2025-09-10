cc = cc
ifeq ($(STATIC), true)
static_ldflags = \
	-static
endif
ifeq ($(TSAN), true)
tsan_ldflags = -fsanitize=thread
tsan_cflags= -fsanitize=thread
endif
ifeq ($(GPROF), true)
gprof_ldflags = -pg
gprof_cflags= -pg
endif
ldflags = \
	-L/usr/local/lib \
	-lm \
	-pthread \
	$(static_ldflags) \
	$(tsan_ldflags) \
	$(gprof_ldflags) \
	$(LDFLAGS)
cflags = \
	-g \
	-O3 \
	-std=c2x \
	-I/usr/local/include \
	-Wall \
	-Wwrite-strings \
	-Wextra \
	-Werror \
	-Wpedantic \
	-D_POSIX_C_SOURCE=200809L \
	-D_TIME_BITS=64 \
	-D_FILE_OFFSET_BITS=64 \
	$(tsan_cflags) \
	$(gprof_cflags) \
	$(CFLAGS)
src = $(shell find src -name '*.c')
headers = $(shell find src -name '*.h')
lib = $(patsubst src/%, lib/%, $(patsubst %.c, %.o, $(src)))
app = inet-lisp
bin = bin/$(app)

.PHONY: all run test-packages test-self run-examples test static tsan gprof clean

all: bin/$(app)

run: bin/$(app)
	./bin/$(app)

test-packages: bin/$(app)
	./bin/$(app) test-packages

test-self: bin/$(app)
	./bin/$(app) test-self

run-examples: bin/$(app)
	sh scripts/run-examples.sh && sh scripts/run-examples-st.sh

test: test-packages test-self run-examples

static:
	make clean && STATIC=true make -j

tsan:
	make clean && TSAN=true make -j

gprof:
	make clean && GPROF=true make -j

bin/$(app): $(lib)
	mkdir -p $(dir $@); $(cc) $^ $(ldflags) -o $@

lib/%.o: src/%.c $(headers)
	mkdir -p $(dir $@); $(cc) -c $(cflags) $< -o $@

clean:
	rm -rf lib bin
