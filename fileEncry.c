/* fse.c */

#include "fileEncry.h"

void changeecho(bool enabled) {
  struct termios *t;

  t = (struct termios *)malloc(sizeof(
        struct termios
        ));

  tcgetattr(0, t);
  if (enabled)
    t->c_lflag |= ECHO;
  else
    t->c_lflag &= ECHO;

    
  tcsetattr(0, 0, t);
  free(t);
  return;
}

int8 *securerand(int16 size) {
  int8 *start, *p;
  size_t n;

  assert(size > 0);
  p=(int8 *)malloc(size);
  assert(p);
  start = p;

  n = getrandom(p, (size_t)size,
      GRND_RANDOM|GRND_NONBLOCK);

  if (n == size) {
    return p;
  }
  else if (n < 0){
    free(p);
    return 0;
  }

  fprintf(stderr, "Warning: Etropy pool is"
      "empty. this may take longer than usual. \n");

  p += n;
  n = getrandom(p, (size-n), GRND_RANDOM);

  if(n == size)
    return start;
  else if (n < 0) {
    free(start);
    return 0;
  }
  return NULL;
}

int8 *readkey(char *prompt) {
  char buf[256];
  int8  *p;
  //int8 size, idx;
  int size;
  
  printf("%s ", prompt);
  fflush(stdout);

  changeecho(false);
  memset(buf, 0, sizeof(buf));
  if(fgets(buf, sizeof(buf), stdin) == NULL){
    changeecho(true);
    return NULL;
  }
/* 
  read(0, buf, 255);
  size = (int8)strlen(buf);
  idx =size - 1;
  p = (int8 *)buf + idx;
  *p = 0;
  */
  size = (int)strlen(buf);
  if(buf[size - 1] == '\n') {
    buf[size - 1] = '\0';
    size--;
  }

  p = (int8 *)malloc(size + 1);
  assert(p);
  memcpy(p, buf, size);
  p[size] = '\0';
  changeecho(true);\
  printf("\n");

  return p;

}

int main(int argc, char *argv[]) {
  Arcfour *rc4;
  char *infile, *outfile;
  int infd, outfd;
  int8 *key;
  int16 keysize;
  int16 padsize;
  //int16 *padsize16;
  int8 *padsize8;
  int8 *padding;

  if(argc < 3) {
    fprintf(stderr,
        "usage: %s <infile> <outfile>\n", *argv);
    return -1;
  }

  infile = argv[1];
  outfile = argv[2];

  infd = open(infile, O_RDONLY);
  if(infd < 1) {
    perror("open");
    return -1;
  }
  outfd = open(outfile, O_WRONLY|O_CREAT, 00600);
  if (outfd < 1) {
    close(infd);
    perror("open");

    return -1;
  }

  key = readkey("Key:");
  assert(key);
  keysize = (int16)strlen(((char *)key));

  rc4 = rc4init(key, keysize);
  assert(rc4);

  padsize8 =  securerand(2);
  if(!padsize8){
    fprintf(stderr, "Failed to generate padsize\n");
    return 1;
  }
  padsize = (padsize8[0] + padsize8[1]) % 64 +1;
  printf("padsize: %d\n", (int)padsize);

  write(outfd, &padsize, 2);
  padding = securerand(padsize);
  write(outfd, padding, padsize);

  //Encrypt the file 
  int8 buffer[1024];
  ssize_t n;
  while((n = read(infd, buffer, sizeof(buffer))) > 0) {
      int8 * encrypted = rc4encrypt(rc4, buffer, (int16)n);
      write(outfd, encrypted, n);
      free(encrypted);
      }
  
  close(infd);
  close(outfd);
  free(key);
  free(padding);
  free(padsize8);
  rc4uninit(rc4);

  return 0;

}
