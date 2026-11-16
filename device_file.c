#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/module.h>

static const char g_msg[] = "Hello world from kernel mode\n";
static const ssize_t g_size = sizeof(g_msg) - 1;

ssize_t device_file_read(struct file *file_ptr,
                         char __user *user_buffer,
                         size_t count,
                         loff_t *position)
{
    if (*position >= g_size)
        return 0;

    if (*position + count > g_size)
        count = g_size - *position;

    if (copy_to_user(user_buffer, g_msg + *position, count) != 0)
        return -EFAULT;

    *position += count;
    return count;
}
