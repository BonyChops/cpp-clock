for i in `seq 0 9`
do
convert -size 18x30 xc:transparent -font Palatino-Bold -pointsize 36 -fill black -draw "text 0,25 '$i'" images/clock/$i.png
done
convert -size 9x30 xc:transparent -font Palatino-Bold -pointsize 36 -fill black -draw "text 0,25 ':'" images/clock/colon.png

convert -size 21x30 xc:transparent -font "Ani" -pointsize 36 -fill white -draw "text 0,28 '0'" images/digital/0.png
convert -size 21x30 xc:transparent -font "Ani" -pointsize 36 -fill white -draw "text 0,28 '1'" images/digital/1.png
for i in `seq 2 9`
do
convert -size 21x30 xc:transparent -font "Ani" -pointsize 36 -fill white -draw "text 0,28 '$i'" images/digital/$i.png
done