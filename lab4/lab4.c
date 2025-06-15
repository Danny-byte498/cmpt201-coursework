#define _GNU_SOURCE
#include <stdint.h> // uint64_t
#include <stdio.h>
#include <stdlib.h> // EXIT_FAILURE
#include <unistd.h> // sbrk()
#define BUF_SIZE 128

void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE); // exit with failure status
}
void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                                       : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

struct header {
  uint64_t size;       // current block size
  struct header *next; // next block size
};

int main() {
  void *total_blocks = sbrk(256); // sbrk for increasing heap size by 256 bytes,
                                  // also pointing to start of this memory
  struct header *first_block =
      (struct header *)total_blocks; // points to start of memory
  struct header *second_block =
      (struct header *)((char *)total_blocks +
                        128); // points to start of memory + 128 byte(top of
                              // first_block)
  first_block->size =
      128; // block size 128 bytes (112 + 16(8+8 in struct header))
  first_block->next = NULL; // first block points to NULL

  second_block->size =
      128; // block size 128 bytes (112 + 16(8+8 in struct header))
  second_block->next = first_block; // second block points back to first block

  // Get pointer to data area (after 16 byte header)
  char *first_data = (char *)first_block + sizeof(struct header);

  // Fill first block with zeros as lab said
  for (int i = 0; i < 128 - (int)sizeof(struct header); i++) {
    first_data[i] = 0;
  }

  // Get pointer to data area (after 16 byte header)
  char *second_data = (char *)second_block + sizeof(struct header);

  // Fill second block with ones as lab said
  for (int i = 0; i < 128 - (int)sizeof(struct header); i++) {
    second_data[i] = 1;
  }
  printf("first block:       %p\n", (void *)first_block);
  printf("second block:      %p\n", (void *)second_block);

  print_out("First block size: %lu\n", &first_block->size, sizeof(uint64_t));
  print_out("Second block size: %lu\n", &second_block->size, sizeof(uint64_t));

  print_out("First block next: %p\n", &first_block->next, sizeof(void *));
  print_out("Second block next: %p\n", &second_block->next, sizeof(void *));

  // Print all 112 bytes of first block's data
  for (int i = 0; i < 128 - (int)sizeof(struct header); i++) {
    printf("%d\n", first_data[i]);
  }

  // Print all 112 bytes of second block's data
  for (int i = 0; i < 128 - (int)sizeof(struct header); i++) {
    printf("%d\n", second_data[i]);
  }
  return 0;
}
