G = gcc -g
O = -o
names = parent sender senderChild

files:
	$(G) parent.c $(O) parent
	$(G) sender.c $(O) sender
	$(G) senderChild.c $(O) senderChild

clean:
	rm -f $(names)
