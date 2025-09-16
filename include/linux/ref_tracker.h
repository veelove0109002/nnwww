#ifndef _BACKPORT_LINUX_REF_TRACKER_H
#define _BACKPORT_LINUX_REF_TRACKER_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 14, 0)
#include_next <linux/ref_tracker.h>
#else
/* Stub implementation for older kernels */
struct ref_tracker_dir;
struct ref_tracker;

static inline void ref_tracker_dir_init(struct ref_tracker_dir *dir,
					 unsigned int stack_size) {}

static inline void ref_tracker_dir_exit(struct ref_tracker_dir *dir) {}

static inline void ref_tracker_alloc(struct ref_tracker_dir *dir,
				      struct ref_tracker **trackerp,
				      gfp_t gfp) {}

static inline int ref_tracker_free(struct ref_tracker_dir *dir,
				    struct ref_tracker **trackerp)
{
	return 0;
}

#define __ref_tracker_dir_print(d, s) do { } while (0)
#define ref_tracker_dir_print(d, s) do { } while (0)

#endif

#endif