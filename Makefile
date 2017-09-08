CC	= g++
CFLAGS = -g `pkg-config --cflags opencv` 
TARGET1	= rgb_hsi
OBJS1	= rgb_hsi.o
TARGET2	= imgStats
OBJS2	= imgStats.o
TARGET3	= mydft
OBJS3	= mydft.o
TARGET4	= sat_int_tb
OBJS4	= sat_int_tb.o

LIBS	= `pkg-config --libs opencv`

all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) 

$(TARGET1): $(OBJS1)
	$(CC) -o $@ $(OBJS1) $(LIBS)

$(TARGET2): $(OBJS2)
	$(CC) -o $@ $(OBJS2) $(LIBS)
	
$(TARGET3): $(OBJS3)
	$(CC) -o $@ $(OBJS3) $(LIBS)

$(TARGET4): $(OBJS4)
	$(CC) -o $@ $(OBJS4) $(LIBS)

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	/bin/rm -f core *.o $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)
