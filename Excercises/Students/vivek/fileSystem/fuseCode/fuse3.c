 /*
 * minimal example filesystem using high-level API
 *
 * Compile with:
 *
 *     gcc -Wall fuse3.c `pkg-config fuse3 --cflags --libs` -o fuse3
 *
 * ## Source code ##
 * \include fuse1.c
 */

	
#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */

static struct options {
	const char *filename;
	const char *contents;
	int show_help;
} options;



static int myFuse_getattr(const char *path, struct stat *stbuf,
			 struct fuse_file_info *fi)
{
	(void) fi;
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0777;
		stbuf->st_nlink = 2;

	} else if (strcmp(path+1, "vivekFile.txt") == 0) {
		stbuf->st_mode = S_IFREG | 0777;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen("vivekFile.txt");

	} else
		res = -ENOENT;

 printf("I am out of it....\n\n");

	return res;
}




static int myFuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi,
			 enum fuse_readdir_flags flags){

	(void) offset;
	(void) fi;
	(void) flags;
  struct stat stbuf;
 
	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0, 0);
	filler(buf, "..", NULL, 0, 0);
 printf("I am out of it....\n\n");

	return 0;

}

int myFuse_mknod(const char *path, mode_t mode, dev_t rdev){

	int res;

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(path, mode);
	else
		res = mknod(path, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;

}

static int myFuse_open(const char *path, struct fuse_file_info *fi){

	int res;
 
  fi->flags = O_RDWR;
	
	res = open("vivekFile.txt", O_RDWR);

	if (res == -1)
		return -errno;

	fi->fh = res;
	return 0;

}



static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;
		fd = open(path, O_WRONLY);
	

	res = pwrite(fd, buf, size, offset);

		close(fd);
	return res;
}



static struct fuse_operations myFuse_oper = {
    .open	= myFuse_open,
		.getattr = myFuse_getattr,
		.readdir = myFuse_readdir,
		.mknod = myFuse_mknod,
		.write = xmp_write,
};



int main(int argc, char *argv[])
{
    while(1){
		 fuse_main(argc, argv, &myFuse_oper,NULL);
		}
	printf("I am out of it....\n\n");
return 0;
}
