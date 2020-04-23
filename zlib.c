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

    fp = fopen (filename, "wb");
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

int zlib_inflate_from_file (char* filename, char* dst, int bufflen)
{
    FILE *fp;
    char* in;
//    char* out;
    int filesize, ret;
    z_stream infstream;

    fp = fopen (filename, "rb");
    if (fp == NULL)
    {
        fprintf (stderr, "Error opening %s for reading\n", filename);
        return 1;
    }

    fseek (fp, 0, SEEK_END);
    filesize = ftell (fp);
    fseek (fp, 0, SEEK_SET);

    in = malloc (filesize);

    if (in == NULL)
    {
        fprintf (stderr, "Error mallocing %i bytes for inflate\n", filesize);
        return 1;
    }
    ret = fread (in, 1, filesize, fp);
    fclose (fp);
    if (ret != filesize)
    {
        fprintf (stderr, "Error in inflate, expected %i, got %i\n", filesize, ret);
        return 1;
    }

    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.avail_in = filesize; // size of input
    infstream.next_in = (Bytef *) in; // input char array
    infstream.avail_out = bufflen; // size of output
    infstream.next_out = (Bytef *) dst; // output char array

    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);

    return infstream.total_out;
}
