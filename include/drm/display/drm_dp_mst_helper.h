#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
#include_next <drm/display/drm_dp_mst_helper.h>
#else
/* For older kernels, include the legacy header */
#include <drm/drm_dp_mst_helper.h>
#endif

#ifndef _BACKPORT_DRM_DISPLAY_DRM_DP_MST_HELPER_H
#define _BACKPORT_DRM_DISPLAY_DRM_DP_MST_HELPER_H

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 10, 0)
/**
 * enum drm_dp_mst_mode - sink's MST mode capability
 */
enum drm_dp_mst_mode {
	/**
	 * @DRM_DP_SST: The sink does not support MST nor single stream sideband
	 * messaging.
	 */
	DRM_DP_SST,
	/**
	 * @DRM_DP_MST: Sink supports MST, more than one stream and single
	 * stream sideband messaging.
	 */
	DRM_DP_MST,
	/**
	 * @DRM_DP_SST_SIDEBAND_MSG: Sink supports only one stream and single
	 * stream sideband messaging.
	 */
	DRM_DP_SST_SIDEBAND_MSG,
};
enum drm_dp_mst_mode drm_dp_read_mst_cap_compat(struct drm_dp_aux *aux, const u8 dpcd[DP_RECEIVER_CAP_SIZE]); // BACKPORT_COMPAT

static inline
bool drm_dp_mst_port_is_logical(struct drm_dp_mst_port *port)
{
	return port->port_num >= DP_MST_LOGICAL_PORT_0;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 12, 0)
void drm_dp_mst_topology_queue_probe(struct drm_dp_mst_topology_mgr *mgr);

struct drm_dp_aux *drm_dp_mst_aux_for_parent(struct drm_dp_mst_port *port);
#endif

#endif