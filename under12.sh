for i in `seq 12`
do
convert -size 36x30 xc:transparent -font Palatino-Bold -pointsize 36 -fill black -draw "text 0,25 '$i'" images/$i.png
done