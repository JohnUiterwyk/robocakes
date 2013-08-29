# Latex compiler
TEX = texi2pdf

# Arguments to pass to compiler
ARGS = --tidy

# Tex file to compile
SRC = specification.tex

# Destination filename
TARGET = robocakes-specification.pdf

all: 
	$(TEX) $(ARGS) $(SRC) -o $(TARGET)

clean:
	rm -rvf *t2d/
