#!/bin/bash
set +ex
title="Компьютерное моделирование кельстких камней. Артур Фомин"
#pandoc --from gfm --to html --standalone --output 2023-physics-artur-report.html --metadata-file metadata.yaml --metadata title="$title" *.md
pandoc --from gfm --to pdf --standalone --pdf-engine=xelatex -V colorlinks=true -V linkcolor=green -V urlcolor=blue -V toccolor=gray --output 2023-physics-artur-report.pdf -V papersize:a4 -V fontsize=12pt -V geometry:margin=0.5in --variable mainfont="Calibri" --variable monofont="Consolas" *.md

 
        