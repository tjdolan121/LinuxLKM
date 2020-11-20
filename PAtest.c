#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(void)
{
  int fd;
  char buf1[20];
  char buf2[20];
  char buf3[20];
  char buf4[20];
  char msg1[20] = "Message from test 1";
  char msg2[20] = "Message from test 2";
  char msg3[20] = "Message from test 4";  // TODO: Clean this up
  char passed[] = "Passed";
  char failed[] = "Failed";

  fd = open("/dev/simple_character_device", O_RDWR);

  /*=============== TEST 1: WRITE/SEEK_SET/READ ===============*/

  // Write msg1 to the device:
  if (write(fd, msg1, sizeof(msg1)) < 1) {
    printf("Write Error in test 1\n");
  }

  // Go back to the start to read
  if (lseek(fd, 0, SEEK_SET) < 0) {
    printf("Seek Error in test 1\n");
  }

  // Read from the device:
  if (read(fd, buf1, sizeof(msg1)) < 1) {
    printf("Read Error in test 1\n");
  }

  printf("----------------------------------------\n");
  printf("Test 1: WRITE/SEEK_SET/READ\n");
  printf("Expected : Message from test 1\n");
  printf("Actual   : %s\n", buf1);
  printf("Result: %s\n", (!strcmp(buf1, msg1)) ? passed : failed);
  printf("----------------------------------------\n");


  /*=============== TEST 2: SEEK_CUR =====================*/

  // Last step we read 20 bytes, so our current position should be
  // 20.  Let's test SEEK_CUR, which will move our pointer x bytes.

  
  loff_t position = lseek(fd, sizeof(msg1), SEEK_CUR); // move 20 more bytes
  if (position < (loff_t)0) {
    printf("Seek Error in Test 2\n");
  }

  printf("Test 2: SEEK_CUR\n");
  printf("Expected : 40 Bytes\n");
  printf("Actual   : %ld Bytes\n", position);
  printf("Result: %s\n", (position == 40) ? passed : failed);
  printf("----------------------------------------\n");

  /*=============== TEST 3: SEEK_END =====================*/

  // Last step we moved our pointer to the 40th byte in the buffer.
  // This implementation has EOF as the last byte written to the file,
  // which is currently still at the 20th byte from test 1.  So let's move
  // the buffer pointer back to the 20th byte using SEEK_END

  position = lseek(fd, 0, SEEK_END);  // Move to EOF
  if (position < (loff_t)0) {
    printf("Seek Error in Test 3\n");
 }

  printf("Test 3: SEEK_END\n");
  printf("Expected : 20 Bytes\n");
  printf("Actual   : %ld Bytes\n", position);
  printf("Result: %s\n", (position == 20) ? passed : failed);
  printf("----------------------------------------\n");


  close(fd);
  return 0;
}
