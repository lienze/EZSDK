CC = g++
CFLAGS = -std=c++11 #-DNDEBUG
OBJS = EZSDK.o EZFile.o
TARGET = EZSDK
RM = rm -f

$(TARGET):$(OBJS)
	$(CC) -g -o $(TARGET) $(OBJS) $(CFLAGS)

$(OBJS):%o:%cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(TARGET) $(OBJS)
