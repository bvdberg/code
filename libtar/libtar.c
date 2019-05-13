/*
**  Copyright 1998-2003 University of Illinois Board of Trustees
**  Copyright 1998-2003 Mark D. Roth
**  All rights reserved.
**
**  libtar.c - demo driver program for libtar
**
**  Mark D. Roth <roth@uiuc.edu>
**  Campus Information Technologies and Educational Services
**  University of Illinois at Urbana-Champaign
*/

/*
    Usage:
    list tarball contents: ./mytar -t test.tar
    create tarball of ~/bin:        ./mytar -C ~ -c test.tar bin
*/

#include <libtar.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/param.h>
#include <stdlib.h>

#ifdef HAVE_LIBZ
# include <zlib.h>
#endif

char *progname;
int verbose = 0;
int use_gnu = 0;

#ifdef HAVE_LIBZ

int use_zlib = 0;

static int gzopen_frontend(char *pathname, int oflags, int mode)
{
	char *gzoflags;
	gzFile gzf;
	int fd;

	switch (oflags & O_ACCMODE)
	{
	case O_WRONLY:
		gzoflags = "wb";
		break;
	case O_RDONLY:
		gzoflags = "rb";
		break;
	default:
	case O_RDWR:
		errno = EINVAL;
		return -1;
	}

	fd = open(pathname, oflags, mode);
	if (fd == -1)
		return -1;

	if ((oflags & O_CREAT) && fchmod(fd, mode))
		return -1;

	gzf = gzdopen(fd, gzoflags);
	if (!gzf)
	{
		errno = ENOMEM;
		return -1;
	}

	/* This is a bad thing to do on big-endian lp64 systems, where the
	   size and placement of integers is different than pointers.
	   However, to fix the problem 4 wrapper functions would be needed and
	   an extra bit of data associating GZF with the wrapper functions.  */
	return (int)gzf;
}

static tartype_t gztype = { (openfunc_t) gzopen_frontend, (closefunc_t) gzclose,
	(readfunc_t) gzread, (writefunc_t) gzwrite
};

#endif


static int create(char *tarfile, char *rootdir, libtar_list_t *l)
{
	TAR *t;
	char *pathname;
	char *buf = NULL;
	libtar_listptr_t lp;
	int len;

	if (tar_open(&t, tarfile,
#ifdef HAVE_LIBZ
		     (use_zlib ? &gztype : NULL),
#else
		     NULL,
#endif
		     O_WRONLY | O_CREAT, 0644,
		     (verbose ? TAR_VERBOSE : 0)
		     | (use_gnu ? TAR_GNU : 0)) == -1)
	{
		fprintf(stderr, "tar_open(): %s\n", strerror(errno));
		return -1;
	}

	libtar_listptr_reset(&lp);
	while (libtar_list_next(l, &lp) != 0)
	{
		pathname = (char *)libtar_listptr_data(&lp);
		if (pathname[0] != '/' && rootdir != NULL)
		{
			len = strlen(rootdir) + 1 + strlen(pathname);
			if ((buf = malloc(len + 1)) == NULL)
				return -1;
			snprintf(buf, len + 1, "%s/%s", rootdir, pathname);
		}
		else
		{
			len = strlen(pathname);
			if ((buf = malloc(len + 1)) == NULL)
				return -1;
			strncpy(buf, pathname, len + 1);
		}
		if (tar_append_tree(t, buf, pathname) != 0)
		{
			fprintf(stderr,
				"tar_append_tree(\"%s\", \"%s\"): %s\n", buf,
				pathname, strerror(errno));
			tar_close(t);
			free(buf);
			return -1;
		}
		free(buf);
	}

	if (tar_append_eof(t) != 0)
	{
		fprintf(stderr, "tar_append_eof(): %s\n", strerror(errno));
		tar_close(t);
		return -1;
	}

	if (tar_close(t) != 0)
	{
		fprintf(stderr, "tar_close(): %s\n", strerror(errno));
		return -1;
	}

	return 0;
}


static int list(char *tarfile)
{
	TAR *t;
	int i;

	if (tar_open(&t, tarfile,
#ifdef HAVE_LIBZ
		     (use_zlib ? &gztype : NULL),
#else
		     NULL,
#endif
		     O_RDONLY, 0,
		     (verbose ? TAR_VERBOSE : 0)
		     | (use_gnu ? TAR_GNU : 0)) == -1)
	{
		fprintf(stderr, "tar_open(): %s\n", strerror(errno));
		return -1;
	}

	while ((i = th_read(t)) == 0)
	{
		th_print_long_ls(t);
		if (TH_ISREG(t) && tar_skip_regfile(t) != 0)
		{
			fprintf(stderr, "tar_skip_regfile(): %s\n",
				strerror(errno));
			return -1;
		}
	}

	if (tar_close(t) != 0)
	{
		fprintf(stderr, "tar_close(): %s\n", strerror(errno));
		return -1;
	}

	return 0;
}


static int extract(char *tarfile, char *rootdir)
{
	TAR *t;

	if (tar_open(&t, tarfile,
#ifdef HAVE_LIBZ
		     (use_zlib ? &gztype : NULL),
#else
		     NULL,
#endif
		     O_RDONLY, 0,
		     (verbose ? TAR_VERBOSE : 0)
		     | (use_gnu ? TAR_GNU : 0)) == -1)
	{
		fprintf(stderr, "tar_open(): %s\n", strerror(errno));
		return -1;
	}

	if (tar_extract_all(t, rootdir) != 0)
	{
		fprintf(stderr, "tar_extract_all(): %s\n", strerror(errno));
		return -1;
	}

	if (tar_close(t) != 0)
	{
		fprintf(stderr, "tar_close(): %s\n", strerror(errno));
		return -1;
	}

	return 0;
}


static void usage()
{
	printf("Usage: %s [-C rootdir] [-g] [-z] -x|-t filename.tar\n",
	       progname);
	printf("       %s [-C rootdir] [-g] [-z] -c filename.tar ...\n",
	       progname);
	exit(-1);
}


#define MODE_LIST	 1
#define MODE_CREATE	 2
#define MODE_EXTRACT 3

int main(int argc, char *argv[])
{
	char *tarfile = NULL;
	char *rootdir = NULL;
	int c;
	int mode = 0;
	libtar_list_t *l;

	progname = basename(argv[0]);

	while ((c = getopt(argc, argv, "cC:gtvVxz")) != -1)
		switch (c)
		{
		case 'V':
			printf("libtar %s by Mark D. Roth <roth@uiuc.edu>\n",
			       libtar_version);
			break;
		case 'C':
			rootdir = strdup(optarg);
			break;
		case 'v':
			verbose = 1;
			break;
		case 'g':
			use_gnu = 1;
			break;
		case 'c':
			if (mode)
				usage();
			mode = MODE_CREATE;
			break;
		case 'x':
			if (mode)
				usage();
			mode = MODE_EXTRACT;
			break;
		case 't':
			if (mode)
				usage();
			mode = MODE_LIST;
			break;
#ifdef HAVE_LIBZ
		case 'z':
			use_zlib = 1;
			break;
#endif /* HAVE_LIBZ */
		default:
			usage();
		}

	if (!mode || ((argc - optind) < (mode == MODE_CREATE ? 2 : 1)))
	{
		usage();
	}

	switch (mode)
	{
	case MODE_EXTRACT:
		return extract(argv[optind], rootdir);
	case MODE_CREATE:
		tarfile = argv[optind];
		l = libtar_list_new(LIST_QUEUE, NULL);
		for (c = optind + 1; c < argc; c++)
			libtar_list_add(l, argv[c]);
		return create(tarfile, rootdir, l);
	case MODE_LIST:
		return list(argv[optind]);
	default:
		break;
	}

	/* NOTREACHED */
	return -2;
}


