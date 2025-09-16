#ifndef _BACKPORT_DRM_DRM_EDID_H
#define _BACKPORT_DRM_DRM_EDID_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
#include_next <drm/drm_edid.h>
#else
/* For older kernels, include the legacy header */
#include <drm/drm_modes.h>
#include <drm/drm_connector.h>

/* Forward declarations only - actual definitions are in backport files */
struct drm_edid;
struct drm_edid_product_id;

#ifndef EDID_LENGTH
#define EDID_LENGTH 128
#endif

/* Function declarations - only if not already declared */
#ifndef _DRM_EDID_FUNCTIONS_DECLARED
#define _DRM_EDID_FUNCTIONS_DECLARED
void drm_edid_get_product_id(const struct drm_edid *drm_edid,
			     struct drm_edid_product_id *id);
void drm_edid_print_product_id(struct drm_printer *p,
			       const struct drm_edid_product_id *id, bool raw);
#endif
#endif

#endif