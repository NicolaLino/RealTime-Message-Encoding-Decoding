#!/bin/sh

# This file only to run the project quickly for testing not for final production

#run chmod 777 run.sh to make it executable

rm -f parent
rm -f sender
rm -f senderChild
rm -f receiver
rm -f helper
gcc -g receiver.c -o receiver
gcc -g helper.c -o helper
gcc -g senderChild.c -o senderChild
gcc -g sender.c -o sender
gcc -g parent.c -o parent

./parent
