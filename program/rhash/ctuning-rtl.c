/*
#
# Collective Tuning Run-Time
# Part of Collective Mind
#
# See cM LICENSE.txt for licensing details.
# See cM Copyright.txt for copyright details.
#
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef OPENME
#include <openme.h>
#endif
#ifdef XOPENME
#include <xopenme.h>
#endif

#include <string.h>
#include <assert.h>
#include "sha3.h"

unsigned char* read_file(FILE* fp, long* buf_size) {
  fseek(fp, 0L, SEEK_END);

  long lSize = ftell(fp);
  rewind(fp);

  /* allocate memory for entire content */
  unsigned char* buffer = (unsigned char*)calloc(1, lSize+1);
  if (!buffer) {
    return NULL;
  }

  if (1 != fread(buffer, lSize, 1, fp)) {
    free(buffer);
    return NULL;
  }
  *buf_size = lSize;
  return buffer;
}

/**
* Convert a byte to a hexadecimal number. The result, consisting of two
* hexadecimal digits is stored into a buffer.
 *
 * @param dest  the buffer to receive two symbols of hex representation
 * @param byte the byte to decode
 * @param upper_case flag to print string in uppercase
 * @return pointer to the chararcter just after the written number (dest + 2)
 */
char* rhash_print_hex_byte(char *dest, const unsigned char byte, int upper_case)
{
  const char add = (upper_case ? 'A' - 10 : 'a' - 10);
  unsigned char c = (byte >> 4) & 15;
  *dest++ = (c > 9 ? c + add : c + '0');
  c = byte & 15;
  *dest++ = (c > 9 ? c + add : c + '0');
  return dest;
}

/**
 * Store hexadecimal representation of a binary string to given buffer.
 *
 * @param dest the buffer to receive hexadecimal representation
 * @param src binary string
 * @param len string length
 * @param upper_case flag to print string in uppercase
 */
void rhash_byte_to_hex(char *dest, const unsigned char *src, unsigned len, int upper_case)
{
  while (len-- > 0) {
    dest = rhash_print_hex_byte(dest, *src++, upper_case);
  }
  *dest = '\0';
}

int main(int argc, char* argv[]) {
  long ct_repeat=0;
  long ct_repeat_max=1;
  int ct_return=0;

  char* source_file;
  FILE* source_fp = NULL;
  unsigned char* buf = NULL;
  long buf_size = 0;

#ifdef OPENME
  openme_init(NULL,NULL,NULL,0);
  openme_callback("PROGRAM_START", NULL);
#endif
#ifdef XOPENME
  xopenme_init(1,0);
#endif

  if (getenv("CT_REPEAT_MAIN")!=NULL) ct_repeat_max=atol(getenv("CT_REPEAT_MAIN"));
              
  if (argc != 2) {
    fputs("usage: < source >\n", stderr);
    ct_return = 1;
    goto program_end;
  }
  source_file = argv[1];

  source_fp = fopen(source_file, "rb");
  if (NULL == source_fp) {
    fputs("Failed to open source file\n", stderr);
    ct_return = 1;
    goto program_end;
  }

  buf = read_file(source_fp, &buf_size);
  if (NULL == buf) {
    fputs("Failed to read source file (probably, it's too big to fit in memory or inaccessible)\n", stderr);
    ct_return = 1;
    goto program_end;
  }

  fclose(source_fp);
  source_fp = NULL;

  sha3_ctx ctx;

#ifdef OPENME
  openme_callback("KERNEL_START", NULL);
#endif
#ifdef XOPENME
  xopenme_clock_start(0);
#endif

  for (ct_repeat=0; ct_repeat<ct_repeat_max; ct_repeat++) {
    rhash_sha3_512_init(&ctx);
    rhash_sha3_update(&ctx, buf, buf_size);
    rhash_sha3_final(&ctx, 0);
  }
  ct_return = 0;

kernel_end:

#ifdef XOPENME
  xopenme_clock_end(0);
#endif
#ifdef OPENME
  openme_callback("KERNEL_END", NULL);
#endif

  unsigned char result[200];
  rhash_sha3_final(&ctx, result);
  char output[200];
  rhash_byte_to_hex(output, result, 64, 0);
  printf("%s\n", output);
    
program_end:

  if (NULL != source_fp) {
    fclose(source_fp);
  }
  if (NULL != buf) {
    free(buf);
  }

#ifdef XOPENME
  xopenme_dump_state();
  xopenme_finish();
#endif
#ifdef OPENME
  openme_callback("PROGRAM_END", NULL);
#endif

  return ct_return;
}
