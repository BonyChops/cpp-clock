for i in `seq 9`
do
convert -size 36x30 xc:transparent -font Palatino-Bold -pointsize 36 -fill black -draw "text 9,25 '$i'" images/$i.png
done