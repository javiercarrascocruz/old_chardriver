#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>

MODULE_AUTHOR("Javi");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("First module tutorial");

static char ker_buf[100];
static int currLen = 0;
static int dev_open(struct inode *inod, struct file *fil);
static ssize_t dev_read(struct file *fil, char *buf, size_t len, loff_t *off);
static ssize_t dev_write(struct file *fil, const char *buf, size_t len, loff_t *off);
static int dev_release(struct inode *inod, struct file *fil);

static struct file_operations fops = {
  .read = dev_read,
  .write = dev_write,
  .open = dev_open,
  .release = dev_release,
};

static int init_hello(void)
{
  printk(KERN_INFO "Hello Javi, devdriver2 loaded\n");
  int t = register_chrdev(90, "devdriver2", &fops);
  if(!t)
    printk(KERN_ALERT "devdriver2 registration failed\n");
  else
    printk(KERN_ALERT "devdriver2 registration accomplished\n");
  return 0;
}

static void exit_hello(void)
{
  unregister_chrdev(90, "devdriver2");
  printk(KERN_INFO "Bye Javi, devdriver2 unloaded\n");
}

static int dev_open(struct inode *inod, struct file *fil)
{
  printk(KERN_INFO "dev2 opened\n");
  return 0;
}

static ssize_t dev_read(struct file *fil, char *buf, size_t len, loff_t *off)
{
  //Get data from user space to kernel space
  printk(KERN_INFO "Reading %d chars from dev2\n", len);
  copy_to_user(buf,ker_buf, currLen);
  return currLen;
  ker_buf[len] = 0;
  currLen = len;
  return len;
}

static ssize_t dev_write(struct file *fil, const char *buf, size_t len, loff_t *off)
{
  //Get data from user space to kernel space
  printk(KERN_INFO "Writing %d chars to dev2\n", len);
  copy_from_user(ker_buf, buf, len);
  ker_buf[len] = 0;
  currLen = len;
  return len;
}

static int dev_release(struct inode *inod, struct file *fil)
{
  printk(KERN_INFO "dev2 closed\n");
  return 0;
}

module_init(init_hello);
module_exit(exit_hello);
