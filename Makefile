# SPDX-License-Identifier: GPL-2.0-or-later
flags = -g -O2 -Wall -Werror -D DEBUG_LIST -I src/
head  = src/list.h
obj   = src/sort.o src/check.o
demo  = examples/benchmark examples/selftest

all: $(demo)

%.o:%.c $(head)
	@ echo -e "  \e[32mCC\e[0m	" $@
	@ gcc -o $@ -c $< $(flags)

$(demo): $(obj) $(addsuffix .c,$(demo))
	@ echo -e "  \e[34mMKELF\e[0m	" $@
	@ gcc -o $@ $@.c $(obj)  $(flags)

clean:
	@ rm -f $(obj) $(demo)
