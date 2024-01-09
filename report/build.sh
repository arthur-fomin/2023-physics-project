#!/bin/bash
set +ex
title="Компьютерное моделирование кельстких камней. Артур Фомин"
pandoc --from gfm --to html --standalone --output 2023-physics-artur-report.html --metadata-file metadata.yaml --metadata title="$title" *.md
pandoc --from gfm --to pdf --standalone --pdf-engine=xelatex  --output 2023-physics-artur-report.pdf --variable mainfont="Calibri"  *.md

 
        