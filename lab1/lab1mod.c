#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/ktime.h>


#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
#define HAVE_PROC_OPS
#endif
//seq_printf(m, "pid = %d\n", current->pid);
  //return 0;
  // struct definition
static int lab1_show(struct seq_file *m, void *v) {
  struct task_struct * cur_task = current;

  // Variables in place of the struct fields
  int PID = current -> pid;
  int PPID = task_ppid_nr(current);
  int RealUID = current -> cred -> uid.val;
  int EffectiveUID = current -> cred -> euid.val;
  int SavedUID = current -> cred -> suid.val;
  int RealGID = current -> cred -> gid.val;
  int EffectiveGID = current -> cred -> egid.val;
  int SavedGID = current -> cred -> sgid.val;

  // start of output
  seq_printf(m, "Current Process PCB Information\n");
  seq_printf(m, "Name = %s\n", current -> comm);
  seq_printf(m, "PID = %d\n", PID);
  seq_printf(m, "PPID = %d\n", PPID);
  seq_printf(m, "State = ");

  // logic for the different states
  // it checks each value associated with state type (running, waiting, stopped), then decides and outputs the state
  if (current -> state == 0x0000) {
    seq_printf(m, "Running\n");
  } else if (current -> state == 0x0001 || 0x0002) {
    seq_printf(m, "Waiting\n");
  } else if (current -> state == 0x0004) {
    seq_printf(m, "Stopped\n");
  } else {
    seq_printf(m, "error\n");
  }

  // continued output on the process about the currently running process
  seq_printf(m, "Real UID = %d\n", RealUID);
  seq_printf(m, "Effective UID = %d\n", EffectiveUID);
  seq_printf(m, "Saved UID = %d\n", SavedUID);
  seq_printf(m, "Real GID = %d\n", RealGID);
  seq_printf(m, "Effective GID = %d\n", EffectiveGID);
  seq_printf(m, "Saved GID = %d\n", SavedGID);

  return 0;

}

static int lab1_open(struct inode *inode, struct  file *file) {
  return single_open(file, lab1_show, NULL);
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops lab1_fops = {
  .proc_open = lab1_open,
  .proc_read = seq_read,
  .proc_lseek = seq_lseek,
  .proc_release = single_release,
};
#else
static const struct file_operations lab1_fops = {
  .owner = THIS_MODULE,
  .open = lab1_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};
#endif

static int __init lab1_init(void) {
  proc_create("lab1", 0, NULL, &lab1_fops);
  printk(KERN_INFO "lab1mod in\n");
  return 0;
}

static void __exit lab1_exit(void) {
  remove_proc_entry("lab1", NULL);
  printk(KERN_INFO "lab1mod out\n");
}

MODULE_LICENSE("GPL");
module_init(lab1_init);
module_exit(lab1_exit);
