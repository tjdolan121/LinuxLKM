#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static int my_open(struct inode* inode, struct file* file);
static int my_release(struct inode* inode, struct file* file);
ssize_t my_read(struct file* fin, char* user_buf, size_t size, loff_t* offset);
ssize_t my_write(struct file* fin, const char* user_buf, size_t size, loff_t* offset);
loff_t my_llseek(struct file* fin, loff_t offset, int whence);

static struct file_operations simple_driver_fops =
{
  .owner	= THIS_MODULE,
  .open		= my_open,
  .release	= my_release,
  .read		= my_read,
  .write	= my_write,
  .llseek	= my_llseek,
};

static int times_opened = 0;
static char buffer[1024];
static char *buffer_ptr;
static char *file_end = buffer;

static int my_open(struct inode* inode, struct file* file)
{
  times_opened++;
  buffer_ptr = buffer;
  return 0;
}

static int my_release(struct inode* inode, struct file* file)
{
  printk(KERN_ALERT "Times opened : %d", times_opened);
  return 0;
}

ssize_t my_read(struct file* fin, char* user_buf, size_t size, loff_t* offset)
{
  if (copy_to_user(user_buf, buffer_ptr, size) < 0) {
    return -1;
  }
  buffer_ptr += size;
  return size;
}

ssize_t my_write(struct file* fin, const char* user_buf, size_t size, loff_t* offset)
{
  if (copy_from_user(buffer_ptr, user_buf, size) < 0)
    return -1;
  if (file_end < (buffer_ptr + size)) {
    file_end = buffer_ptr + size;
  }
  buffer_ptr += size;
  return size;
}

loff_t my_llseek(struct file* fin, loff_t offset, int whence)
{
  if (whence == SEEK_SET) {
    buffer_ptr = buffer;
    buffer_ptr += offset;
  }
  else if (whence == SEEK_CUR) {
    buffer_ptr += offset;
  }
  else if (whence == SEEK_END) {
    buffer_ptr = file_end;
    buffer_ptr += offset;
  }
  else {
    return -1;
  }
  return buffer_ptr - buffer;
}

// INIT/EXIT

int my_driver_init(void) {
  // Write to log: we have loaded the module
  printk(KERN_ALERT "inside %s function\n", __FUNCTION__);
  // REGISTER DEVICE
  register_chrdev(240, "simple_character_device", &simple_driver_fops);
  return 0;
}

void my_driver_exit(void) {
  // Write to log: we have unloaded the module
  printk(KERN_ALERT "inside %s function\n", __FUNCTION__);
  // UNREGISTER DEVICE
  unregister_chrdev(240, "simple_character_device");
}

module_init(my_driver_init);
module_exit(my_driver_exit);
