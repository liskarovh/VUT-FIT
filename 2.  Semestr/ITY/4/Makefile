# K suboru: proj4.tex
# Datum:    15.4.2024
# Autor:    Jiri Pavela (ipavela@fit.vutbr.cz), upravila Hana Liškařová
# Projekt:  ITY projekt 4


PROJ=proj4

INCLUDE=czechiso.bst

$(PROJ).pdf: $(PROJ).tex
	latex $(PROJ).tex
	bibtex $(PROJ)
	latex $(PROJ).tex
	latex $(PROJ).tex
	dvips -t a4 $(PROJ).dvi
	ps2pdf $(PROJ).ps



clean:
	rm -f $(PROJ).aux $(PROJ).dvi $(PROJ).log $(PROJ).ps $(PROJ).out

clean-all: clean
	rm -f $(PROJ).pdf