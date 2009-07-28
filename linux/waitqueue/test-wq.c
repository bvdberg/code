#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/jiffies.h>
#include <linux/wait.h>

MODULE_DESCRIPTION("Test for WaitQueues");
MODULE_AUTHOR("BvdB");
MODULE_LICENSE("GPL");

#define TIMER_INTERVAL 1000

static int timeout = 3;
static int with_timer = 0;

module_param(timeout, int, 0);
MODULE_PARM_DESC(timeout, "timeout value");

module_param(with_timer, int, 0);
MODULE_PARM_DESC(with_timer, "with_timer");

struct main_struct {
    wait_queue_head_t wait;
    unsigned int start_jiffies;
    struct timer_list timer;
};

static struct main_struct info;


static unsigned long getDelay(void) {
    return jiffies - info.start_jiffies;
}


static unsigned char condition(void) {
    unsigned char ret = jiffies >= info.start_jiffies + (5*HZ);
    printk("%s() condition=%s at jiffies=%lu\n", __func__, ret ? "true" : "false", getDelay());
    return ret;
}


static void handle_timer(unsigned long param) {
    printk("%s() at jiffies %lu\n", __func__, getDelay());
    mod_timer(&(info.timer), jiffies + msecs_to_jiffies(TIMER_INTERVAL));
    wake_up_interruptible(&info.wait);
}


static int __init my_module_init(void) {
    int ret = 0;
    info.start_jiffies = jiffies;
    printk("%s() START (timeout=%d  with_timer=%s) at jiffies %lu\n",
            __func__, timeout, with_timer ? "yes" : "no", getDelay());

    init_timer(&(info.timer));
    info.timer.data = (unsigned long) 0;
    info.timer.function = handle_timer;
    info.timer.expires = jiffies + msecs_to_jiffies(TIMER_INTERVAL);

    if (with_timer) add_timer(&info.timer);

    // IMPORTANT WAITQUEUE_PART
    init_waitqueue_head(&info.wait);
    ret = wait_event_interruptible_timeout(info.wait, condition(), timeout * HZ);
    if (ret == 0) {
        printk("%s() FINISH timeout at jiffies %lu\n", __func__, getDelay());
    } else {
        printk("%s() FINISH ret=%d at jiffies %lu\n", __func__, ret, getDelay());
    // IMPORTANT WAITQUEUE_PART

    }
    del_timer_sync(&info.timer);
    return 0;
}

static void __exit my_module_exit(void) {}

module_init(my_module_init);
module_exit(my_module_exit);

