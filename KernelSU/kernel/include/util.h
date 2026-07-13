#ifndef __KSU_H_UTIL
#define __KSU_H_UTIL

#include "linux/fdtable.h" // IWYU pragma: keep
#include <linux/version.h>
#include <linux/syscalls.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
#define ksu_close_fd close_fd
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
#define ksu_close_fd ksys_close
#else
#define ksu_close_fd sys_close
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
static inline long ksys_unshare(unsigned long flags)
{
	return sys_unshare(flags);
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 8, 0)
#define strncpy_from_user_nofault(dst, src, count) strncpy_from_unsafe((dst), (const void *)(src), (count))

static inline long copy_from_user_nofault(void *dst, const void __user *src, size_t size)
{
	long ret;
	mm_segment_t old_fs = get_fs();

	set_fs(USER_DS);
	pagefault_disable();
	ret = __copy_from_user_inatomic(dst, src, size);
	pagefault_enable();
	set_fs(old_fs);

	return ret ? -EFAULT : 0;
}

static inline long copy_to_user_nofault(void __user *dst, const void *src, size_t size)
{
	long ret;
	mm_segment_t old_fs = get_fs();

	set_fs(USER_DS);
	pagefault_disable();
	ret = __copy_to_user_inatomic(dst, src, size);
	pagefault_enable();
	set_fs(old_fs);

	return ret ? -EFAULT : 0;
}
#endif


#endif
