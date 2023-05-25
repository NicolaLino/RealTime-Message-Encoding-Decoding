G = gcc -g
O = -o
names = parent sender senderChild receiver

files:
	$(G) parent.c $(O) parent
	$(G) sender.c $(O) sender
	$(G) senderChild.c $(O) senderChild
	$(G) receiver.c $(O) receiver

clean:
	rm -f $(names)
