## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

netmddocdir = ${prefix}/doc/netmd
netmddoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(netmddoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(netmddocdir)
