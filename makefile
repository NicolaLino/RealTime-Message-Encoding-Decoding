G = gcc -g
O = -o
names = parent

files:
	$(G) parent.c $(O) parent

clean:
	rm -f $(names)
