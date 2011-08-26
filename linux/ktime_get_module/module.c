#include <linux/kernel.h>
#include <linux/module.h>
//#include <linux/sched.h>
#include <linux/jiffies.h>
#include <linux/ktime.h>

MODULE_DESCRIPTION("Test for ktime_get");
MODULE_AUTHOR("AltenPTS");
MODULE_LICENSE("GPL");

static int interval = 1000;
module_param(interval, int, 0);
MODULE_PARM_DESC(interval, "interval (ms)");

extern ktime_t ktime_get(void);

struct main_struct {
    unsigned int start_jiffies;
    struct timer_list timer;
};

static struct main_struct info;


static unsigned long getDelay(void) {
    return jiffies - info.start_jiffies;
}


static void handle_timer(unsigned long param) {
    ktime_t t = ktime_get();

    printk("%s() at jiffies %lu  ktime_get=%d.%09d\n", __func__, getDelay(), t.tv.sec, t.tv.nsec);
    // TODO print ktime_get

    // reschedule
    mod_timer(&(info.timer), jiffies + msecs_to_jiffies(interval));
}


static int __init my_module_init(void) {
    info.start_jiffies = jiffies;

    printk("%s() START at jiffies %lu\n", __func__, getDelay());

    init_timer(&(info.timer));
    info.timer.data = (unsigned long) 0;
    info.timer.function = handle_timer;
    info.timer.expires = jiffies + msecs_to_jiffies(interval);

    add_timer(&info.timer);

    return 0;
}

static void __exit my_module_exit(void) {
    del_timer_sync(&info.timer);
}

module_init(my_module_init);
module_exit(my_module_exit);

