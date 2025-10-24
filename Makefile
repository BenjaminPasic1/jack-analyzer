CC = gcc
CFLAGS = -Wall -g
TARGET = final.c
SOURCES = main.c src/jack_tokenizer.c src/hashmap.c src/parser/jack_parser.c src/parser/compile_class.c src/parser/compile_class_var_dec.c src/util.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(TARGET)
