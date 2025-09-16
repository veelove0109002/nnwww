#ifndef _BACKPORT_DRM_DRM_EDID_H
#define _BACKPORT_DRM_DRM_EDID_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
#include_next <drm/drm_edid.h>
#else
/* For older kernels, include the legacy header */
#include <drm/drm_modes.h>
#include <drm/drm_connector.h>

/* Forward declarations and definitions for compatibility */
struct drm_edid;

#ifndef EDID_LENGTH
#define EDID_LENGTH 128
#endif

struct drm_edid_product_id {
	__be16 manufacturer_name;
	__le16 product_code;
	__le32 serial_number;
	u8 week_of_manufacture;
	u8 year_of_manufacture;
};

/* Stub function declarations */
void drm_edid_get_product_id(const struct drm_edid *drm_edid,
			     struct drm_edid_product_id *id);
#endif

#endif