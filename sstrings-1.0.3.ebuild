# Copyright 1999-2004 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header
# path: /usr/portage/sys-libs/sstrings

inherit eutils
DESCRIPTION="A small C library that handles with C Strings in a safe way"
HOMEPAGE="http://pcpool.mathematik.uni-freiburg.de/~pabloy/projects/sstrings/"
SRC_URI="http://pcpool.mathematik.uni-freiburg.de/~pabloy/projects/sstrings/${P}.tar.gz"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="x86" #I did't tested it on other architectures, I think it works on
#other arc. well
IUSE=""

DEPEND="virtual/libc"
RDEPEND="virtual/libc"

src_compile() {
	econf || die "could not configure"
	emake || die "emake failed"
}

src_install() {
	make DESTDIR="${D}" install || die
	dodoc AUTHORS README TODO COPYING BUGS ChangeLog
}

pkg_postinst() {
	einfo "Type man sstrings for information about the functions"
	einfo "or visit $HOMEPAGE for online documentation"
}
