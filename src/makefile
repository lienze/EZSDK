CC = g++
CFLAGS = #-DNDEBUG
OBJS = EZSDK.o
TARGET = EZSDK
RM = rm -f

$(TARGET):$(OBJS)
	$(CC) -g -o $(TARGET) $(OBJS) $(CFLAGS)

$(OBJS):%o:%cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(TARGET) $(OBJS)
