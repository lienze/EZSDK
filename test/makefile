CC = g++
CFLAGS = -std=c++11#-DNDEBUG
OBJS = TestFile.o
TARGET = TestFile
RM = rm -f
LIB_PATH = -L ../src
LIB_INC = -I ../src
LIBS = -lez

# link
$(TARGET):$(OBJS)
	$(CC) -g -o $(TARGET) $(OBJS) $(CFLAGS) $(LIB_PATH) $(LIBS)

# compile
$(OBJS):%o:%cpp
	$(CC) -c $(CFLAGS) $< -o $@ $(LIB_INC)

clean:
	$(RM) $(TARGET) $(OBJS)

