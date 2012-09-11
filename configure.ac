dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(netmd, 0.1)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CXX
AM_PROG_CC_STDC
AC_HEADER_STDC


AC_CHECK_LIB(pthread,[main])
AC_CHECK_LIB(rt,[main])
AC_CHECK_LIB(m,[main])
AC_CHECK_LIB(dl,[main])
AC_CHECK_LIB(portaudio,[main],,AC_MSG_ERROR([Missing a required library]))
AC_CHECK_LIB(usb,[main],,AC_MSG_ERROR([Missing a required library]))
AC_CHECK_LIB(sndfile,[main],,AC_MSG_ERROR([Missing a required library]))
AC_CHECK_HEADERS([portaudio.h],,AC_MSG_ERROR([Missing a required header]))
AC_CHECK_HEADERS([usb.h],,AC_MSG_ERROR([Missing a required header]))
AC_CHECK_HEADERS([sndfile.h],,AC_MSG_ERROR([Missing a required header]))


AC_OUTPUT([
Makefile
src/Makefile

])
