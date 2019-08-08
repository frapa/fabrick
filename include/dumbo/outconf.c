#include "outconf.h"

/*
 * Library with helper functions to manage the output configuration.
 * 
 * Features:
 *  - Data structure for storing a display configuration
 *  - Data structure for storing a output configuration
 */

#include "xf86drm.h"
#include "xf86drmMode.h"
#include "dumbo.h"

/* 
 * @param fd - File descriptor of a drm device (usually /dev/dri/card0).
 * @param buf_id - Id of the buffer to be used for scanout.
 *
 * @return error - Returns error code on error, zero on success.
 * 
 * Sets the buffer buf_id as the scanout buffer, with the conf
 * display configuration (combination of controller and output port).
 */
int set_buf_id(int fd, struct display_conf conf, uint32_t buf_id) {
	int err = drmModeSetCrtc(fd,
		conf.crtc_id, buf_id,
		0, 0,
		&conf.connector_id,
		1, &conf.mode
	);
	if (err) {
		return err;
	}
}

/* 
 * @param fd - File descriptor of a drm device (usually /dev/dri/card0).
 * @param buf - Buffer to be used for scanout.
 *
 * @return error - Returns error code on error, zero on success.
 * 
 * Sets the buffer buf as the scanout buffer, with the conf
 * display configuration (combination of controller and output port).
 */
int set_buf(int fd, struct display_conf conf, struct dumb_buf buf) {
    return set_buf_id(fd, conf, buf.id);
}

/*
 * @param fd - File descriptor of a drm device (usually /dev/dri/card0).
 * 
 * @return display_conf - Returns valid display_conf.
 * 
 * Finds the first valid display configuration possible and returns
 * it. This function is mean to quickly set up a display to show
 * something, either for debugging or testing or in situations
 * where a simple configuration is desirable (boot time).
 */
struct display_conf find_basic_conf(int fd) {
	struct display_conf conf;

	drmModeRes* res = drmModeGetResources(fd);
	for (int ci = 0; ci < res->count_connectors; ++ci) {
		uint32_t connector_id = res->connectors[ci];
		drmModeConnector* connector = drmModeGetConnector(fd, connector_id);

		if (!connector) continue;
		if (!connector->count_modes) continue;
		if (!connector->count_encoders) continue;

		// Select first valid connector
		conf.connector_id = connector_id;
		conf.mode = connector->modes[0];

		for (int ei = 0; ei < connector->count_encoders; ++ei) {
			uint32_t encoder_id = connector->encoders[ei];
			drmModeEncoder* encoder = drmModeGetEncoder(fd, encoder_id);

			if (!encoder) continue;

			for (int cj = 0; cj < res->count_crtcs; ++cj) {
				/* check whether this CRTC works with the encoder */
				if (!(encoder->possible_crtcs & (1 << cj))) continue;

				uint32_t ctrc_id = res->crtcs[cj];
				conf.crtc_id = ctrc_id;
			}

			drmModeFreeEncoder(encoder);
		}

		drmModeFreeConnector(connector);
	}

	drmModeFreeResources(res);

	return conf;
}