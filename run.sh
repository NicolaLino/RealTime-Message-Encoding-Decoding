#!/bin/sh

# This file only to run the project quickly for testing not for final production



rm -f parent
rm -f sender
rm -f senderChild
gcc -g senderChild.c -o senderChild
gcc -g sender.c -o sender
gcc -g parent.c -o parent


./parent