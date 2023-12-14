
lora_read: lora_read.c log_info.c
	gcc $^ -o $@ -Wall -Wextra -g


.PHONY: clean
	@rm lora_read
