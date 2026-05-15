CC = gcc
CFLAGS = -Wall -g

TARGETS = BiTree SqList SqStack DulNode LNode QNode

all: $(TARGETS)

BiTree: BiTree.c
	$(CC) $(CFLAGS) -o $@ $<

SqList: SqList.c
	$(CC) $(CFLAGS) -o $@ $<

SqStack: SqStack.c
	$(CC) $(CFLAGS) -o $@ $<

DulNode: DulNode.c
	$(CC) $(CFLAGS) -o $@ $<

LNode: LNode.c
	$(CC) $(CFLAGS) -o $@ $<

QNode: QNode.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
