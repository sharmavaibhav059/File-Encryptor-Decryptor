/* fsd.c */ 

#include "fsd.h"

void usage(int8 *arg) {
  fprintf(stderr, "%s <source> <destination> \n", arg);

  return;
}

bool echo(bool on) {
  struct termios t;

  tcgetattr(0, &t);
  t.c_lflag = (on) ?
    t.c_lflag | ECHO :
    t.c_lflag & ~ECHO;
  tcsetattr(0, 0, &t);

  return on;
}

int8 *readkey(int8 *prompt) {
  char buf[256];
  int8  *key;
  //int8 size, idx;
  int size;
  
  printf("%s ", prompt);
  fflush(stdout);

  echo(false);
  memset(buf, 0, sizeof(buf));
  if(fgets(buf, sizeof(buf), stdin) == NULL){
    echo(true);
    return NULL;
  }

  size = strlen(buf);
  if (buf[size - 1] == '\n') {
        buf[size - 1] = '\0';
        size--;
  }

  key = (int8 *)malloc(size + 1);
  assert(key);
    memcpy(key, buf, size);
    key[size] = '\0';

    echo(true);
    printf("\n");
    return key;
}

Arcfour *initcrypto(int8 *key, int16 size) {
    Arcfour *state = rc4init(key, size);
    assert(state);
    return state;
}

int16 grabmagic(int32 fd) {
   int8 buf[2];

   ssize_t n = read(fd, buf, 2);
   assert( n == 2 );

   return (int16)(buf[0] | (buf[1] << 8));
 }

void skipbytes(int32 fd, int16 nbytes) {
    off_t result = lseek(fd, nbytes, SEEK_CUR);
    assert(result != (off_t)-1);
}

void decryptrestoffile(Arcfour *state, int32 in, int32 out) {
    int8 buffer[1024];
    ssize_t n;

    while ((n = read(in, buffer, sizeof(buffer))) > 0) {
        int8 *decrypted = rc4encrypt(state, buffer, (int16)n);
        write(out, decrypted, n);
        free(decrypted);
    }
}


int main( int argc, char *argv[]) {
  Arcfour *state;
  int8 *srcfile, *dstfile;
  int16 size, magic;
  int8 *key;
  int32 in,out;

  if(argc <3 ){
    usage((int8 *)*argv);
    return -1;
  }
  srcfile = (int8 *)argv[1];
  dstfile = (int8 *)argv[2];

  key = readkey((int8 *)"Key:");
 // printf("Key is %s\n", (char *)tmp);
  size = (int16)strlen((char *)key);
  assert(size);

  state = initcrypto(key, size);
  assert(state);

  in = (int32)open((char *)srcfile, O_RDONLY);
  assert( in > 0);
  out = (int32)open((char *)dstfile, O_WRONLY|O_CREAT, 00600);
  assert(out > 0);

  int16 padsize = grabmagic(in);
  skipbytes(in, padsize);

  decryptrestoffile(state, in, out);

    close(in);
    close(out);
    free(key);
    rc4uninit(state);



  return 0;
/*
  readkey();
  initcrypto();
  decryptint16();
  skipbytes();
  readhash();
  verifyhas();
  decryptrestoffile();
*/
  }
