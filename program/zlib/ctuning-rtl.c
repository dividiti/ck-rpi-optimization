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
#include "zlib.h"

#define CHUNK 16384

/* Compress from file source to file dest until EOF on source.
   def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
int def(unsigned char* buf, long buf_size, long* foo)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK];
    int level = Z_DEFAULT_COMPRESSION;

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    long pos = 0;
    do {
        strm.avail_in = pos + CHUNK < buf_size ? CHUNK : (uInt)(buf_size - pos);
        strm.next_in = buf + pos;
        pos += strm.avail_in;
        flush = pos >= buf_size ? Z_FINISH : Z_NO_FLUSH;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            *foo += CHUNK - strm.avail_out; 
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

/* Decompress from file source to file dest until stream ends or EOF.
   inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_DATA_ERROR if the deflate data is
   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
   the version of the library linked do not match, or Z_ERRNO if there
   is an error reading or writing the files. */
int inf(unsigned char* buf, long buf_size, long* foo)
{
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    long pos = 0;
    do {
        strm.avail_in = pos + CHUNK < buf_size ? CHUNK : (uInt)(buf_size - pos);
        if (strm.avail_in == 0)
            break;
        strm.next_in = buf + pos;
        pos += strm.avail_in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            *foo += CHUNK - strm.avail_out; 
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

/* report a zlib or i/o error */
void zerr(int ret)
{
    switch (ret) {
    case Z_ERRNO:
        fputs("error reading or writing\n", stderr);
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
}

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

int main(int argc, char* argv[])
{
  long ct_repeat=0;
  long ct_repeat_max=1;
  int ct_return=0;

  char* source_file;
  FILE* source_fp = NULL;
  unsigned char* buf = NULL;
  long buf_size = 0;
  long foo = 0;
  int decode;

#ifdef OPENME
  openme_init(NULL,NULL,NULL,0);
  openme_callback("PROGRAM_START", NULL);
#endif
#ifdef XOPENME
  xopenme_init(1,0);
#endif

  if (getenv("CT_REPEAT_MAIN")!=NULL) ct_repeat_max=atol(getenv("CT_REPEAT_MAIN"));
              
  if (argc != 3) {
    fputs("usage: < --encode | --decode > < source >\n", stderr);
    ct_return = 1;
    goto program_end;
  }
  if (strcmp(argv[1], "--decode") == 0) {
    decode = 1;
  } else if (strcmp(argv[1], "--encode") == 0) {
    decode = 0;
  } else {
    fputs("usage: < --decode | --encode > < source >\n", stderr);
    ct_return = 1;
    goto program_end;
  }
  source_file = argv[2];

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

  int (*func)(unsigned char*, long, long*);
  func = decode ? &inf : &def;

#ifdef OPENME
  openme_callback("KERNEL_START", NULL);
#endif
#ifdef XOPENME
  xopenme_clock_start(0);
#endif

  for (ct_repeat=0; ct_repeat<ct_repeat_max; ct_repeat++) {
    ct_return = func(buf, buf_size, &foo);
    if (0 != ct_return) {
      zerr(ct_return);
      goto kernel_end;
    }
  }

kernel_end:

#ifdef XOPENME
  xopenme_clock_end(0);
#endif
#ifdef OPENME
  openme_callback("KERNEL_END", NULL);
#endif

  printf("Foo: %ld\n", foo);
    
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
