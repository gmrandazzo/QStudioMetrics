# Makefile for QStudioMetrics Documentation using Pandoc

SOURCEDIR = source
BUILDDIR = build
FILENAME = MANUAL

.PHONY: all html pdf clean help

all: html pdf

help:
	@echo "Targets:"
	@echo "  html  : Generate HTML documentation"
	@echo "  pdf   : Generate PDF documentation (requires LaTeX)"
	@echo "  all   : Generate both HTML and PDF"
	@echo "  clean : Remove the build directory"

html:
	mkdir -p $(BUILDDIR)
	pandoc $(SOURCEDIR)/$(FILENAME).md -o $(BUILDDIR)/index.html --self-contained --metadata title="QStudioMetrics Manual"

pdf:
	mkdir -p $(BUILDDIR)
	pandoc $(SOURCEDIR)/$(FILENAME).md -o $(BUILDDIR)/$(FILENAME).pdf --metadata title="QStudioMetrics Manual"

clean:
	rm -rf $(BUILDDIR)