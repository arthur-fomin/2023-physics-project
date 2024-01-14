#!/bin/bash
set +ex
title="Компьютерное моделирование кельтcких камней. Артур Фомин"
#pandoc --from gfm --to html --standalone --output 2023-physics-artur-report.html --metadata-file metadata.yaml --metadata title="$title" *.md
pandoc --to pdf --standalone --pdf-engine=xelatex -V header-includes="\usepackage{pdfpages}" -V colorlinks=true -V linkcolor=green -V urlcolor=blue -V toccolor=gray --output 2023-physics-artur-report.pdf -V papersize:a4 -V fontsize=12pt -V geometry:margin=2cm --variable mainfont="Calibri" --variable monofont="Consolas" *.md

 
        