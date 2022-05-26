/*
 *  OpenSlide, a library for reading whole slide image files
 *
 *  Copyright (c) 2007-2015 Carnegie Mellon University
 *  Copyright (c) 2011 Google, Inc.
 *  All rights reserved.
 *
 *  OpenSlide is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 2.1.
 *
 *  OpenSlide is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with OpenSlide. If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

/*
 * Hologic Thinprep 2.0 WSI support
 *
 *
 */

#include <config.h>

#include "openslide-private.h"
#include "openslide-decode-jpeg.h"
#include "openslide-decode-sqlite.h"
#include "openslide-hash.h"
#include "openslide-vendor-generic-tiff.h"

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>
#include <string.h>
#include <errno.h>

// Software (35 ASCII): GDDS Imager 1.0.2.0, Hologic, Inc
static const char HOLOGIC_2_0_SOFTWARE_DESCRIPTION = "Hologic, Inc";
struct associated_image {
    struct _openslide_associated_image base;
}


static bool thinPrep2_0_detect(const char *filename G_GNUC_UNUSED,
struct _openslide_tifflike *tl,GError **err){
    // ensure the file is a tif file

    if(!tl){
        g_set_error(err, OPENSLIDE_ERROR, OPENSLIDE_ERROR_FAILED, "Not a TIFF File");
        return false;
    }

    // ensure tiff file is tiled
    if(!_openslide_tifflike_is_tiled(tl,0)) {
        g_set_error(err, OPENSLIDE_ERROR, OPENSLIDE_ERROR_FAILED,
        "TIFF is not tiled");
        return false;
    }

    // read XML decsription; check that is contains the software tag containing the string
    // Hologic, Inc.

    const char *image_desc = _openslide_tifflike_get_buffer(tl, 0, TIFFTAG_SOFTWARE, err);

    if(!image_desc){
        return false;
    }

    char *subString = strstr(image_desc,HOLOGIC_2_0_SOFTWARE_DESCRIPTION);
    if(subString){
        return true;
    }
    else{
        return false;
    }
}

// static void add_associated_jpeg(openslide_t *osr, TIFF *tiff,
//                                 const char *extension,
//                                 const char *name,
//                                 GError **err) {
// tdir_t *dir = 9;

// _openslide_tiff_add_associated_image(openslide_t *osr,
//                                           const char *name,
//                                           struct _openslide_tiffcache *tc,
//                                           tdir_t dir,
//                                           GError **err)

// }






destroy(openslide_t *osr);

read_tile(openslide_t *osr,
                      cairo_t *cr,
                      struct _openslide_level *level,
                      int64_t tile_col, int64_t tile_row,
                      void *arg,
                      GError **err);



paint_region(openslide_t *osr, cairo_t *cr,
                         int64_t x, int64_t y,
                         struct _openslide_level *level,
                         int32_t w, int32_t h,
                         GError **err);


width_compare(gconstpointer a, gconstpointer b);


generic_tiff_open(openslide_t *osr,
                              const char *filename,
                              struct _openslide_tifflike *tl,
                              struct _openslide_hash *quickhash1,
                              GError **err); 




const struct _openslide_format _openslide_format_thinprep_2_0 = {
    .name = "Hologic Thinprep 2.0",
    .vendor = "Hologic",
    .detect = thinPrep2_0_detect,
    .open = generic_tiff_open,
}