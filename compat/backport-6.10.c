/*
 * Copyright (c) 2024
 *
 * Backport functionality introduced in Linux 6.10.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/version.h>
#include <linux/bitfield.h>
#include <linux/byteorder/generic.h>
#include <linux/seq_buf.h>

#include <drm/display/drm_dp_helper.h>
#include <drm/display/drm_dp_mst_helper.h>
#include <drm/drm_client.h>
#include <drm/drm_drv.h>
#include <drm/drm_edid.h>

/* Backport missing macros and functions for older kernels */
#ifndef DECLARE_SEQ_BUF
#define DECLARE_SEQ_BUF(name, sz) \
	char __##name##_buf[sz] = {}; \
	struct seq_buf name = { \
		.buffer = __##name##_buf, \
		.size = sz, \
	}
#endif

#ifndef seq_buf_str
static inline const char *seq_buf_str(struct seq_buf *s)
{
	s->buffer[s->len] = '\0';
	return s->buffer;
}
#endif

/* Define missing structures and constants */
#ifndef EDID_LENGTH
#define EDID_LENGTH 128
#endif

/* Only define if not already defined */
#ifndef _DRM_EDID_PRODUCT_ID_DEFINED
#define _DRM_EDID_PRODUCT_ID_DEFINED
struct drm_edid_product_id {
	__be16 manufacturer_name;
	__le16 product_code;
	__le32 serial_number;
	u8 week_of_manufacture;
	u8 year_of_manufacture;
};
#endif

/* Define missing timing structures for struct edid */
#ifndef _EDID_TIMING_STRUCTS_DEFINED
#define _EDID_TIMING_STRUCTS_DEFINED
struct est_timings {
	u8 t1;
	u8 t2;
	u8 mfg_rsvd;
} __attribute__((packed));

struct std_timing {
	u8 hsize; /* need to multiply by 8 then add 248 */
	u8 vfreq_aspect;
} __attribute__((packed));

struct detailed_pixel_timing {
	u8 hactive_lo;
	u8 hblank_lo;
	u8 hactive_hblank_hi;
	u8 vactive_lo;
	u8 vblank_lo;
	u8 vactive_vblank_hi;
	u8 hsync_offset_lo;
	u8 hsync_pulse_width_lo;
	u8 vsync_offset_pulse_width_lo;
	u8 hsync_vsync_offset_pulse_width_hi;
	u8 width_mm_lo;
	u8 height_mm_lo;
	u8 width_height_mm_hi;
	u8 hborder;
	u8 vborder;
	u8 misc;
} __attribute__((packed));

struct detailed_data_string {
	u8 str[13];
} __attribute__((packed));

struct detailed_data_monitor_range {
	u8 min_vfreq;
	u8 max_vfreq;
	u8 min_hfreq_khz;
	u8 max_hfreq_khz;
	u8 pixel_clock_mhz; /* need to multiply by 10 */
	u8 flags;
	union {
		struct {
			u8 reserved;
			u8 hfreq_start_khz; /* need to multiply by 2 */
			u8 c; /* need to divide by 2 */
			__le16 m;
			u8 k;
			u8 j; /* need to divide by 2 */
		} __attribute__((packed)) gtf2;
		struct {
			u8 version;
			u8 data1; /* high 6 bits: extra clock resolution */
			u8 data2; /* plus low 2 of above: max hactive */
			u8 supported_aspects;
			u8 flags; /* preferred aspect and blanking support */
			u8 supported_scalings;
			u8 preferred_refresh;
		} __attribute__((packed)) cvt;
	} formula;
} __attribute__((packed));

struct detailed_data_wpindex {
	u8 white_yx_lo; /* Lower 2 bits each */
	u8 white_x_hi;
	u8 white_y_hi;
	u8 gamma; /* need to divide by 100 then add 1 */
} __attribute__((packed));

struct detailed_data_color_point {
	u8 windex1;
	u8 wpindex1[3];
	u8 windex2;
	u8 wpindex2[3];
} __attribute__((packed));

struct detailed_non_pixel {
	u8 pad1;
	u8 type; /* ff=serial, fe=string, fd=monitor range, fc=monitor name
		    fb=color point data, fa=standard timing data,
		    f9=undefined, f8=mfg. reserved */
	u8 pad2;
	union {
		struct detailed_data_string str;
		struct detailed_data_monitor_range range;
		struct detailed_data_wpindex color;
		struct detailed_data_color_point colorpoint;
	} data;
} __attribute__((packed));

struct detailed_timing {
	__le16 pixel_clock; /* need to multiply by 10 KHz */
	union {
		struct detailed_pixel_timing pixel_data;
		struct detailed_non_pixel other_data;
	} data;
} __attribute__((packed));
#endif

/* Define struct edid for older kernels */
#ifndef _STRUCT_EDID_DEFINED
#define _STRUCT_EDID_DEFINED
struct edid {
	u8 header[8];
	/* Vendor & product info */
	u8 mfg_id[2];
	u8 prod_code[2];
	u32 serial; /* FIXME: byte order */
	u8 mfg_week;
	u8 mfg_year;
	/* EDID version */
	u8 version;
	u8 revision;
	/* Display info: */
	u8 input;
	u8 width_cm;
	u8 height_cm;
	u8 gamma;
	u8 features;
	/* Color characteristics */
	u8 red_green_lo;
	u8 black_white_lo;
	u8 red_x;
	u8 red_y;
	u8 green_x;
	u8 green_y;
	u8 blue_x;
	u8 blue_y;
	u8 white_x;
	u8 white_y;
	/* Est. timings and mfg rsvd timings*/
	struct est_timings established_timings;
	/* Standard timings 1-8*/
	struct std_timing standard_timings[8];
	/* Detailing timings 1-4 */
	struct detailed_timing detailed_timings[4];
	/* Number of 128 byte ext blocks */
	u8 extensions;
	/* Checksum */
	u8 checksum;
} __attribute__((packed));
#endif

/* Stub implementations for missing functions */
#ifndef drm_edid_decode_mfg_id
static inline void drm_edid_decode_mfg_id(u16 mfg_id, char vend[4])
{
	vend[0] = '@' + ((mfg_id >> 10) & 0x1f);
	vend[1] = '@' + ((mfg_id >> 5) & 0x1f);
	vend[2] = '@' + (mfg_id & 0x1f);
	vend[3] = '\0';
}
#endif
#include <drm/drm_print.h>
#include <drm/drm_vblank.h>


#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 10, 0)
void drm_dp_as_sdp_log(struct drm_printer *p,
                       const struct drm_dp_as_sdp *as_sdp) {
  drm_printf(p, "DP SDP: AS_SDP, revision %u, length %u\n", as_sdp->revision,
             as_sdp->length);
  drm_printf(p, "    vtotal: %d\n", as_sdp->vtotal);
  drm_printf(p, "    target_rr: %d\n", as_sdp->target_rr);
  drm_printf(p, "    duration_incr_ms: %d\n", as_sdp->duration_incr_ms);
  drm_printf(p, "    duration_decr_ms: %d\n", as_sdp->duration_decr_ms);
  drm_printf(p, "    operation_mode: %d\n", as_sdp->mode);
}

/**
 * drm_dp_read_mst_cap() - Read the sink's MST mode capability
 * @aux: The DP AUX channel to use
 * @dpcd: A cached copy of the DPCD capabilities for this sink
 *
 * Returns: enum drm_dp_mst_mode to indicate MST mode capability
 */
enum drm_dp_mst_mode drm_dp_read_mst_cap_compat(
    struct drm_dp_aux *aux,
    const u8 dpcd[DP_RECEIVER_CAP_SIZE]) // BACKPORT_COMPAT
{
  u8 mstm_cap;

  if (dpcd[DP_DPCD_REV] < DP_DPCD_REV_12)
    return DRM_DP_SST;

  if (drm_dp_dpcd_readb(aux, DP_MSTM_CAP, &mstm_cap) != 1)
    return DRM_DP_SST;
  if (mstm_cap & DP_MST_CAP)
    return DRM_DP_MST;
  if (mstm_cap & DP_SINGLE_STREAM_SIDEBAND_MSG)
    return DRM_DP_SST_SIDEBAND_MSG;

  return DRM_DP_SST;
}


struct drm_edid {
	/* Size allocated for edid */
	size_t size;
	const struct edid *edid;
};
/**
 * drm_edid_get_product_id - Get the vendor and product identification
 * @drm_edid: EDID
 * @id: Where to place the product id
 */
void drm_edid_get_product_id(const struct drm_edid *drm_edid,
                             struct drm_edid_product_id *id) {
  if (drm_edid && drm_edid->edid && drm_edid->size >= EDID_LENGTH) {
    memcpy(&id->manufacturer_name, &drm_edid->edid->mfg_id,
           sizeof(id->manufacturer_name));
    memcpy(&id->product_code, &drm_edid->edid->prod_code,
           sizeof(id->product_code));
    memcpy(&id->serial_number, &drm_edid->edid->serial,
           sizeof(id->serial_number));
    memcpy(&id->week_of_manufacture, &drm_edid->edid->mfg_week,
           sizeof(id->week_of_manufacture));
    memcpy(&id->year_of_manufacture, &drm_edid->edid->mfg_year,
           sizeof(id->year_of_manufacture));
  } else {
    memset(id, 0, sizeof(*id));
  }
}

static void decode_date(struct seq_buf *s, const struct drm_edid_product_id *id)
{
	int week = id->week_of_manufacture;
	int year = id->year_of_manufacture + 1990;
	if (week == 0xff)
		seq_buf_printf(s, "model year: %d", year);
	else if (!week)
		seq_buf_printf(s, "year of manufacture: %d", year);
	else
		seq_buf_printf(s, "week/year of manufacture: %d/%d", week, year);
}
/**
 * drm_edid_print_product_id - Print decoded product id to printer
 * @p: drm printer
 * @id: EDID product id
 * @raw: If true, also print the raw hex
 *
 * See VESA E-EDID 1.4 section 3.4.
 */
void drm_edid_print_product_id(struct drm_printer *p,
			       const struct drm_edid_product_id *id, bool raw)
{
	DECLARE_SEQ_BUF(date, 40);
	char vend[4];
	drm_edid_decode_mfg_id(be16_to_cpu(id->manufacturer_name), vend);
	decode_date(&date, id);
	drm_printf(p, "manufacturer name: %s, product code: %u, serial number: %u, %s\n",
		   vend, le16_to_cpu(id->product_code),
		   le32_to_cpu(id->serial_number), seq_buf_str(&date));
	if (raw)
		drm_printf(p, "raw product id: %*ph\n", (int)sizeof(*id), id);
	WARN_ON(seq_buf_has_overflowed(&date));
}

static struct drm_vblank_crtc *
drm_vblank_crtc(struct drm_device *dev, unsigned int pipe)
{
	return &dev->vblank[pipe];
}

struct drm_vblank_crtc *
drm_crtc_vblank_crtc(struct drm_crtc *crtc)
{
	return drm_vblank_crtc(crtc->dev, drm_crtc_index(crtc));
}

/**
 * drm_dp_as_sdp_supported() - check if adaptive sync sdp is supported
 * @aux: DisplayPort AUX channel
 * @dpcd: DisplayPort configuration data
 *
 * Returns true if adaptive sync sdp is supported, else returns false
 */
bool drm_dp_as_sdp_supported(struct drm_dp_aux *aux, const u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	u8 rx_feature;
	if (dpcd[DP_DPCD_REV] < DP_DPCD_REV_13)
		return false;
	if (drm_dp_dpcd_readb(aux, DP_DPRX_FEATURE_ENUMERATION_LIST_CONT_1,
			      &rx_feature) != 1) {
		/* For older kernels without drm_dev in aux, use a simple debug */
		printk(KERN_DEBUG "Failed to read DP_DPRX_FEATURE_ENUMERATION_LIST_CONT_1\n");
		return false;
	}
	return (rx_feature & DP_ADAPTIVE_SYNC_SDP_SUPPORTED);
}

/**
 * drm_dp_mst_aux_for_parent() - Get the AUX device for an MST port's parent
 * @port: MST port whose parent's AUX device is returned
 *
 * Return the AUX device for @port's parent or NULL if port's parent is the
 * root port.
 */
struct drm_dp_aux *drm_dp_mst_aux_for_parent(struct drm_dp_mst_port *port)
{
	if (!port->parent || !port->parent->port_parent)
		return NULL;
	return &port->parent->port_parent->aux;
}

void drm_client_dev_unregister(struct drm_device *dev)
{
	struct drm_client_dev *client, *tmp;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return;

	mutex_lock(&dev->clientlist_mutex);
	list_for_each_entry_safe(client, tmp, &dev->clientlist, list) {
		list_del(&client->list);
		if (client->funcs && client->funcs->unregister) {
			client->funcs->unregister(client);
		} else {
			drm_client_release(client);
			kfree(client);
		}
	}
	mutex_unlock(&dev->clientlist_mutex);
}

/**
 *	sysfs_bin_attr_simple_read - read callback to simply copy from memory.
 *	@file:	attribute file which is being read.
 *	@kobj:	object to which the attribute belongs.
 *	@attr:	attribute descriptor.
 *	@buf:	destination buffer.
 *	@off:	offset in bytes from which to read.
 *	@count:	maximum number of bytes to read.
 *
 * Simple ->read() callback for bin_attributes backed by a buffer in memory.
 * The @private and @size members in struct bin_attribute must be set to the
 * buffer's location and size before the bin_attribute is created in sysfs.
 *
 * Bounds check for @off and @count is done in sysfs_kf_bin_read().
 * Negative value check for @off is done in vfs_setpos() and default_llseek().
 *
 * Returns number of bytes written to @buf.
 */
 ssize_t sysfs_bin_attr_simple_read(struct file *file, struct kobject *kobj,
	struct bin_attribute *attr, char *buf,
	loff_t off, size_t count)
{
	memcpy(buf, attr->private + off, count);
	return count;
}
EXPORT_SYMBOL_NS(sysfs_bin_attr_simple_read, I915_SRIOV_COMPAT);
#endif