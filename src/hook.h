#ifndef HOOK_H_
#define HOOK_H_

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/stddef.h>
#include <linux/version.h>

struct __proc_dir_entry
{
  atomic_t in_use;
  refcount_t refcnt;
  struct list_head pde_openers;
  spinlock_t pde_unload_lock;
  struct completion *pde_unload_completion;
  const struct inode_operations *proc_iops;
  union
  {
    const struct proc_ops *proc_ops;
    const struct file_operations *proc_dir_ops;
  };
  const struct dentry_operations *proc_dops;
  union
  {
    const struct seq_operations *seq_ops;
    int (*single_show) (struct seq_file *, void *);
  };
  proc_write_t write;
  void *data;
  unsigned int state_size;
  unsigned int low_ino;
  nlink_t nlink;
  kuid_t uid;
  kgid_t gid;
  loff_t size;
  struct proc_dir_entry *parent;
  struct rb_root subdir;
  struct rb_node subdir_node;
  char *name;
  umode_t mode;
  u8 flags;
  u8 namelen;
  char inline_name[];
};

union proc_op
{
  int (*proc_get_link) (struct dentry *, struct path *);
  int (*proc_show) (struct seq_file *m, struct pid_namespace *ns,
                    struct pid *pid, struct task_struct *task);
  const char *lsm;
};

struct __proc_inode
{
  struct pid *pid;
  unsigned int fd;
  union proc_op op;
  struct proc_dir_entry *pde;
  struct ctl_table_header *sysctl;
  struct ctl_table *sysctl_entry;
  struct hlist_node sibling_inodes;
  const struct proc_ns_operations *ns_ops;
  struct inode vfs_inode;
};

extern void *_stext_addr;
extern void *_etext_addr;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HOOK_PROC(path, member, hook)                                   \
  hook_proc (path, offsetof (struct proc_ops, proc_##member), (void *)hook)
#else
#define HOOK_PROC(path, member, hook)                                   \
  hook_proc (path, offsetof (struct file_operations, member), (void *)hook)
#endif
#define UNHOOK_PROC(path, member, hook) HOOK_PROC (path, member, hook)

// swaps callback pointer and returns old pointer
void *hook_proc (char *path, unsigned long member, void *hook);

#endif // HOOK_H_
