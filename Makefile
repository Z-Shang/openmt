MAIN = src/main.c \
	   src/openmt.a

openmt: src/main.c src/openmt.a
	$(CC) `ecl-config --cflags` -o $@ $(MAIN) \
	      `ecl-config --ldflags` -lecl

src/openmt.a: src/openmt.lisp
	ecl -norc \
	    -eval '(require :asdf)' \
	    -eval '(push "./src/" asdf:*central-registry*)' \
	    -eval '(asdf:make-build :openmt :type :static-library :move-here "./src/")' \
	    -eval '(quit)'

clean:
	-rm -f src/openmt.a openmt
