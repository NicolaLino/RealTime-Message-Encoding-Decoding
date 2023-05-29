G = gcc -g
O = -o
names = parent sender senderChild receiver

files:
	$(G) parent.c $(O) parent
	$(G) sender.c $(O) sender
	$(G) senderChild.c $(O) senderChild
	$(G) receiver.c $(O) receiver
	$(G) spy.c $(O) spy
	$(G) master_spy.c $(O) master_spy

clean:
	rm -f $(names)
