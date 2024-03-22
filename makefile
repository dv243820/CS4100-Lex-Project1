all: scanner cmos run_script

scanner: lex.yy.c
	g++ -o scanner lex.yy.c -lfl

cmos: cmos.cpp
	g++ -o cmos cmos.cpp

lex.yy.c: cmos.l
	flex cmos.l

run_script:
	sudo bash detector.sh Examples/Examples

.PHONY: clean
clean:
	rm -f scanner cmos lex.yy.c *.txt