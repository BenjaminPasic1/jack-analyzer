CC = gcc
CFLAGS = -Wall -g
TARGET = final.c
SOURCES = main.c src/jack_tokenizer.c src/hashmap.c src/jack_parser.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(TARGET)
