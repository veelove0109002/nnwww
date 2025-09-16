#ifndef _BACKPORT_DRM_DISPLAY_DRM_DSC_HELPER_H
#define _BACKPORT_DRM_DISPLAY_DRM_DSC_HELPER_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
#include_next <drm/display/drm_dsc_helper.h>
#else
/* For older kernels, include the legacy header or provide minimal definitions */
#include <drm/drm_dsc.h>
#endif

#endif