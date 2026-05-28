CC = gcc
CFLAGS = -Wall -g

TARGETS = BiTree SqList SqStack DulNode LNode QNode String \
          HFTree ThreadBiTree TreeForest HashTable Sort Search Graph

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

String: String.c
	$(CC) $(CFLAGS) -o $@ $<

HFTree: HFTree.c
	$(CC) $(CFLAGS) -o $@ $<

ThreadBiTree: ThreadBiTree.c
	$(CC) $(CFLAGS) -o $@ $<

TreeForest: TreeForest.c
	$(CC) $(CFLAGS) -o $@ $<

HashTable: HashTable.c
	$(CC) $(CFLAGS) -o $@ $<

Sort: Sort.c
	$(CC) $(CFLAGS) -o $@ $<

Search: Search.c
	$(CC) $(CFLAGS) -o $@ $<

Graph: Graph.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
