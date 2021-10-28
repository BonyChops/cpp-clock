for i in `seq 0 9`
do
convert -size 18x30 xc:transparent -font Palatino-Bold -pointsize 36 -fill black -draw "text 0,25 '$i'" images/$i.png
done
convert -size 9x30 xc:transparent -font Palatino-Bold -pointsize 36 -fill black -draw "text 0,25 ':'" images/colon.png