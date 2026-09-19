truth_table: truth_table.o validate.o storage.o eval.o intervals.o test.o
	cc -o truth_table truth_table.o eval.o intervals.o storage.o validate.o test.o

truth_table.o: truth_table.c eval.h intervals.h storage.h validate.h test.h
	cc -c truth_table.c

validate.o: validate.c

storage.o: storage.c eval.h storage.h
	cc -c storage.c

eval.o: eval.c eval.h storage.h
	cc -c eval.c

intervals.o: intervals.c
	cc -c intervals.c

test.o: test.c validate.h storage.h eval.h intervals.h
	cc -c test.c
