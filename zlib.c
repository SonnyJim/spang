#include "spang.h"
#include <zlib.h>
//TODO Error checking

int zlib_deflate_to_file (char* src, int srclen, char* filename)
{
    FILE *fp;
    char* out;

    out = malloc (srclen);
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = srclen;
    defstream.next_in = (Bytef *) src;
    defstream.avail_out = srclen;
    defstream.next_out =  (Bytef *) out;

    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    fp = fopen (filename, "w");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening zfile for writing\n");
        return 0;
    }

    fwrite (out, defstream.total_out, 1, fp);
    fclose (fp);
    free (out);
    return defstream.total_out;
}

char* zlib_inflate_from_file (char* filename)
{
    FILE *fp;
    char* in;
    char* out;
    int filesize;
    z_stream infstream;

    fp = fopen (filename, "r");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening zrec %s for reading\n", filename);
        return NULL;
    }

    fseek (fp, 0, SEEK_END);
    filesize = ftell (fp);
    fseek (fp, 0, SEEK_SET);

    in = malloc (filesize);
    if (in == NULL)
    {
        fprintf (stderr, "Error mallocing %i for zlib deflate\n", filesize);
        return NULL;
    }
/*
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.avail_in = srclen; // size of input
    infstream.next_in = (Bytef *) src; // input char array
    infstream.avail_out = dstlen; // size of output
    infstream.next_out = (Bytef *) dst; // output char array

    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);

    return infstream.total_out;
    */
}

int zlib_deflate (char *src, int srclen, char *dst)
{
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = srclen;
    defstream.next_in = (Bytef *) src;
    defstream.avail_out = srclen;
    defstream.next_out =  (Bytef *) dst;

    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);
    return defstream.total_out;
}

int zlib_inflate (char *src, int srclen, char *dst, int dstlen)
{
    z_stream strm  = {0};
    strm.total_in  = strm.avail_in  = srclen;
    strm.total_out = strm.avail_out = dstlen;
    strm.next_in   = (Bytef *) src;
    strm.next_out  = (Bytef *) dst;

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    int err = -1;
    int ret = -1;

    err = inflateInit2(&strm, (15 + 32)); //15 window bits, and the +32 tells zlib to to detect if using gzip or zlib
    if (err == Z_OK) {
        err = inflate(&strm, Z_FINISH);
        if (err == Z_STREAM_END) {
            ret = strm.total_out;
        }
        else {
             inflateEnd(&strm);
             return err;
        }
    }
    else {
        inflateEnd(&strm);
        return err;
    }

    inflateEnd(&strm);
    return ret;
}
