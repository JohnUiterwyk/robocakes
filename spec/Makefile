# Latex compiler
TEX = texi2pdf

# LATEX specific options for outputting to text
LATEX = latex
OUTPUTDIR=robocakes-specification.t2d

# Arguments to pass to compiler
ARGS = --tidy
LATEXARGS = --output-directory=$(OUTPUTDIR)

# Tex file to compile
SRC = specification.tex
SRCHTML = $(SRC:.tex=.html)

# Destination filename
TARGET = robocakes-specification.pdf
TARGETTEXT = $(TARGET:.pdf=.txt)

all: 
	$(TEX) $(ARGS) $(SRC) -o $(TARGET)

text:
	htlatex $(SRC) "" "" "" ""
	links -dump $(SRCHTML) > $(TARGETTEXT) 

clean:
	-rm -rv *t2d/
	-$(RM) *.html
	-$(RM) *.4ct
	-$(RM) *.4tc
	-$(RM) *.aux
	-$(RM) *.css
	-$(RM) *.dvi
	-$(RM) *.idv
	-$(RM) *.lg
	-$(RM) *.log
	-$(RM) *.tmp
	-$(RM) *.xref

clean-all: clean
	-$(RM) *.pdf
	-$(RM) *.txt

_dummy := $(shell mkdir -p $(OUTPUTDIR))
