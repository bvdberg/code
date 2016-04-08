all: sequence-diagram.pdf

sequence-diagram.pdf: sequence-diagram.tex
	pdflatex sequence-diagram.tex
	pdflatex sequence-diagram.tex
	pdflatex sequence-diagram.tex

clean:
	@${RM} *.aux *.log *.nav *.out *.snm *.toc *.vrb sequence-diagram.pdf *.idx

