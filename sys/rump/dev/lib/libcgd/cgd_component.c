/*	$NetBSD: cgd_component.c,v 1.2 2016/01/26 23:12:15 pooka Exp $	*/

/*
 * Copyright (c) 2009 Antti Kantee.  All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: cgd_component.c,v 1.2 2016/01/26 23:12:15 pooka Exp $");

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/device.h>
#include <sys/stat.h>

#include <rump-sys/kern.h>
#include <rump-sys/vfs.h>

RUMP_COMPONENT(RUMP_COMPONENT_DEV)
{
	extern const struct bdevsw cgd_bdevsw;
	extern const struct cdevsw cgd_cdevsw;
	devmajor_t bmaj, cmaj;
	int error;

	/* go, mydevfs */
	bmaj = cmaj = -1;

	if ((error = devsw_attach("/dev/cgd0", &cgd_bdevsw, &bmaj,
	    &cgd_cdevsw, &cmaj)) != 0)
		panic("cannot attach cgd: %d", error);

	if ((error = rump_vfs_makedevnodes(S_IFBLK, "/dev/cgd0", 'a',
	    bmaj, 0, 7)) != 0)
		panic("cannot create cooked cgd dev nodes: %d", error);
	if ((error = rump_vfs_makedevnodes(S_IFCHR, "/dev/rcgd0", 'a',
	    cmaj, 0, 7)) != 0)
		panic("cannot create raw cgd dev nodes: %d", error);
}
