all: lora_read directories

.PHONY: clean all directories

lora_read: lora_read.c log_info.c
	gcc $^ -o $@ -Wall -Wextra -g

OUTPUT_DIR := ramki

directories:
	@mkdir -p $(OUTPUT_DIR)

clean:
	@rm lora_read
	@rm -rf $(OUTPUT_DIR)
