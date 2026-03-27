/*
 * mccp.c - support functions for mccp (the Mud Client Compression Protocol)
 *
 * see http://homepages.ihug.co.nz/~icecube/compress/ and README.Rom24-mccp
 *
 * Copyright (c) 1999, Oliver Jowett <icecube@ihug.co.nz>.
 *
 * This code may be freely distributed and used if this copyright notice is
 * retained intact.
 */
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "player.h"
#include "telnet.h"
#include <errno.h>

char    compress_start  [] = { IAC, SB, TELOPT_COMPRESS, WILL, SE, '\0' };

bool processCompressed(DESCRIPTOR_DATA *desc);
bool	write_to_descriptor	args( ( DESCRIPTOR_DATA *d, char *txt, int length ) );

/*
 * Memory management - zlib uses these hooks to allocate and free memory
 * it needs
 */

void *zlib_alloc(void *opaque, unsigned int items, unsigned int size)
{
    return calloc(items, size);
}

void zlib_free(void *opaque, void *address)
{
    free(address);
}

/*
 * Begin compressing data on `desc'
 */
bool compressStart(DESCRIPTOR_DATA *desc)
{
    z_stream *s;
    
    if (desc->out_compress) /* already compressing */
        return TRUE;

    /* allocate and init stream, buffer */
    s = (z_stream *)alloc_mem(sizeof(*s));
    desc->out_compress_buf = (unsigned char *)alloc_mem(COMPRESS_BUF_SIZE);
    
    s->next_in = NULL;
    s->avail_in = 0;

    s->next_out = desc->out_compress_buf;
    s->avail_out = COMPRESS_BUF_SIZE;

    s->zalloc = zlib_alloc;
    s->zfree  = zlib_free;
    s->opaque = NULL;

    if (deflateInit(s, 9) != Z_OK) {
        /* problems with zlib, try to clean up */
        free_mem(desc->out_compress_buf, COMPRESS_BUF_SIZE);
        free_mem(s, sizeof(z_stream));
        return FALSE;
    }

    write_to_descriptor(desc, compress_start, strlen(compress_start));

    /* now we're compressing */
    desc->out_compress = s;
    return TRUE;
}

/* Cleanly shut down compression on `desc' */
bool compressEnd(DESCRIPTOR_DATA *desc)
{
    unsigned char dummy[1];
    
    if (!desc->out_compress)
        return TRUE;

    desc->out_compress->avail_in = 0;
    desc->out_compress->next_in = dummy;

    /* No terminating signature is needed - receiver will get Z_STREAM_END */
    
    if (deflate(desc->out_compress, Z_FINISH) != Z_STREAM_END)
        return FALSE;

    if (!processCompressed(desc)) /* try to send any residual data */
        return FALSE;

    deflateEnd(desc->out_compress);
    free_mem(desc->out_compress_buf, COMPRESS_BUF_SIZE);
    free_mem(desc->out_compress, sizeof(z_stream));
    desc->out_compress = NULL;
    desc->out_compress_buf = NULL;

    return TRUE;
}

/* write_to_descriptor, the compressed case */
bool writeCompressed(DESCRIPTOR_DATA *desc, char *txt, int length)
{
    z_stream *s = desc->out_compress;
    
    s->next_in = (unsigned char *)txt;
    s->avail_in = length;

    while (s->avail_in) {
        s->avail_out = COMPRESS_BUF_SIZE - (s->next_out - desc->out_compress_buf);
            
        if (s->avail_out) {
            int status = deflate(s, Z_SYNC_FLUSH);

            if (status != Z_OK) {
                /* Boom */
                return FALSE;
            }
        }

        /* Try to write out some data.. */
        if (!processCompressed(desc))
            return FALSE;

        /* loop */
    }
    
    /* Done. */
    return TRUE;
}

/* User-level compression toggle */
void do_compress( CHAR_DATA *ch, char *argument )
{
    if (!ch->desc) {
        send_to_char("What descriptor?!\n", ch);
        return;
    }

    if (!ch->desc->out_compress) {
        if (!compressStart(ch->desc)) {
            send_to_char("Failed.\n", ch);
            return;
        }

        send_to_char("Ok, compression enabled.\n", ch);
    } else {
        if (!compressEnd(ch->desc)) {
            send_to_char("Failed.\n", ch);
            return;
        }

        send_to_char("Ok, compression disabled.\n", ch);
    }
}
