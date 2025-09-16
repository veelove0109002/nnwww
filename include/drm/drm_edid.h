#ifndef _BACKPORT_DRM_DRM_EDID_H
#define _BACKPORT_DRM_DRM_EDID_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
#include_next <drm/drm_edid.h>
#else
/* For older kernels, include the legacy header */
#include <drm/drm_modes.h>
#include <drm/drm_connector.h>

/* Forward declarations for compatibility */
struct drm_edid;
struct drm_edid_product_id;

/* Stub function declarations */
void drm_edid_get_product_id(const struct drm_edid *drm_edid,
			     struct drm_edid_product_id *id);
#endif

#endif