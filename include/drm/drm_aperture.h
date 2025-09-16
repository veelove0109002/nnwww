#ifndef _BACKPORT_DRM_DRM_APERTURE_H
#define _BACKPORT_DRM_DRM_APERTURE_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
#include_next <drm/drm_aperture.h>
#else
/* Stub implementation for older kernels */
struct pci_dev;
struct platform_device;

static inline int drm_aperture_remove_conflicting_pci_framebuffers(struct pci_dev *pdev,
								   const char *name)
{
	return 0;
}

static inline int drm_aperture_remove_conflicting_framebuffers(resource_size_t base,
							       resource_size_t size,
							       bool primary,
							       const char *name)
{
	return 0;
}
#endif

#endif