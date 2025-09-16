#include <linux/version.h>

#define DKMS_MODULE_VERSION "6.12.39-sriov"

/* Define missing config options for older kernels */
#ifndef CONFIG_DRM_I915_FENCE_TIMEOUT
#define CONFIG_DRM_I915_FENCE_TIMEOUT 0
#endif

/* Additional compatibility definitions */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 15, 0)
#define DRM_I915_BACKPORT_LEGACY_KERNEL 1
#endif