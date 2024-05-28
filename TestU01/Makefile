# /usr/share/doc/testu01-doc/examples/Makefile
#
# Ad hoc Makefile for building and playing with the sample sources
# distributed within the debian package testu01-doc.
#
# Recommended usage:
#  create a dedicated folder somewhere in your HOME directory;
#  link all the files in /usr/share/doc/testu01-doc/examples in the dedicated folder;
#  launch this Makefile in the dedicated folder:
#  $ make ;
#  for a basic cleanup, consider the clean target:
#  $ make clean ;
#  for an entire cleanup, the maintainer-clean target:
#  $ make maintainer-clean
#  for other targets, just read the Makefile.
#
# written for Debian by Jerome Benoit <calculus@rezozer.net>
# on behalf of the Debian Science Team
# copyright: 2015-2017 Jerome Benoit <calculus@rezozer.net>
# distributed under the terms and conditions of GPL version 3 or later
#

SHELL = /bin/bash
#CC = gcc # Flag for implicit rules
#CFLAGS = -g0 -Wall
CFLAGS = -Wall -O3 -g0 
#-std=c99

default: all

#PROGRAMS = ex8 
#PROGRAMS = ex9
#PROGRAMS = ex9_64
PROGRAMS = ex11
#PROGRAMS = ex12
#PROGRAMS = ex13
#PROGRAMS = ex81 
#ex82
PROGRAMS_OLD = \
	bat1 \
	bat2 \
	bat3 \
	birth1 \
	birth2 \
	ex1 \
	ex3 \
	ex4 \
	ex7 \
	fbirth \
		fbirth-ltx \
	fcoll \
		fcoll-ltx \
		scat_gen \
	scat \
	scat2

RESULTFILES = \
	$(addsuffix .res,$(PROGRAMS))

PDFSCRAPFILES = \
	$(patsubst %.tex,%.pdf, $(filter %.tex,$(CLEANFILES)) $(wildcard *-ltx.tex) )

CLEANFILES= \
	bone.tex RandomOrg.tex

MOSTLYCLEANFILES =

DISTCLEANFILES = \
	$(PDFSCRAPFILES)

MAINTAINERCLEANFILES = \
	RandomOrg.pts

LDLIBS = -ltestu01

ex3: ex3.c my16807.c my16807.h
	$(CC) $(CFLAGS) $(filter-out %.h,$^) $(LDLIBS) -ltestu01mylib -o $@

ex7: ex7.c mrg32k3a.c xorshift.c
ex8: ex8.c AKalashnikovaGaloisLeftLFSR.c AKalashnikovaGaloisRightLFSR.c
ex81: ex81.c AKalashnikovaGaloisLeftLFSR.c
ex82: ex82.c AKalashnikovaGaloisRightLFSR.c
ex9: ex9.c AKalashnikovaPcg.c
ex9_64: ex9_64.c AKalashnikovaPcg64.c
ex11: ex11.c chacha20.c
ex12: ex12.c xchacha20.c
ex13: ex13.c chacha20_.c

all: build check pdf

build: $(PROGRAMS)

check: $(RESULTFILES)

pdf: $(filter %.pdf,$(PDFSCRAPFILES))

mostlyclean:
	$(if $(MOSTLYCLEANFILES), $(RM) $(MOSTLYCLEANFILES),)
	$(RM) $(COMPOSEDIR)/*
	test ! -d $(COMPOSEDIR) || $(RMDIR) $(COMPOSEDIR)

checkclean:
	$(RM) $(RESULTFILES)

clean: mostlyclean checkclean
	$(RM) $(PROGRAMS) $(CLEANFILES) $(wildcard *-table-*.tex)

distclean: clean
	$(RM) $(DISTCLEANFILES)

maintainer-clean: distclean
	$(RM) $(MAINTAINERCLEANFILES)

RandomOrg.pts: scat_gen
	./$<

scat.res RandomOrg.tex: scat RandomOrg.pts RandomOrg.dat
	./$< | tee $<.res

scat2.res bone.tex: scat2
	./$< | tee $<.res

%.res : %
	./$< | tee $@
#%.res : %
#	./$< > $@

%-ltx-table-00.tex %-ltx-table-01.tex \
%-ltx-table-02.tex %-ltx-table-03.tex \
%-ltx-table-04.tex %-ltx-table-05.tex \
%-ltx-table-06.tex %-ltx-table-07.tex \
%-ltx-table-08.tex %-ltx-table-09.tex \
%-ltx.res : %-ltx
	./$< | tee >( csplit -f "$<-table-" -b "%02d.tex" - %$(FLTXTABLEREGEX)% /$(FLTXTABLEREGEX)/ {*} ) $<.res

.SECONDEXPANSION:
%-ltx.pdf : $$(subst .pdf,.tex,$$@) \
		$$(shell sed -n 's@\\input\(\|table\){\(.*\)-table-\(.*\)}@\2-table-\3@p' $$(subst .pdf,.tex,$$@)) \
		$$(subst .pdf,.c,$$@)
	$(MKDIR_P) $(COMPOSEDIR)
	$(PDFLATEX) $(PDFLATEXFLAGS) -draftmode $<
	$(PDFLATEX) $(PDFLATEXFLAGS) $<
	$(MV) $(COMPOSEDIR)/$(@F) $@

FLTXTABLEREGEX:=%%%%%%%%%%%%%%%%%%%%%


%.pdf: %.tex
	$(MKDIR_P) $(COMPOSEDIR)
	$(PDFLATEX) $(PDFLATEXFLAGS) $<
	$(MV) $(COMPOSEDIR)/$(@F) $@

PDFLATEX = pdflatex

PDFLATEXFLAGS = \
	-no-shell-escape \
	-interaction=batchmode \
	-output-directory $(COMPOSEDIR)

COMPOSEDIR ?= $(TMPDIR)/mk/$(subst /,%,$(realpath $(firstword $(MAKEFILE_LIST))))%compose

TMPDIR ?= /tmp

MV ?= /bin/mv -f
MKDIR_P ?= /bin/mkdir -p
RMDIR ?= /bin/rmdir
