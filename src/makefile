CC = g++
CFLAGS = -std=c++11 #-DNDEBUG
OBJS =  EZFile.o EZTime.o EZLog.o EZMath.o EZNet.o EZString.o
TARGET = libez.a
RM = rm -f

$(TARGET):$(OBJS)
	# $(CC) -g -o $(TARGET) $(OBJS) $(CFLAGS)
	$(AR) -r $@ $^

$(OBJS):%o:%cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(TARGET) $(OBJS)
