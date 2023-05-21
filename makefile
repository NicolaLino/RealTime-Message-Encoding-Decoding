G = gcc -g
O = -o
names = parent child

files:
	$(G) parent.c $(O) parent


clean:
	rm -f $(names)