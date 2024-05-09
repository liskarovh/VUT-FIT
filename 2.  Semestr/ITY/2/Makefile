# K suboru: proj1.tex
# Datum:    22.2.2024
# Autor:    Jiri Pavela (ipavela@fit.vutbr.cz), upraveno by liskarovh
# Projekt:  ITY projekt 2

PROJ=proj2

$(PROJ).pdf: $(PROJ).tex
	latex $(PROJ).tex
	latex $(PROJ).tex
	dvips -t a4 $(PROJ).dvi
	ps2pdf $(PROJ).ps



clean:
	rm -f $(PROJ).aux $(PROJ).dvi $(PROJ).log $(PROJ).ps $(PROJ).out

clean-all: clean
	rm -f $(PROJ).pdf
